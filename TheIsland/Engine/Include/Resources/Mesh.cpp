#include "Mesh.h"
#include "FbxLoader.h"
#include "../Device.h"
#include "../Component/Material.h"
#include "../Component/Animation.h"
#include "../Core/PathManager.h"
#include "../Core/NavigationMesh.h"

GAME_USING

CMesh::CMesh()
{
	SetTag( "Mesh" );
	SetTypeName( "CMesh" );
	SetTypeID<CMesh>();
	m_pAnimation = NULL;
	m_vMin.x = 10000000.f;
	m_vMin.y = 10000000.f;
	m_vMin.z = 10000000.f;

	m_vMax.x = -10000000.f;
	m_vMax.y = -10000000.f;
	m_vMax.z = -10000000.f;
}

CMesh::~CMesh()
{
	SAFE_RELEASE( m_pAnimation );

	for ( size_t i = 0; i < m_vecMeshContainer.size(); ++i )
	{
		SAFE_RELEASE( m_vecMeshContainer[i]->tVB.pBuffer );
		SAFE_DELETE_ARRAY( m_vecMeshContainer[i]->tVB.pVtxData );
		Safe_Release_VecList( m_vecMeshContainer[i]->vecMaterial );

		/*if (m_vecMeshContainer[i]->pStreamVB)
		{
			SAFE_RELEASE(m_vecMeshContainer[i]->pStreamVB->pBuffer);
			SAFE_DELETE(m_vecMeshContainer[i]->pStreamVB);
		}*/

		for ( size_t j = 0; j < m_vecMeshContainer[i]->vecIB.size(); ++j )
		{
			SAFE_RELEASE( m_vecMeshContainer[i]->vecIB[j].pBuffer );
			SAFE_DELETE_ARRAY( m_vecMeshContainer[i]->vecIB[j].pIdxData );
		}

		SAFE_DELETE( m_vecMeshContainer[i] );
	}

	m_vecMeshContainer.clear();
}

void CMesh::SetShaderKey( const string & strShaderKey )
{
	m_strShaderKey = strShaderKey;
}

void CMesh::SetInputLayoutKey( const string & strInputLayoutKey )
{
	m_strInputLayoutKey = strInputLayoutKey;
}

size_t CMesh::GetContainerCount() const
{
	return m_vecMeshContainer.size();
}

size_t CMesh::GetSubsetCount( int iContainer ) const
{
	return m_vecMeshContainer[iContainer]->vecMaterial.size();
}

string CMesh::GetShaderKey() const
{
	return m_strShaderKey;
}

string CMesh::GetInputLayoutKey() const
{
	return m_strInputLayoutKey;
}

Vector3 CMesh::GetLength() const
{
	return m_vLength;
}

Vector3 CMesh::GetCenter() const
{
	return m_vCenter;
}

float CMesh::GetRadius() const
{
	return m_fRadius;
}

string CMesh::GetKey() const
{
	return m_strKey;
}

void CMesh::SetKey( const string & strKey )
{
	m_strKey = strKey;
}

CMaterial * CMesh::CloneMaterial( int iContainer, int iSubset )
{
	return m_vecMeshContainer[iContainer]->vecMaterial[iSubset]->Clone();
}

CAnimation * CMesh::CloneAnimation() const
{
	if ( !m_pAnimation )
		return NULL;
	return m_pAnimation->Clone();
}

void CMesh::GetMeshPosition( vector<Vector3>* vecPos, int iContainer )
{
	// 정점 수만큼 반복한다.
	char*	pVtx = ( char* )m_vecMeshContainer[iContainer]->tVB.pVtxData;
	for ( int i = 0; i < m_vecMeshContainer[iContainer]->tVB.iVtxCount; ++i )
	{
		int	iSize = m_vecMeshContainer[iContainer]->tVB.iVtxSize;

		Vector3	vPos;
		memcpy( &vPos, pVtx + ( i * iSize ), sizeof( Vector3 ) );

		vecPos->push_back( vPos );
	}
}

void CMesh::GetMeshIndex( vector<int>* vecIndex, int iContainer, int iSubset )
{
	char*	pIdx = ( char* )m_vecMeshContainer[iContainer]->vecIB[iSubset].pIdxData;

	for ( int i = 0; i < m_vecMeshContainer[iContainer]->vecIB[iSubset].iIdxCount; ++i )
	{
		switch ( m_vecMeshContainer[iContainer]->vecIB[iSubset].iIdxSize )
		{
		case 2:
		{
			short	sIdx;
			memcpy( &sIdx, pIdx + ( i * 2 ), 2 );
			vecIndex->push_back( sIdx );
		}
		break;
		case 4:
		{
			int	iIdx;
			memcpy( &iIdx, pIdx + ( i * 4 ), 4 );
			vecIndex->push_back( iIdx );
		}
		break;
		}
	}
}

