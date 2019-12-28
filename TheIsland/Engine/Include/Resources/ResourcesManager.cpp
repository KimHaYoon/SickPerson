#include "ResourcesManager.h"
#include "Mesh.h"
#include "Texture.h"
#include "Sampler.h"
#include "../Core/PathManager.h"

GAME_USING

DEFINITION_SINGLE( CResourcesManager );

CResourcesManager::CResourcesManager()
{
}

CResourcesManager::~CResourcesManager()
{
	Safe_Release_Map( m_mapSampler );
	Safe_Release_Map( m_mapTexture );
	Safe_Release_Map( m_mapMesh );
}

bool CResourcesManager::Init()
{
	Vector3	vPos[5] =
	{
		Vector3( 0.f, 0.5f, 0.f ),
		Vector3( -0.5f, -0.5f, 0.5f ),
		Vector3( 0.5f, -0.5f, 0.5f ),
		Vector3( -0.5f, -0.5f, -0.5f ),
		Vector3( 0.5f, -0.5f,-0.5f )
	};
	Vector4	vColor[5] =
	{
		Vector4::Red,
		Vector4::Green,
		Vector4::Blue,
		Vector4::Yellow,
		Vector4::Magenta
	};
	Vector3	vFaceNormal[4];

	Vector3	vEdge1 = vPos[4] - vPos[0];
	Vector3	vEdge2 = vPos[3] - vPos[0];
	vFaceNormal[0] = vEdge1.Cross( vEdge2 ).Normalize();

	vEdge1 = vPos[3] - vPos[0];
	vEdge2 = vPos[1] - vPos[0];
	vFaceNormal[1] = vEdge1.Cross( vEdge2 ).Normalize();

	vEdge1 = vPos[1] - vPos[0];
	vEdge2 = vPos[2] - vPos[0];
	vFaceNormal[2] = vEdge1.Cross( vEdge2 ).Normalize();

	vEdge1 = vPos[2] - vPos[0];
	vEdge2 = vPos[4] - vPos[0];
	vFaceNormal[3] = vEdge1.Cross( vEdge2 ).Normalize();

	Vector3	vNormal[4];
	vNormal[0] = vFaceNormal[1] + vFaceNormal[2];
	vNormal[0] = vNormal[0].Normalize();

	vNormal[1] = vFaceNormal[2] + vFaceNormal[3];
	vNormal[1] = vNormal[1].Normalize();

	vNormal[2] = vFaceNormal[0] + vFaceNormal[1];
	vNormal[2] = vNormal[2].Normalize();

	vNormal[3] = vFaceNormal[0] + vFaceNormal[3];
	vNormal[3] = vNormal[3].Normalize();


	VERTEXCOLORNORMAL	tPyramid[9] =
	{
		VERTEXCOLORNORMAL( vPos[0], Vector3( 0.f, 1.f, 0.f ), vColor[0] ),
		VERTEXCOLORNORMAL( vPos[1], vNormal[0], vColor[1] ),
		VERTEXCOLORNORMAL( vPos[2], vNormal[1], vColor[2] ),
		VERTEXCOLORNORMAL( vPos[3], vNormal[2], vColor[3] ),
		VERTEXCOLORNORMAL( vPos[4], vNormal[3], vColor[4] ),
		VERTEXCOLORNORMAL( vPos[1], Vector3( 0.f, -1.f, 0.f ), vColor[1] ),
		VERTEXCOLORNORMAL( vPos[2], Vector3( 0.f, -1.f, 0.f ), vColor[2] ),
		VERTEXCOLORNORMAL( vPos[3], Vector3( 0.f, -1.f, 0.f ), vColor[3] ),
		VERTEXCOLORNORMAL( vPos[4], Vector3( 0.f, -1.f, 0.f ), vColor[4] )
	};

	UINT	iIndex[18] = { 0, 4, 3, 0, 3, 1, 0, 2, 4, 0, 1, 2, 7, 8, 6, 7, 6, 5 };

	CMesh*	pMesh = CreateMesh( "Pyramid", 9, sizeof( VERTEXCOLORNORMAL ), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, tPyramid, 18, 4,
		D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT, iIndex );

	SAFE_RELEASE( pMesh );

	VERTEXCOLOR	tAABB[8] =
	{
		VERTEXCOLOR( -0.5f, 0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f ),
		VERTEXCOLOR( 0.5f, 0.5f, -0.5f, 0.f, 1.f, 0.f, 1.f ),
		VERTEXCOLOR( -0.5f, -0.5f, -0.5f, 0.f, 0.f, 1.f, 1.f ),
		VERTEXCOLOR( 0.5f, -0.5f, -0.5f, 1.f, 1.f, 0.f, 1.f ),
		VERTEXCOLOR( -0.5f, 0.5f, 0.5f, 1.f, 0.f, 1.f, 1.f ),
		VERTEXCOLOR( 0.5f, 0.5f, 0.5f, 0.f, 1.f, 1.f, 1.f ),
		VERTEXCOLOR( -0.5f, -0.5f, 0.5f, 1.f, 0x69 / 255.f, 0xb4 / 255.f, 1.f ),
		VERTEXCOLOR( 0.5f, -0.5f, 0.5f, 1.f, 0xd7 / 255.f, 0.f, 1.f )
	};

	UINT	iAABBIndex[24] = { 0, 1, 4, 5, 4, 0, 5, 1, 0, 2, 1, 3, 5, 7, 4, 6, 6, 7, 2, 3,
					6, 2, 7, 3 };

	pMesh = CreateMesh( "AABB", 8, sizeof( VERTEXCOLOR ), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_LINELIST, tAABB, 24, 4,
		D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT, iAABBIndex );

	SAFE_RELEASE( pMesh );

	VERTEXTEX	tVtxRectLTTex[4] =
	{
		VERTEXTEX( 0.f, 0.f, 0, 0, 0 ),
		VERTEXTEX( 1.f, 0.f, 0, 1, 0 ),
		VERTEXTEX( 0.f, 1.f, 0, 0, 1 ),
		VERTEXTEX( 1.f, 1.f, 0, 1, 1 )
	};

	UINT	tIdxRectCol[6] = { 0, 1, 3, 0, 3, 2 };

	pMesh = CreateMesh( "RectOrthoLTTex", 4, sizeof( VERTEXTEX ),
		D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		tVtxRectLTTex, 6, 4, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT,
		tIdxRectCol );

	SAFE_RELEASE( pMesh );

	VERTEXCOLOR	tVtxRectCenterLine[5] =
	{
		VERTEXCOLOR( -0.5f, -0.5f, 0, 0, 1, 0, 1 ),
		VERTEXCOLOR( 0.5f, -0.5f, 0, 0, 1, 0, 1 ),
		VERTEXCOLOR( 0.5f, 0.5f, 0, 0, 1, 0, 1 ),
		VERTEXCOLOR( -0.5f, 0.5f, 0, 0, 1, 0, 1 ),
		VERTEXCOLOR( -0.5f, -0.5f, 0, 0, 1, 0, 1 )
	};

	pMesh = CreateMesh( "RectOrthoCenterLine", 5, sizeof( VERTEXCOLOR ),
		D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
		tVtxRectCenterLine );

	SAFE_RELEASE( pMesh );

	Vector3	tCube[8] =
	{
		Vector3( -0.5f, 0.5f, -0.5f ),
		Vector3( 0.5f, 0.5f, -0.5f ),
		Vector3( -0.5f, -0.5f, -0.5f ),
		Vector3( 0.5f, -0.5f, -0.5f ),
		Vector3( -0.5f, 0.5f, 0.5f ),
		Vector3( 0.5f, 0.5f, 0.5f ),
		Vector3( -0.5f, -0.5f, 0.5f ),
		Vector3( 0.5f, -0.5f, 0.5f )
	};

	UINT	tCubeIdx[36] = { 0, 1, 3, 0, 3, 2, 1, 5, 7, 1, 7, 3,
		4, 0, 2, 4, 2, 6, 5, 4, 6, 5, 6, 7, 4, 5, 1, 4, 1, 0,
		2, 3, 7, 2, 7, 6 };

	pMesh = CreateMesh( "Cube", 8, sizeof( Vector3 ),
		D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		tCube, 36, 4, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT,
		tCubeIdx );

	SAFE_RELEASE( pMesh );

	// 구 메쉬를 생성한다.
	pMesh = CreateSphere( "Sphere", 1.f, 10 );

	SAFE_RELEASE( pMesh );

	Vector3	vEffectPos;

	pMesh = CreateMesh( "ParticleMesh", 1, sizeof( Vector3 ),
		D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
		&vEffectPos );

	SAFE_RELEASE( pMesh );

	CSampler*	pSampler = CreateSampler( SAMPLER_LINEAR );

	SAFE_RELEASE( pSampler );

	pSampler = CreateSampler( SAMPLER_POINT, D3D11_FILTER_MIN_MAG_MIP_POINT );

	SAFE_RELEASE( pSampler );

	return true;
}

