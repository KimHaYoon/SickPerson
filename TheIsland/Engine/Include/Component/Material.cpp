#include "Material.h"
#include "../Resources/Texture.h"
#include "../Resources/ResourcesManager.h"
#include "../Resources/Sampler.h"

GAME_USING

CMaterial::CMaterial()
{
	SetTag( "Material" );
	SetTypeName( "CMaterial" );
	SetTypeID<CMaterial>();
	m_eType = CT_MATERIAL;
}

CMaterial::CMaterial( const CMaterial & material ) :
	CComponent( material )
{
	m_tDifInfo = material.m_tDifInfo;

	if ( m_tDifInfo.pTexture )
		m_tDifInfo.pTexture->AddRef();

	if ( m_tDifInfo.pSampler )
		m_tDifInfo.pSampler->AddRef();
}

CMaterial::~CMaterial()
{
	SAFE_RELEASE( m_tDifInfo.pTexture );
	SAFE_RELEASE( m_tDifInfo.pSampler );
}

void CMaterial::SetDiffuseTexInfo( const string & strSmpKey,
	const string & strDifKey, int iRegTex, int iRegSmp,
	const TCHAR * pFileName,
	const string & strPathKey )
{
	SAFE_RELEASE( m_tDifInfo.pTexture );
	SAFE_RELEASE( m_tDifInfo.pSampler );

	if ( !pFileName )
		m_tDifInfo.pTexture = GET_SINGLE( CResourcesManager )->FindTexture( strDifKey );

	else
		m_tDifInfo.pTexture = GET_SINGLE( CResourcesManager )->LoadTexture( strDifKey, pFileName, strPathKey );

	m_tDifInfo.pSampler = GET_SINGLE( CResourcesManager )->FindSampler( strSmpKey );

	m_tDifInfo.iRegisterTex = iRegTex;
	m_tDifInfo.iRegisterSmp = iRegSmp;
}

void CMaterial::SetDiffuseTexInfo( const string & strSmpKey,
	const string & strDifKey, int iRegTex, int iRegSmp,
	const vector<wstring>* vecFileName, const string & strPathKey )
{
	SAFE_RELEASE( m_tDifInfo.pTexture );
	SAFE_RELEASE( m_tDifInfo.pSampler );

	if ( !vecFileName )
		m_tDifInfo.pTexture = GET_SINGLE( CResourcesManager )->FindTexture( strDifKey );

	else
		m_tDifInfo.pTexture = GET_SINGLE( CResourcesManager )->LoadTexture( strDifKey, *vecFileName, strPathKey );

	m_tDifInfo.pSampler = GET_SINGLE( CResourcesManager )->FindSampler( strSmpKey );

	m_tDifInfo.iRegisterTex = iRegTex;
	m_tDifInfo.iRegisterSmp = iRegSmp;
}

void CMaterial::SetDiffuseTex( CTexture * pTex )
{
	SAFE_RELEASE( m_tDifInfo.pTexture );

	pTex->AddRef();
	m_tDifInfo.pTexture = pTex;
}

void CMaterial::SetDiffuseTex( const string & strKey )
{
	SAFE_RELEASE( m_tDifInfo.pTexture );

	CTexture* pTex = GET_SINGLE( CResourcesManager )->FindTexture( strKey );

	m_tDifInfo.pTexture = pTex;
}

void CMaterial::SetDiffuseTex( const string & strKey, const char * pFullPath )
{
	SAFE_RELEASE( m_tDifInfo.pTexture );

	CTexture* pTex = GET_SINGLE( CResourcesManager )->LoadTextureFromFullPath( strKey, pFullPath );

	m_tDifInfo.pTexture = pTex;
}

void CMaterial::SetDiffuseTex( const string & strKey, const vector<string>& vecFullPath )
{
	SAFE_RELEASE( m_tDifInfo.pTexture );

	CTexture* pTex = GET_SINGLE( CResourcesManager )->LoadTextureFromFullPath( strKey, vecFullPath );

	m_tDifInfo.pTexture = pTex;
}

void CMaterial::SetMaterial( int iShaderConstantType )
{
	if ( m_tDifInfo.pTexture )
	{
		m_tDifInfo.pTexture->SetTexture( m_tDifInfo.iRegisterTex, iShaderConstantType );
		m_tDifInfo.pSampler->SetSampler( m_tDifInfo.iRegisterSmp, iShaderConstantType );
	}
}

bool CMaterial::Init()
{
	return true;
}

void CMaterial::Input( float fTime )
{
}

int CMaterial::Update( float fTime )
{
	return 0;
}

int CMaterial::LateUpdate( float fTime )
{
	return 0;
}

void CMaterial::Collision( float fTime )
{
}

void CMaterial::Render( float fTime )
{
}

CMaterial * CMaterial::Clone()
{
	return new CMaterial( *this );
}

void CMaterial::Save( FILE * pFile )
{
	CComponent::Save( pFile );

	m_tDifInfo.pTexture->Save( pFile );
	fwrite( &m_tDifInfo.iRegisterTex, 4, 1, pFile );

	string	strKey = m_tDifInfo.pSampler->GetKey();
	int	iLength = strKey.length();
	fwrite( &iLength, 4, 1, pFile );
	fwrite( strKey.c_str(), 1, iLength, pFile );

	fwrite( &m_tDifInfo.iRegisterSmp, 4, 1, pFile );
}

void CMaterial::Load( FILE * pFile )
{
	CComponent::Load( pFile );

	int	iLength = 0;
	char	strKey[MAX_PATH] = {};
	fread( &iLength, 4, 1, pFile );
	fread( strKey, 1, iLength, pFile );

	vector<string>	vecFullPath;

	int	iCount;
	fread( &iCount, 4, 1, pFile );

	for ( size_t i = 0; i < iCount; ++i )
	{
		char	strPath[MAX_PATH] = {};
		fread( &iLength, 4, 1, pFile );
		fread( strPath, 1, iLength, pFile );

		vecFullPath.push_back( strPath );
	}

	if ( vecFullPath.size() == 1 )
		SetDiffuseTex( strKey, vecFullPath[0].c_str() );

	else
		SetDiffuseTex( strKey, vecFullPath );

	fread( &m_tDifInfo.iRegisterTex, 4, 1, pFile );

	fread( &iLength, 4, 1, pFile );
	fread( strKey, 1, iLength, pFile );
	strKey[iLength] = 0;
	fread( &m_tDifInfo.iRegisterSmp, 4, 1, pFile );
	m_tDifInfo.pSampler = GET_SINGLE( CResourcesManager )->FindSampler( strKey );
}