void CMesh::CreateNaviMeshInfo( CNavigationMesh * pNaviMesh, const Vector3& vScale,
	const Vector3& vLandPos )
{
	for ( size_t i = 0; i < m_vecMeshContainer.size(); ++i )
	{
		vector<Vector3>	vecPos;

		GetMeshPosition( &vecPos, i );

		// 메쉬에서 인덱스 정보를 얻어온다.
		for ( size_t j = 0; j < m_vecMeshContainer[i]->vecIB.size(); ++j )
		{
			vector<int>		vecIndex;
			GetMeshIndex( &vecIndex, i, j );


			for ( size_t k = 0; k < vecIndex.size(); k += 3 )
			{
				Vector3	vPos[3];
				vPos[0] = vecPos[vecIndex[k]] * vScale + vLandPos;
				vPos[1] = vecPos[vecIndex[k + 1]] * vScale + vLandPos;
				vPos[2] = vecPos[vecIndex[k + 2]] * vScale + vLandPos;

				pNaviMesh->AddCell( vPos );
			}

			// 격자맵일 경우 규칙이 정해져있기때문에 그 규칙에 의해서 인접정보를 만들어낸다.
			//pNaviMesh->CreateGridMapAdj(128);
			vecIndex.clear();
		}

		vecPos.clear();
	}

	pNaviMesh->CreateGridMapAdj( 128 );
}

bool CMesh::CreateMesh( UINT iVtxCount, UINT iVtxSize, D3D11_USAGE eVtxUsage,
	D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void * pVtxData, UINT iIdxCount,
	UINT iIdxSize, D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt, void * pIdxData )
{
	PMESHCONTAINER	pContainer = new MESHCONTAINER;

	m_vecMeshContainer.push_back( pContainer );

	if ( !CreateVertexBuffer( iVtxCount, iVtxSize, eVtxUsage, ePrimitive,
		pVtxData ) )
		return false;

	if ( !CreateIndexBuffer( iIdxCount, iIdxSize, eIdxUsage, eFmt,
		pIdxData ) )
		return false;

	m_vLength = m_vMax - m_vMin;
	m_vCenter = ( m_vMax + m_vMin ) / 2.f;

	m_fRadius = m_vLength.x > m_vLength.y ? m_vLength.x : m_vLength.y;
	m_fRadius = m_fRadius > m_vLength.z ? m_fRadius : m_vLength.z;
	m_fRadius /= 2.f;

	return true;
}

bool CMesh::LoadMesh( const string & strKey, const char * pFullPath )
{
	m_strKey = strKey;

	char	strExt[_MAX_EXT] = {};
	_splitpath_s( pFullPath, 0, 0, 0, 0, 0, 0, strExt, _MAX_EXT );

	_strupr_s( strExt );

	if ( strcmp( strExt, ".FBX" ) == 0 )
	{
		CFbxLoader	loader;

		if ( !loader.LoadFbx( pFullPath ) )
			return false;

		return ConvertFbx( &loader );
	}

	return LoadFromFullPath( pFullPath );
}

bool CMesh::CreateVertexBuffer( UINT iVtxCount, UINT iVtxSize,
	D3D11_USAGE eVtxUsage, D3D11_PRIMITIVE_TOPOLOGY ePrimitive,
	void * pVtxData )
{
	PMESHCONTAINER	pContainer = m_vecMeshContainer[m_vecMeshContainer.size() - 1];

	pContainer->tVB.pBuffer = NULL;
	pContainer->tVB.iVtxCount = iVtxCount;
	pContainer->tVB.iVtxSize = iVtxSize;
	pContainer->tVB.eUsage = eVtxUsage;
	pContainer->tVB.ePrimitive = ePrimitive;
	pContainer->tVB.pVtxData = new char[iVtxCount * iVtxSize];

	memcpy( pContainer->tVB.pVtxData, pVtxData, iVtxCount * iVtxSize );

	D3D11_BUFFER_DESC	tDesc = {};
	tDesc.ByteWidth = iVtxCount * iVtxSize;
	tDesc.Usage = eVtxUsage;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	if ( eVtxUsage == D3D11_USAGE_DYNAMIC )
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA	tData = {};
	tData.pSysMem = pVtxData;

	if ( FAILED( DEVICE->CreateBuffer( &tDesc, &tData, &pContainer->tVB.pBuffer ) ) )
		return false;

	char*	pVtx = ( char* )pVtxData;

	// 정점의 Min, Max 값을 구한다.
	for ( int i = 0; i < iVtxCount; ++i )
	{
		Vector3	vPos;
		memcpy( &vPos, pVtx + iVtxSize * i, sizeof( Vector3 ) );

		if ( vPos.x < m_vMin.x )
			m_vMin.x = vPos.x;

		if ( vPos.y < m_vMin.y )
			m_vMin.y = vPos.y;

		if ( vPos.z < m_vMin.z )
			m_vMin.z = vPos.z;

		if ( vPos.x > m_vMax.x )
			m_vMax.x = vPos.x;

		if ( vPos.y > m_vMax.y )
			m_vMax.y = vPos.y;

		if ( vPos.z > m_vMax.z )
			m_vMax.z = vPos.z;
	}

	return true;
}

bool CMesh::CreateIndexBuffer( UINT iIdxCount, UINT iIdxSize,
	D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt, void * pIdxData )
{
	if ( iIdxCount == 0 )
		return true;

	PMESHCONTAINER	pContainer = m_vecMeshContainer[m_vecMeshContainer.size() - 1];

	INDEXBUFFER	tIdxBuffer = {};

	tIdxBuffer.pBuffer = NULL;
	tIdxBuffer.iIdxCount = iIdxCount;
	tIdxBuffer.iIdxSize = iIdxSize;
	tIdxBuffer.eUsage = eIdxUsage;
	tIdxBuffer.eFmt = eFmt;
	tIdxBuffer.pIdxData = new char[iIdxCount * iIdxSize];

	memcpy( tIdxBuffer.pIdxData, pIdxData, iIdxCount * iIdxSize );

	D3D11_BUFFER_DESC	tDesc = {};
	tDesc.ByteWidth = iIdxCount * iIdxSize;
	tDesc.Usage = eIdxUsage;
	tDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	if ( eIdxUsage == D3D11_USAGE_DYNAMIC )
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA	tData = {};
	tData.pSysMem = pIdxData;

	if ( FAILED( DEVICE->CreateBuffer( &tDesc, &tData, &tIdxBuffer.pBuffer ) ) )
		return false;

	pContainer->vecIB.push_back( tIdxBuffer );

	return true;
}