CMesh * CResourcesManager::CreateMesh( const string & strKey, UINT iVtxCount,
	UINT iVtxSize, D3D11_USAGE eVtxUsage,
	D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void * pVtxData, UINT iIdxCount,
	UINT iIdxSize, D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt, void * pIdxData )
{
	CMesh*	pMesh = FindMesh( strKey );

	if ( pMesh )
		return pMesh;

	pMesh = new CMesh;
	pMesh->m_strKey = strKey;

	if ( !pMesh->CreateMesh( iVtxCount, iVtxSize, eVtxUsage, ePrimitive, pVtxData,
		iIdxCount, iIdxSize, eIdxUsage, eFmt, pIdxData ) )
	{
		SAFE_RELEASE( pMesh );
		return NULL;
	}

	pMesh->SetKey( strKey );

	pMesh->AddRef();

	m_mapMesh.insert( make_pair( strKey, pMesh ) );

	return pMesh;
}

CMesh * CResourcesManager::CreateSphere( const string & strKey, float radius, UINT numSubdivisions )
{
	CMesh*	pMesh = FindMesh( strKey );

	if ( pMesh )
		return pMesh;

	pMesh = new CMesh;
	pMesh->m_strKey = strKey;

	if ( !pMesh->CreateSphere( radius, numSubdivisions ) )
	{
		SAFE_RELEASE( pMesh );
		return NULL;
	}

	pMesh->SetKey( strKey );

	pMesh->AddRef();

	m_mapMesh.insert( make_pair( strKey, pMesh ) );

	return pMesh;
}

