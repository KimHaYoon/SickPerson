#include "ResourcesManager.h"
#include "Mesh.h"
#include "Texture.h"
#include "Sampler.h"

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
	VERTEXCOLOR	tPyramid[5] =
	{
		VERTEXCOLOR( 0.f, 0.5f, 0.f, 1.f, 0.f, 0.f, 1.f ),
		VERTEXCOLOR( -0.5f, -0.5f, 0.5f, 0.f, 1.f, 0.f, 1.f ),
		VERTEXCOLOR( 0.5f, -0.5f, 0.5f, 0.f, 0.f, 1.f, 1.f ),
		VERTEXCOLOR( -0.5f, -0.5f, -0.5f, 1.f, 1.f, 0.f, 1.f ),
		VERTEXCOLOR( 0.5f, -0.5f, -0.5f, 1.f, 0.f, 1.f, 1.f )
	};

	UINT	iIndex[18] = { 0, 4, 3, 0, 3, 1, 0, 2, 4, 0, 1, 2, 3, 4, 2, 3, 2, 1 };

	CMesh*	pMesh = CreateMesh( "Pyramid", 5, sizeof( VERTEXCOLOR ), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, tPyramid, 18, 4,
		D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT, iIndex );

	SAFE_RELEASE( pMesh );

	// AABB Mesh
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

	// Sphere Mesh
	pMesh = CreateSphere( "Sphere", 1.f, 10 );

	SAFE_RELEASE( pMesh );

	CSampler*	pSampler = CreateSampler( SAMPLER_LINEAR );

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

CMesh * CResourcesManager::CreateSphere( const string & strKey, float radius, UINT numSubdivisions )
{
	CMesh*	pMesh = FindMesh( strKey );

	if ( pMesh )
		return pMesh;

	pMesh = new CMesh;

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