void CMesh::Render( float fTime )
{
	for ( size_t i = 0; i < m_vecMeshContainer.size(); ++i )
	{
		UINT	iOffset = 0;
		UINT	iSize = m_vecMeshContainer[i]->tVB.iVtxSize;
		CONTEXT->IASetVertexBuffers( 0, 1, &m_vecMeshContainer[i]->tVB.pBuffer,
			&iSize, &iOffset );
		CONTEXT->IASetPrimitiveTopology( m_vecMeshContainer[i]->tVB.ePrimitive );

		if ( m_vecMeshContainer[i]->vecIB.empty() )
		{
			CONTEXT->Draw( m_vecMeshContainer[i]->tVB.iVtxCount, 0 );
		}

		else
		{
			for ( size_t j = 0; j < m_vecMeshContainer[i]->vecIB.size(); ++j )
			{
				CONTEXT->IASetIndexBuffer( m_vecMeshContainer[i]->vecIB[j].pBuffer,
					m_vecMeshContainer[i]->vecIB[j].eFmt, 0 );
				CONTEXT->DrawIndexed( m_vecMeshContainer[i]->vecIB[j].iIdxCount,
					0, 0 );
			}
		}
	}
}

void CMesh::Render( float fTime, int iContainer, int iSubset )
{
	UINT	iOffset = 0;
	UINT	iSize = m_vecMeshContainer[iContainer]->tVB.iVtxSize;
	CONTEXT->IASetVertexBuffers( 0, 1, &m_vecMeshContainer[iContainer]->tVB.pBuffer,
		&iSize, &iOffset );
	CONTEXT->IASetPrimitiveTopology( m_vecMeshContainer[iContainer]->tVB.ePrimitive );

	if ( m_vecMeshContainer[iContainer]->vecIB.empty() )
	{
		CONTEXT->Draw( m_vecMeshContainer[iContainer]->tVB.iVtxCount, 0 );
	}

	else
	{
		CONTEXT->IASetIndexBuffer( m_vecMeshContainer[iContainer]->vecIB[iSubset].pBuffer,
			m_vecMeshContainer[iContainer]->vecIB[iSubset].eFmt, 0 );
		CONTEXT->DrawIndexed( m_vecMeshContainer[iContainer]->vecIB[iSubset].iIdxCount,
			0, 0 );
	}
}

bool CMesh::CreateSphere( float radius, UINT numSubdivisions )
{
	// Put a cap on the number of subdivisions.
	numSubdivisions = min( numSubdivisions, 5u );

	// Approximate a sphere by tessellating an icosahedron.

	const float X = 0.525731f;
	const float Z = 0.850651f;

	XMFLOAT3 pos[12] =
	{
		XMFLOAT3( -X, 0.0f, Z ),  XMFLOAT3( X, 0.0f, Z ),
		XMFLOAT3( -X, 0.0f, -Z ), XMFLOAT3( X, 0.0f, -Z ),
		XMFLOAT3( 0.0f, Z, X ),   XMFLOAT3( 0.0f, Z, -X ),
		XMFLOAT3( 0.0f, -Z, X ),  XMFLOAT3( 0.0f, -Z, -X ),
		XMFLOAT3( Z, X, 0.0f ),   XMFLOAT3( -Z, X, 0.0f ),
		XMFLOAT3( Z, -X, 0.0f ),  XMFLOAT3( -Z, -X, 0.0f )
	};

	DWORD k[60] =
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};

	vector<XMFLOAT3>	vecPos;
	vector<UINT>	vecIndex;

	vecPos.resize( 12 );
	vecIndex.resize( 60 );

	for ( UINT i = 0; i < 12; ++i )
		vecPos[i] = pos[i];

	for ( UINT i = 0; i < 60; ++i )
		vecIndex[i] = k[i];

	for ( UINT i = 0; i < numSubdivisions; ++i )
		Subdivide( vecPos, vecIndex );

	// Project vertices onto sphere and scale.
	for ( UINT i = 0; i < vecPos.size(); ++i )
	{
		// Project onto unit sphere.
		XMVECTOR n = XMVector3Normalize( XMLoadFloat3( &vecPos[i] ) );

		// Project onto sphere.
		XMVECTOR p = radius * n;

		XMStoreFloat3( &vecPos[i], p );
		//XMStoreFloat3(&meshData.Vertices[i].Normal, n);

		// Derive texture coordinates from spherical coordinates.
		/*float theta = AngleFromXY(
			vecPos[i].x,
			vecPos[i].z);*/

			//float phi = acosf(vecPos[i].y / radius);

			//meshData.Vertices[i].TexC.x = theta / XM_2PI;
			//meshData.Vertices[i].TexC.y = phi / XM_PI;

			// Partial derivative of P with respect to theta
			//meshData.Vertices[i].TangentU.x = -radius*sinf(phi)*sinf(theta);
			//meshData.Vertices[i].TangentU.y = 0.0f;
			//meshData.Vertices[i].TangentU.z = +radius*sinf(phi)*cosf(theta);

			//XMVECTOR T = XMLoadFloat3(&meshData.Vertices[i].TangentU);
			//XMStoreFloat3(&meshData.Vertices[i].TangentU, XMVector3Normalize(T));
	}

	PMESHCONTAINER	pContainer = new MESHCONTAINER;

	m_vecMeshContainer.push_back( pContainer );

	if ( FAILED( CreateVertexBuffer( vecPos.size(), sizeof( XMFLOAT3 ), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &vecPos[0] ) ) )
		return false;

	if ( FAILED( CreateIndexBuffer( vecIndex.size(), 4, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT,
		&vecIndex[0] ) ) )
		return false;

	return true;
}