CMesh * CResourcesManager::LoadMesh( const string & strKey,
	const wchar_t * pFileName, const string & strPathKey )
{
	const wchar_t* pPath = GET_SINGLE( CPathManager )->FindPath( strPathKey );
	wstring	strFullPath;

	if ( pPath )
		strFullPath = pPath;

	strFullPath += pFileName;

	return LoadMeshFromFullPath( strKey, strFullPath.c_str() );
}

CMesh * CResourcesManager::LoadMesh( const string & strKey,
	const char * pFileName, const string & strPathKey )
{
	const char* pPath = GET_SINGLE( CPathManager )->FindPathToMultiByte( strPathKey );
	string	strFullPath;

	if ( pPath )
		strFullPath = pPath;

	strFullPath += pFileName;

	return LoadMeshFromFullPath( strKey, strFullPath.c_str() );
}

CMesh * CResourcesManager::LoadMeshFromFullPath( const string & strKey,
	const wchar_t * pFullPath )
{
	char	strPath[MAX_PATH] = {};

	WideCharToMultiByte( CP_ACP, 0, pFullPath, -1, strPath,
		lstrlen( pFullPath ) * 2, 0, 0 );

	return LoadMeshFromFullPath( strKey, strPath );
}

CMesh * CResourcesManager::LoadMeshFromFullPath( const string & strKey,
	const char * pFullPath )
{
	CMesh*	pMesh = FindMesh( strKey );

	if ( pMesh )
		return pMesh;

	pMesh = new CMesh;
	pMesh->m_strKey = strKey;

	if ( !pMesh->LoadMesh( strKey, pFullPath ) )
	{
		SAFE_RELEASE( pMesh );
		return NULL;
	}

	pMesh->AddRef();

	m_mapMesh.insert( make_pair( strKey, pMesh ) );

	return pMesh;
}

