#include "Mesh.h"
#include "../Device.h"

GAME_USING


CMesh::CMesh() :
	m_pVB( NULL ),
	m_pIB( NULL )
{
	SetTag( "Mesh" );
	SetTypeName( "CMesh" );
	SetTypeID<CMesh>();
}

CMesh::~CMesh()
{
	if ( m_pVB )
		SAFE_RELEASE( m_pVB->pBuffer );

	if ( m_pIB )
		SAFE_RELEASE( m_pIB->pBuffer );

	SAFE_DELETE( m_pVB );
	SAFE_DELETE( m_pIB );
}

Vector3 CMesh::GetLength() const
{
	return m_vLength;
}

string CMesh::GetKey() const
{
	return m_strKey;
}

void CMesh::SetKey( const string & strKey )
{
	m_strKey = strKey;
}

bool CMesh::CreateMesh( UINT iVtxCount, UINT iVtxSize, D3D11_USAGE eVtxUsage,
	D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void * pVtxData, UINT iIdxCount,
	UINT iIdxSize, D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt, void * pIdxData )
{
	if ( !CreateVertexBuffer( iVtxCount, iVtxSize, eVtxUsage, ePrimitive,
		pVtxData ) )
		return false;

	if ( !CreateIndexBuffer( iIdxCount, iIdxSize, eIdxUsage, eFmt,
		pIdxData ) )
		return false;

	m_vLength = m_vMax - m_vMin;

	return true;
}

bool CMesh::CreateVertexBuffer( UINT iVtxCount, UINT iVtxSize,
	D3D11_USAGE eVtxUsage, D3D11_PRIMITIVE_TOPOLOGY ePrimitive,
	void * pVtxData )
{
	if ( m_pVB )
		SAFE_RELEASE( m_pVB->pBuffer );

	SAFE_DELETE( m_pVB );

	m_pVB = new VERTEXBUFFER;

	m_pVB->iVtxCount = iVtxCount;
	m_pVB->iVtxSize = iVtxSize;
	m_pVB->eUsage = eVtxUsage;
	m_pVB->ePrimitive = ePrimitive;

	D3D11_BUFFER_DESC	tDesc = {};
	tDesc.ByteWidth = iVtxCount * iVtxSize;
	tDesc.Usage = eVtxUsage;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	if ( eVtxUsage == D3D11_USAGE_DYNAMIC )
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA	tData = {};
	tData.pSysMem = pVtxData;

	if ( FAILED( DEVICE->CreateBuffer( &tDesc, &tData, &m_pVB->pBuffer ) ) )
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
	if ( !pIdxData )
		return true;

	if ( m_pIB )
		SAFE_RELEASE( m_pIB->pBuffer );

	SAFE_DELETE( m_pIB );

	m_pIB = new INDEXBUFFER;

	m_pIB->iIdxCount = iIdxCount;
	m_pIB->iIdxSize = iIdxSize;
	m_pIB->eUsage = eIdxUsage;
	m_pIB->eFmt = eFmt;

	D3D11_BUFFER_DESC	tDesc = {};
	tDesc.ByteWidth = iIdxCount * iIdxSize;
	tDesc.Usage = eIdxUsage;
	tDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	if ( eIdxUsage == D3D11_USAGE_DYNAMIC )
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA	tData = {};
	tData.pSysMem = pIdxData;

	if ( FAILED( DEVICE->CreateBuffer( &tDesc, &tData, &m_pIB->pBuffer ) ) )
		return false;

	return true;
}

void CMesh::Render( float fTime )
{
	// 도형 위상구조를 설정한다.
	CONTEXT->IASetPrimitiveTopology( m_pVB->ePrimitive );
	// 버텍스버퍼를 지정한다.
	UINT	iStride = m_pVB->iVtxSize;
	UINT	iOffset = 0;
	CONTEXT->IASetVertexBuffers( 0, 1, &m_pVB->pBuffer, &iStride, &iOffset );

	if ( m_pIB )
	{
		// 인덱스버퍼를 지정한다.
		CONTEXT->IASetIndexBuffer( m_pIB->pBuffer, m_pIB->eFmt, 0 );
		CONTEXT->DrawIndexed( m_pIB->iIdxCount, 0, 0 );
	}

	else
	{
		CONTEXT->Draw( m_pVB->iVtxCount, 0 );
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
	}

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
			theta += 2.0f * PI; // in [0, 2*pi).
	}

	// Quadrant II or III
	else
		theta = atanf( y / x ) + PI; // in [0, 2*pi).

	return theta;
}