void CMesh::Subdivide( vector<XMFLOAT3>& vecPos, vector<UINT>& vecIndex )
{
	vector<XMFLOAT3>	vecCopyPos = vecPos;
	vector<UINT>	vecCopyIndex = vecIndex;

	vecPos.resize( 0 );
	vecIndex.resize( 0 );

	//       v1
	//       *
	//      / \
		//     /   \
	//  m0*-----*m1
//   / \   / \
	//  /   \ /   \
	// *-----*-----*
// v0    m2     v2

	UINT numTris = vecCopyIndex.size() / 3;
	for ( UINT i = 0; i < numTris; ++i )
	{
		XMFLOAT3 v0 = vecCopyPos[vecCopyIndex[i * 3 + 0]];
		XMFLOAT3 v1 = vecCopyPos[vecCopyIndex[i * 3 + 1]];
		XMFLOAT3 v2 = vecCopyPos[vecCopyIndex[i * 3 + 2]];

		//
		// Generate the midpoints.
		//

		XMFLOAT3 m0, m1, m2;

		// For subdivision, we just care about the position component.  We derive the other
		// vertex components in CreateGeosphere.

		m0 = XMFLOAT3(
			0.5f*( v0.x + v1.x ),
			0.5f*( v0.y + v1.y ),
			0.5f*( v0.z + v1.z ) );

		m1 = XMFLOAT3(
			0.5f*( v1.x + v2.x ),
			0.5f*( v1.y + v2.y ),
			0.5f*( v1.z + v2.z ) );

		m2 = XMFLOAT3(
			0.5f*( v0.x + v2.x ),
			0.5f*( v0.y + v2.y ),
			0.5f*( v0.z + v2.z ) );

		//
		// Add new geometry.
		//

		vecPos.push_back( v0 ); // 0
		vecPos.push_back( v1 ); // 1
		vecPos.push_back( v2 ); // 2
		vecPos.push_back( m0 ); // 3
		vecPos.push_back( m1 ); // 4
		vecPos.push_back( m2 ); // 5

		vecIndex.push_back( i * 6 + 0 );
		vecIndex.push_back( i * 6 + 3 );
		vecIndex.push_back( i * 6 + 5 );

		vecIndex.push_back( i * 6 + 3 );
		vecIndex.push_back( i * 6 + 4 );
		vecIndex.push_back( i * 6 + 5 );

		vecIndex.push_back( i * 6 + 5 );
		vecIndex.push_back( i * 6 + 4 );
		vecIndex.push_back( i * 6 + 2 );

		vecIndex.push_back( i * 6 + 3 );
		vecIndex.push_back( i * 6 + 1 );
		vecIndex.push_back( i * 6 + 4 );
	}
}

float CMesh::AngleFromXY( float x, float y )
{
	float theta = 0.0f;

	// Quadrant I or IV
	if ( x >= 0.0f )
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf( y / x ); // in [-pi/2, +pi/2]

		if ( theta < 0.0f )
			theta += 2.0f*PI; // in [0, 2*pi).
	}

	// Quadrant II or III
	else
		theta = atanf( y / x ) + PI; // in [0, 2*pi).

	return theta;
}