CMesh * CResourcesManager::FindMesh( const string & strKey )
{
	unordered_map<string, class CMesh*>::iterator	iter = m_mapMesh.find( strKey );

	if ( iter == m_mapMesh.end() )
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

CTexture * CResourcesManager::LoadTexture( const string & strKey,
	const wchar_t * pFileName, const string & strPathKey )
{
	CTexture*	pTexture = FindTexture( strKey );

	if ( pTexture )
		return pTexture;

	pTexture = new CTexture;

	if ( !pTexture->LoadTexture( strKey, pFileName, strPathKey ) )
	{
		SAFE_RELEASE( pTexture );
		return NULL;
	}

	pTexture->AddRef();

	m_mapTexture.insert( make_pair( strKey, pTexture ) );

	return pTexture;
}

CTexture * CResourcesManager::LoadTexture( const string & strKey,
	const char * pFileName, const string & strPathKey )
{
	CTexture*	pTexture = FindTexture( strKey );

	if ( pTexture )
		return pTexture;

	pTexture = new CTexture;

	if ( !pTexture->LoadTexture( strKey, pFileName, strPathKey ) )
	{
		SAFE_RELEASE( pTexture );
		return NULL;
	}

	pTexture->AddRef();

	m_mapTexture.insert( make_pair( strKey, pTexture ) );

	return pTexture;
}

class CTexture* CResourcesManager::LoadTexture( const string & strKey,
	const vector<wstring>& vecFileName, const string & strPathKey )
{
	CTexture*	pTexture = FindTexture( strKey );

	if ( pTexture )
		return pTexture;

	pTexture = new CTexture;

	if ( !pTexture->LoadTexture( strKey, vecFileName, strPathKey ) )
	{
		SAFE_RELEASE( pTexture );
		return NULL;
	}

	pTexture->AddRef();

	m_mapTexture.insert( make_pair( strKey, pTexture ) );

	return pTexture;
}

CTexture * CResourcesManager::LoadTextureFromFullPath( const string & strKey,
	const char * pFullPath )
{
	CTexture*	pTexture = FindTexture( strKey );

	if ( pTexture )
		return pTexture;

	pTexture = new CTexture;

	if ( !pTexture->LoadTextureFromFullPath( strKey, pFullPath ) )
	{
		SAFE_RELEASE( pTexture );
		return NULL;
	}

	pTexture->AddRef();

	m_mapTexture.insert( make_pair( strKey, pTexture ) );

	return pTexture;
}

CTexture * CResourcesManager::LoadTextureFromFullPath(
	const string & strKey,
	const vector<string>& vecFullPath )
{
	CTexture*	pTexture = FindTexture( strKey );

	if ( pTexture )
		return pTexture;

	pTexture = new CTexture;

	if ( !pTexture->LoadTextureFromFullPath( strKey, vecFullPath ) )
	{
		SAFE_RELEASE( pTexture );
		return NULL;
	}

	pTexture->AddRef();

	m_mapTexture.insert( make_pair( strKey, pTexture ) );

	return pTexture;
}

CTexture * CResourcesManager::FindTexture( const string & strKey )
{
	unordered_map<string, CTexture*>::iterator	iter = m_mapTexture.find( strKey );

	if ( iter == m_mapTexture.end() )
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

CSampler * CResourcesManager::CreateSampler( const string & strKey,
	D3D11_FILTER eFilter,
	D3D11_TEXTURE_ADDRESS_MODE eAddrU,
	D3D11_TEXTURE_ADDRESS_MODE eAddrV,
	D3D11_TEXTURE_ADDRESS_MODE eAddrW )
{
	CSampler*	pSampler = FindSampler( strKey );

	if ( pSampler )
		return pSampler;

	pSampler = new CSampler;

	if ( !pSampler->CreateSampler( strKey, eFilter, eAddrU, eAddrV, eAddrW ) )
	{
		SAFE_RELEASE( pSampler );
		return NULL;
	}

	pSampler->AddRef();

	m_mapSampler.insert( make_pair( strKey, pSampler ) );

	return pSampler;
}

CSampler * CResourcesManager::FindSampler( const string & strKey )
{
	unordered_map<string, CSampler*>::iterator	iter = m_mapSampler.find( strKey );

	if ( iter == m_mapSampler.end() )
		return NULL;

	iter->second->AddRef();

	return iter->second;
}