bool CMesh::ConvertFbx( CFbxLoader * pLoader )
{
	const vector<PFBXMESHCONTAINER>*	pvecContainer = pLoader->GetMeshContainer();
	const vector<vector<PFBXMATERIAL>>*	pvecMaterials = pLoader->GetMaterials();

	vector<PFBXMESHCONTAINER>::const_iterator	iter;
	vector<PFBXMESHCONTAINER>::const_iterator	iterEnd = pvecContainer->end();

	vector<bool>	vecEmptyIndex;

	for ( iter = pvecContainer->begin(); iter != iterEnd; ++iter )
	{
		PMESHCONTAINER	pContainer = new MESHCONTAINER;

		m_vecMeshContainer.push_back( pContainer );

		int	iVtxSize = 0;

		if ( ( *iter )->bBump )
		{
			if ( ( *iter )->bAnimation )
			{
				m_strShaderKey = STANDARD_BUMP_ANIM_SHADER;
				m_strInputLayoutKey = "BumpAnim";
				iVtxSize = sizeof( VERTEXBUMPANIM );

				vector<VERTEXBUMPANIM>	vecVtx;

				for ( size_t i = 0; i < ( *iter )->vecPos.size(); ++i )
				{
					VERTEXBUMPANIM	tVtx = {};

					tVtx.vPos = ( *iter )->vecPos[i];
					tVtx.vNormal = ( *iter )->vecNormal[i];
					tVtx.vUV = ( *iter )->vecUV[i];
					tVtx.vTangent = ( *iter )->vecTangent[i];
					tVtx.vBinormal = ( *iter )->vecBinormal[i];
					tVtx.vWeights = ( *iter )->vecBlendWeight[i];
					tVtx.vIndices = ( *iter )->vecBlendIndex[i];

					vecVtx.push_back( tVtx );
				}

				if ( !CreateVertexBuffer( vecVtx.size(),
					iVtxSize, D3D11_USAGE_DEFAULT,
					D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &vecVtx[0] ) )
					return false;
			}

			else
			{
				m_strShaderKey = STANDARD_BUMP_SHADER;
				m_strInputLayoutKey = "Bump";
				iVtxSize = sizeof( VERTEXBUMP );

				vector<VERTEXBUMP>	vecVtx;

				for ( size_t i = 0; i < ( *iter )->vecPos.size(); ++i )
				{
					VERTEXBUMP	tVtx = {};

					tVtx.vPos = ( *iter )->vecPos[i];
					tVtx.vNormal = ( *iter )->vecNormal[i];
					tVtx.vUV = ( *iter )->vecUV[i];
					tVtx.vTangent = ( *iter )->vecTangent[i];
					tVtx.vBinormal = ( *iter )->vecBinormal[i];

					vecVtx.push_back( tVtx );
				}

				if ( !CreateVertexBuffer( vecVtx.size(),
					iVtxSize, D3D11_USAGE_DEFAULT,
					D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &vecVtx[0] ) )
					return false;
			}
		}

		else
		{
			if ( ( *iter )->bAnimation )
			{
				m_strShaderKey = STANDARD_TEX_NORMAL_ANIM_SHADER;
				m_strInputLayoutKey = "VertexAnim";
				iVtxSize = sizeof( VERTEXANIM );

				vector<VERTEXANIM>	vecVtx;

				for ( size_t i = 0; i < ( *iter )->vecPos.size(); ++i )
				{
					VERTEXANIM	tVtx = {};

					tVtx.vPos = ( *iter )->vecPos[i];
					tVtx.vNormal = ( *iter )->vecNormal[i];
					tVtx.vUV = ( *iter )->vecUV[i];
					tVtx.vWeights = ( *iter )->vecBlendWeight[i];
					tVtx.vIndices = ( *iter )->vecBlendIndex[i];

					vecVtx.push_back( tVtx );
				}

				if ( !CreateVertexBuffer( vecVtx.size(),
					iVtxSize, D3D11_USAGE_DEFAULT,
					D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &vecVtx[0] ) )
					return false;
			}

			else
			{
				m_strShaderKey = STANDARD_TEX_NORMAL_SHADER;
				m_strInputLayoutKey = "VertexTexNormal";
				iVtxSize = sizeof( VERTEXTEXNORMAL );

				vector<VERTEXTEXNORMAL>	vecVtx;

				for ( size_t i = 0; i < ( *iter )->vecPos.size(); ++i )
				{
					VERTEXTEXNORMAL	tVtx = {};

					tVtx.vPos = ( *iter )->vecPos[i];
					tVtx.vNormal = ( *iter )->vecNormal[i];
					tVtx.vUV = ( *iter )->vecUV[i];

					vecVtx.push_back( tVtx );
				}

				if ( !CreateVertexBuffer( vecVtx.size(),
					iVtxSize, D3D11_USAGE_DEFAULT,
					D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &vecVtx[0] ) )
					return false;
			}
		}

		// 인덱스버퍼 생성
		for ( size_t i = 0; i < ( *iter )->vecIndices.size(); ++i )
		{
			if ( ( *iter )->vecIndices[i].empty() )
			{
				vecEmptyIndex.push_back( false );
				continue;
			}

			vecEmptyIndex.push_back( true );

			if ( !CreateIndexBuffer( ( *iter )->vecIndices[i].size(), 4,
				D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT,
				&( *iter )->vecIndices[i][0] ) )
				return false;
		}
	}

	// 재질 정보를 읽어온다.
	const vector<vector<PFBXMATERIAL>>*	pMaterials = pLoader->GetMaterials();

	vector<vector<PFBXMATERIAL>>::const_iterator	iterM;
	vector<vector<PFBXMATERIAL>>::const_iterator	iterMEnd = pMaterials->end();

	int	iContainer = 0;
	for ( iterM = pMaterials->begin(); iterM != iterMEnd; ++iterM, ++iContainer )
	{
		PMESHCONTAINER	pContainer = m_vecMeshContainer[iContainer];

		for ( size_t i = 0; i < ( *iterM ).size(); ++i )
		{
			if ( !vecEmptyIndex[i] )
				continue;

			PFBXMATERIAL	pMtrl = ( *iterM )[i];

			CMaterial*	pMaterial = new CMaterial;

			if ( !pMaterial->Init() )
			{
				SAFE_RELEASE( pMaterial );
				return NULL;
			}


			pMaterial->SetMaterial( pMtrl->vDif, pMtrl->vAmb,
				pMtrl->vSpc, pMtrl->vEmv, pMtrl->fShininess );

			// 이름을 불러온다.
			char	strName[MAX_PATH] = {};
			_splitpath_s( pMtrl->strDifTex.c_str(), NULL, 0, NULL, 0,
				strName, MAX_PATH, NULL, 0 );

			wchar_t	strPath[MAX_PATH] = {};

			MultiByteToWideChar( CP_ACP, 0, pMtrl->strDifTex.c_str(),
				-1, strPath, pMtrl->strDifTex.length() );

			pMaterial->SetDiffuseTexInfoFromFullPath( "Linear", strName, 0, 0,
				pMtrl->strDifTex.c_str() );

			if ( !pMtrl->strBumpTex.empty() )
			{
				memset( strName, 0, MAX_PATH );
				_splitpath_s( pMtrl->strBumpTex.c_str(), NULL, 0, NULL, 0,
					strName, MAX_PATH, NULL, 0 );

				memset( strPath, 0, sizeof( wchar_t ) * MAX_PATH );

				MultiByteToWideChar( CP_ACP, 0, pMtrl->strBumpTex.c_str(),
					-1, strPath, pMtrl->strBumpTex.length() );

				pMaterial->SetNormalTexInfoFromFullPath( "Linear", strName, 1, 1,
					pMtrl->strBumpTex.c_str() );
			}

			if ( !pMtrl->strSpcTex.empty() )
			{
				memset( strName, 0, MAX_PATH );
				_splitpath_s( pMtrl->strSpcTex.c_str(), NULL, 0, NULL, 0,
					strName, MAX_PATH, NULL, 0 );

				memset( strPath, 0, sizeof( wchar_t ) * MAX_PATH );

				MultiByteToWideChar( CP_ACP, 0, pMtrl->strSpcTex.c_str(),
					-1, strPath, pMtrl->strSpcTex.length() );

				pMaterial->SetSpecularTexInfoFromFullPath( "Linear", strName, 2, 2,
					pMtrl->strSpcTex.c_str() );
			}

			pContainer->vecMaterial.push_back( pMaterial );
		}
	}

	// 텍스쳐가 저장된 폴더명을 키로 변경한다.
	char	strFullName[MAX_PATH] = {};
	iterM = pMaterials->begin();
	strcpy_s( strFullName, ( *iterM )[0]->strDifTex.c_str() );

	int	iLength = strlen( strFullName );
	for ( int i = iLength - 1; i >= 0; --i )
	{
		if ( strFullName[i] == '\\' || strFullName[i] == '/' )
		{
			memset( strFullName + ( i + 1 ), 0, sizeof( char ) * ( iLength - ( i + 1 ) ) );
			strFullName[i] = '\\';
			//strFullName[i] = 0;
			break;
		}
	}

	char	strChange[MAX_PATH] = {};
	strcpy_s( strChange, strFullName );
	iLength = strlen( strChange );
	for ( int i = iLength - 2; i >= 0; --i )
	{
		if ( strChange[i] == '\\' || strChange[i] == '/' )
		{
			memset( strChange + ( i + 1 ), 0, sizeof( char ) * ( iLength - ( i + 1 ) ) );
			break;
		}
	}

	strcat_s( strChange, m_strKey.c_str() );
	strcat_s( strChange, "\\" );

	MoveFileA( strFullName, strChange );

	// Mesh\\ 까지의 경로를 제거한다.
	iLength = strlen( strChange );
	for ( int i = iLength - 2; i >= 0; --i )
	{
		char	cText[5] = {};
		memcpy( cText, &strChange[i - 4], 4 );
		_strupr_s( cText );

		if ( strcmp( cText, "MESH" ) == 0 )
		{
			memset( strChange, 0, sizeof( char ) * ( i + 1 ) );
			memcpy( strChange, &strChange[i + 1], sizeof( char ) * ( iLength - ( i + 1 ) ) );
			memset( strChange + ( i + 1 ), 0, sizeof( char ) * ( iLength - ( i + 1 ) ) );
			break;
		}
	}

	for ( size_t i = 0; i < m_vecMeshContainer.size(); ++i )
	{
		PMESHCONTAINER	pContainer = m_vecMeshContainer[i];

		for ( size_t j = 0; j < pContainer->vecMaterial.size(); ++j )
		{
			pContainer->vecMaterial[j]->SetTexturePathKey( MESH_PATH );
			pContainer->vecMaterial[j]->ChangeTexturePath( strChange );
		}
	}

	m_vLength = m_vMax - m_vMin;

	m_vCenter = ( m_vMax + m_vMin ) / 2.f;
	m_fRadius = m_vLength.Length() / 2.f;

	// 애니메이션 처리
	const vector<PFBXBONE>*	pvecBone = pLoader->GetBones();

	if ( pvecBone->empty() )
		return true;

	SAFE_RELEASE( m_pAnimation );

	m_pAnimation = new CAnimation;

	if ( !m_pAnimation->Init() )
	{
		SAFE_RELEASE( m_pAnimation );
		return false;
	}

	//// 본 수만큼 반복한다.
	vector<PFBXBONE>::const_iterator	iterB;
	vector<PFBXBONE>::const_iterator	iterBEnd = pvecBone->end();

	for ( iterB = pvecBone->begin(); iterB != iterBEnd; ++iterB )
	{
		PBONE	pBone = new BONE;

		pBone->strName = ( *iterB )->strName;
		pBone->iDepth = ( *iterB )->iDepth;
		pBone->iParentIndex = ( *iterB )->iParentIndex;

		float	fMat[4][4];

		for ( int i = 0; i < 4; ++i )
		{
			for ( int j = 0; j < 4; ++j )
			{
				fMat[i][j] = ( *iterB )->matOffset.mData[i].mData[j];
			}
		}

		pBone->matOffset = new Matrix;
		*pBone->matOffset = fMat;

		for ( int i = 0; i < 4; ++i )
		{
			for ( int j = 0; j < 4; ++j )
			{
				fMat[i][j] = ( *iterB )->matBone.mData[i].mData[j];
			}
		}

		pBone->matBone = new Matrix;
		*pBone->matBone = fMat;

		m_pAnimation->AddBone( pBone );
	}

	m_pAnimation->CreateBoneTexture();

	// 애니메이션 클립을 추가한다.
	const vector<PFBXANIMATIONCLIP>* pvecClip = pLoader->GetClips();

	// 클립을 읽어온다.
	vector<PFBXANIMATIONCLIP>::const_iterator	iterC;
	vector<PFBXANIMATIONCLIP>::const_iterator	iterCEnd = pvecClip->end();

	for ( iterC = pvecClip->begin(); iterC != iterCEnd; ++iterC )
	{
		m_pAnimation->AddClip( AO_LOOP, *iterC );
	}

	return true;
}

bool CMesh::Save( const wchar_t * pFileName, const string & strPathKey )
{
	const wchar_t* pPath = GET_SINGLE( CPathManager )->FindPath( strPathKey );
	wstring	strPath;

	if ( pPath )
		strPath = pPath;

	strPath += pFileName;

	return SaveFromFullPath( strPath.c_str() );
}

bool CMesh::Save( const char * pFileName, const string & strPathKey )
{
	const char*	pPath = GET_SINGLE( CPathManager )->FindPathToMultiByte( strPathKey );

	string	strPath;
	if ( pPath )
		strPath = pPath;
	strPath += pFileName;

	return SaveFromFullPath( strPath.c_str() );
}

bool CMesh::SaveFromFullPath( const wchar_t * pFullPath )
{
	char	strPath[MAX_PATH] = {};
	WideCharToMultiByte( CP_ACP, 0, pFullPath, -1,
		strPath, lstrlen( pFullPath ), 0, 0 );
	return SaveFromFullPath( strPath );
}

bool CMesh::SaveFromFullPath( const char * pFullPath )
{
	FILE*	pFile = NULL;

	fopen_s( &pFile, pFullPath, "wb" );

	if ( !pFile )
		return false;

	size_t	iSize = m_vecMeshContainer.size();

	// 컨테이너 수 저장
	fwrite( &iSize, sizeof( size_t ), 1, pFile );

	for ( size_t i = 0; i < iSize; ++i )
	{
		// 버텍스버퍼 저장
		fwrite( &m_vecMeshContainer[i]->tVB.iVtxCount,
			sizeof( UINT ), 1, pFile );
		fwrite( &m_vecMeshContainer[i]->tVB.iVtxSize,
			sizeof( UINT ), 1, pFile );
		fwrite( &m_vecMeshContainer[i]->tVB.eUsage,
			sizeof( D3D11_USAGE ), 1, pFile );
		fwrite( &m_vecMeshContainer[i]->tVB.ePrimitive,
			sizeof( D3D11_PRIMITIVE_TOPOLOGY ), 1, pFile );
		fwrite( m_vecMeshContainer[i]->tVB.pVtxData,
			m_vecMeshContainer[i]->tVB.iVtxSize,
			m_vecMeshContainer[i]->tVB.iVtxCount, pFile );

		size_t	iSubset = m_vecMeshContainer[i]->vecIB.size();
		fwrite( &iSubset, sizeof( size_t ), 1, pFile );

		for ( size_t j = 0; j < iSubset; ++j )
		{
			// 인덱스버퍼 저장
			fwrite( &m_vecMeshContainer[i]->vecIB[j].iIdxCount,
				sizeof( UINT ), 1, pFile );
			fwrite( &m_vecMeshContainer[i]->vecIB[j].iIdxSize,
				sizeof( UINT ), 1, pFile );
			fwrite( &m_vecMeshContainer[i]->vecIB[j].eUsage,
				sizeof( D3D11_USAGE ), 1, pFile );
			fwrite( &m_vecMeshContainer[i]->vecIB[j].eFmt,
				sizeof( DXGI_FORMAT ), 1, pFile );
			fwrite( m_vecMeshContainer[i]->vecIB[j].pIdxData,
				m_vecMeshContainer[i]->vecIB[j].iIdxSize,
				m_vecMeshContainer[i]->vecIB[j].iIdxCount, pFile );
		}

		// 재질 정보 저장
		for ( size_t j = 0; j < iSubset; ++j )
		{
			m_vecMeshContainer[i]->vecMaterial[j]->Save( pFile );
		}
	}

	int	iLength = m_strKey.length();
	fwrite( &iLength, sizeof( int ), 1, pFile );
	fwrite( m_strKey.c_str(), sizeof( char ), iLength, pFile );

	fwrite( &m_vLength, sizeof( Vector3 ), 1, pFile );
	fwrite( &m_vMin, sizeof( Vector3 ), 1, pFile );
	fwrite( &m_vMax, sizeof( Vector3 ), 1, pFile );
	fwrite( &m_vCenter, sizeof( Vector3 ), 1, pFile );
	fwrite( &m_fRadius, sizeof( float ), 1, pFile );

	iLength = m_strShaderKey.length();
	fwrite( &iLength, sizeof( int ), 1, pFile );
	fwrite( m_strShaderKey.c_str(), sizeof( char ), iLength, pFile );

	iLength = m_strInputLayoutKey.length();
	fwrite( &iLength, sizeof( int ), 1, pFile );
	fwrite( m_strInputLayoutKey.c_str(), sizeof( char ), iLength, pFile );

	fclose( pFile );

	return true;
}

bool CMesh::Load( const wchar_t * pFileName, const string & strPathKey )
{
	const wchar_t* pPath = GET_SINGLE( CPathManager )->FindPath( strPathKey );
	wstring	strPath;

	if ( pPath )
		strPath = pPath;

	strPath += pFileName;

	return LoadFromFullPath( strPath.c_str() );
}

bool CMesh::Load( const char * pFileName, const string & strPathKey )
{
	const char*	pPath = GET_SINGLE( CPathManager )->FindPathToMultiByte( strPathKey );

	string	strPath;
	if ( pPath )
		strPath = pPath;
	strPath += pFileName;

	return LoadFromFullPath( strPath.c_str() );
}

bool CMesh::LoadFromFullPath( const wchar_t * pFullPath )
{
	char	strPath[MAX_PATH] = {};
	WideCharToMultiByte( CP_ACP, 0, pFullPath, -1,
		strPath, lstrlen( pFullPath ), 0, 0 );
	return LoadFromFullPath( strPath );
}

bool CMesh::LoadFromFullPath( const char * pFullPath )
{
	FILE*	pFile = NULL;

	fopen_s( &pFile, pFullPath, "rb" );

	if ( !pFile )
		return false;

	size_t	iContainerCount = 0;

	// 컨테이너 수 저장
	fread( &iContainerCount, sizeof( size_t ), 1, pFile );

	for ( size_t i = 0; i < iContainerCount; ++i )
	{
		PMESHCONTAINER	pContainer = new MESHCONTAINER;

		m_vecMeshContainer.push_back( pContainer );

		// 버텍스버퍼 저장
		int	iCount, iSize;
		D3D11_USAGE	eUsage;
		D3D11_PRIMITIVE_TOPOLOGY	ePrimitive;
		fread( &iCount, sizeof( UINT ), 1, pFile );
		fread( &iSize, sizeof( UINT ), 1, pFile );
		fread( &eUsage, sizeof( D3D11_USAGE ), 1, pFile );
		fread( &ePrimitive, sizeof( D3D11_PRIMITIVE_TOPOLOGY ), 1, pFile );

		void* pVtxData = new char[iCount * iSize];

		fread( pVtxData, iSize, iCount, pFile );

		CreateVertexBuffer( iCount, iSize, eUsage, ePrimitive, pVtxData );
		SAFE_DELETE_ARRAY( pVtxData );

		size_t	iSubset = 0;
		fread( &iSubset, sizeof( size_t ), 1, pFile );

		for ( size_t j = 0; j < iSubset; ++j )
		{
			INDEXBUFFER	tIdxBuf = {};

			// 인덱스버퍼 저장
			fread( &tIdxBuf.iIdxCount,
				sizeof( UINT ), 1, pFile );
			fread( &tIdxBuf.iIdxSize,
				sizeof( UINT ), 1, pFile );
			fread( &tIdxBuf.eUsage,
				sizeof( D3D11_USAGE ), 1, pFile );
			fread( &tIdxBuf.eFmt,
				sizeof( DXGI_FORMAT ), 1, pFile );

			tIdxBuf.pIdxData = new char[tIdxBuf.iIdxCount * tIdxBuf.iIdxSize];

			fread( tIdxBuf.pIdxData,
				tIdxBuf.iIdxSize,
				tIdxBuf.iIdxCount, pFile );

			CreateIndexBuffer( tIdxBuf.iIdxCount, tIdxBuf.iIdxSize, tIdxBuf.eUsage,
				tIdxBuf.eFmt, tIdxBuf.pIdxData );

			SAFE_DELETE_ARRAY( tIdxBuf.pIdxData );
		}

		// 재질 정보 저장
		for ( size_t j = 0; j < iSubset; ++j )
		{
			CMaterial*	pMaterial = new CMaterial;

			pMaterial->Load( pFile );

			pContainer->vecMaterial.push_back( pMaterial );
		}
	}

	size_t	iLength = 0;
	char	strName[256] = {};

	fread( &iLength, sizeof( int ), 1, pFile );
	fread( strName, sizeof( char ), iLength, pFile );
	m_strKey = strName;

	fread( &m_vLength, sizeof( Vector3 ), 1, pFile );
	fread( &m_vMin, sizeof( Vector3 ), 1, pFile );
	fread( &m_vMax, sizeof( Vector3 ), 1, pFile );
	fread( &m_vCenter, sizeof( Vector3 ), 1, pFile );
	fread( &m_fRadius, sizeof( float ), 1, pFile );

	memset( strName, 0, sizeof( char ) * 256 );
	fread( &iLength, sizeof( int ), 1, pFile );
	fread( strName, sizeof( char ), iLength, pFile );
	m_strShaderKey = strName;

	memset( strName, 0, sizeof( char ) * 256 );
	fread( &iLength, sizeof( int ), 1, pFile );
	fread( strName, sizeof( char ), iLength, pFile );
	m_strInputLayoutKey = strName;

	fclose( pFile );

	return true;
}
