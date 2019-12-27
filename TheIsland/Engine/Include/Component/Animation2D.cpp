#include "Animation2D.h"
#include "../Resources/Texture.h"
#include "../Resources/ResourcesManager.h"
#include "../GameObject/GameObject.h"
#include "Material.h"
#include "Renderer2D.h"
#include "../Rendering/ShaderManager.h"

GAME_USING

CAnimation2D::CAnimation2D() :
	m_pCurClip( NULL ),
	m_bClipEnd( false )
{
	SetTag( "Animation2D" );
	SetTypeName( "CAnimation2D" );
	SetTypeID<CAnimation2D>();
	m_eType = CT_ANIMATION2D;
}

CAnimation2D::CAnimation2D( const CAnimation2D & anim ) :
	CComponent( anim )
{
	unordered_map<string, PANIMATIONCLIP2D>::const_iterator	iter;
	unordered_map<string, PANIMATIONCLIP2D>::const_iterator	iterEnd = anim.m_mapClip.end();

	for ( iter = anim.m_mapClip.begin(); iter != iterEnd; ++iter )
	{
		PANIMATIONCLIP2D	pClip = new ANIMATIONCLIP2D;

		*pClip = *iter->second;
		pClip->pTexture->AddRef();

		m_mapClip.insert( make_pair( iter->first, pClip ) );
	}

	//m_strCurrentAnim = anim.m_strCurrentAnim;
	m_strDefaultAnim = anim.m_strDefaultAnim;

	m_pCurClip = NULL;
	//m_pCurClip = FindClip(m_strCurrentAnim);
	ChangeAnimation( anim.m_strCurrentAnim );

	m_bClipEnd = false;
}

CAnimation2D::~CAnimation2D()
{
	unordered_map<string, PANIMATIONCLIP2D>::iterator	iter;
	unordered_map<string, PANIMATIONCLIP2D>::iterator	iterEnd = m_mapClip.end();

	for ( iter = m_mapClip.begin(); iter != iterEnd; ++iter )
	{
		SAFE_RELEASE( iter->second->pTexture );
	}

	Safe_Delete_Map( m_mapClip );
}

bool CAnimation2D::GetClipEnd() const
{
	return m_bClipEnd;
}

bool CAnimation2D::CreateClip( const string & strKey,
	ANIMATION2D_TYPE eType, ANIMATION2D_OPTION eOption,
	int iFrameMaxX, int iFrameMaxY, int iLengthX, int iLengthY, int iStartY,
	float fAnimLimitTime,
	int iCountLimit, float fOptionTimeLimit, const string & strTexKey,
	TCHAR * pFileName, const string & strPathKey )
{
	if ( FindClip( strKey ) )
		return false;

	PANIMATIONCLIP2D	pClip = new ANIMATIONCLIP2D;

	pClip->eType = eType;
	pClip->eOption = eOption;
	pClip->iFrameMaxX = iFrameMaxX;
	pClip->iFrameMaxY = iFrameMaxY;
	pClip->fAnimLimitTime = fAnimLimitTime;
	pClip->iCountLimit = iCountLimit;
	pClip->fOptionTimeLimit = fOptionTimeLimit;
	pClip->iLengthX = iLengthX;
	pClip->iLengthY = iLengthY;
	pClip->iStartY = iStartY;
	pClip->iFrameY = iStartY;

	if ( pFileName )
		pClip->pTexture = GET_SINGLE( CResourcesManager )->LoadTexture( strTexKey, pFileName, strPathKey );

	else
		pClip->pTexture = GET_SINGLE( CResourcesManager )->FindTexture( strTexKey );

	m_mapClip.insert( make_pair( strKey, pClip ) );

	if ( m_strCurrentAnim.empty() )
	{
		m_pCurClip = pClip;
		ChangeAnimation( strKey );
		m_strDefaultAnim = strKey;
	}

	return true;
}

bool CAnimation2D::CreateClip( const string & strKey, ANIMATION2D_TYPE eType,
	ANIMATION2D_OPTION eOption, int iFrameMaxX, int iFrameMaxY, int iLengthX,
	int iLengthY, int iStartY, float fAnimLimitTime, int iCountLimit,
	float fOptionTimeLimit, const string & strTexKey,
	const vector<wstring>* vecFileName, const string & strPathKey )
{
	if ( FindClip( strKey ) )
		return false;

	PANIMATIONCLIP2D	pClip = new ANIMATIONCLIP2D;

	pClip->eType = eType;
	pClip->eOption = eOption;
	pClip->iFrameMaxX = iFrameMaxX;
	pClip->iFrameMaxY = iFrameMaxY;
	pClip->fAnimLimitTime = fAnimLimitTime;
	pClip->iCountLimit = iCountLimit;
	pClip->fOptionTimeLimit = fOptionTimeLimit;
	pClip->iLengthX = iLengthX;
	pClip->iLengthY = iLengthY;
	pClip->iStartY = iStartY;
	pClip->iFrameY = iStartY;

	if ( vecFileName )
		pClip->pTexture = GET_SINGLE( CResourcesManager )->LoadTexture( strTexKey, *vecFileName, strPathKey );

	else
		pClip->pTexture = GET_SINGLE( CResourcesManager )->FindTexture( strTexKey );

	m_mapClip.insert( make_pair( strKey, pClip ) );

	if ( m_strCurrentAnim.empty() )
	{
		m_pCurClip = pClip;
		ChangeAnimation( strKey );
		m_strDefaultAnim = strKey;
	}

	return true;
}

bool CAnimation2D::AddClip( const string & strKey, PANIMATIONCLIP2D pClip )
{
	if ( FindClip( strKey ) )
		return false;

	m_mapClip.insert( make_pair( strKey, pClip ) );

	if ( m_strCurrentAnim.empty() )
	{
		m_pCurClip = pClip;
		ChangeAnimation( strKey );
		m_strDefaultAnim = strKey;
	}

	return true;
}

bool CAnimation2D::ChangeAnimation( const string & strKey )
{
	if ( m_strCurrentAnim == strKey )
		return false;

	PANIMATIONCLIP2D	pClip = FindClip( strKey );

	if ( m_pCurClip )
	{
		m_pCurClip->iFrameX = 0;
		m_pCurClip->iFrameY = m_pCurClip->iStartY;
		m_pCurClip->iCount = 0;
		m_pCurClip->fAnimTime = 0.f;
		m_pCurClip->fOptionTime = 0.f;
	}

	m_pCurClip = pClip;

	m_pCurClip->iFrameY = m_pCurClip->iStartY;
	m_strCurrentAnim = strKey;

	CRenderer2D*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer2D>();

	CMaterial*	pMaterial = pRenderer->GetMaterial();

	pMaterial->SetDiffuseTex( m_pCurClip->pTexture );

	SAFE_RELEASE( pMaterial );

	SAFE_RELEASE( pRenderer );

	return true;
}

void CAnimation2D::SetDefaultAnim( const string & strKey )
{
	m_strDefaultAnim = strKey;
}

void CAnimation2D::ReturnDefaultAnimation()
{
	ChangeAnimation( m_strDefaultAnim );
}

void CAnimation2D::SetShader()
{
	GET_SINGLE( CShaderManager )->UpdateCBuffer( "Animation2D", &m_tCBuffer, SCT_VERTEX | SCT_PIXEL );
}

PANIMATIONCLIP2D CAnimation2D::FindClip( const string & strKey )
{
	unordered_map<string, PANIMATIONCLIP2D>::iterator	iter = m_mapClip.find( strKey );

	if ( iter == m_mapClip.end() )
		return NULL;

	return iter->second;
}

bool CAnimation2D::Init()
{
	return true;
}

void CAnimation2D::Input( float fTime )
{
}

int CAnimation2D::Update( float fTime )
{
	m_bClipEnd = false;

	if ( !m_pCurClip )
		return 0;

	m_pCurClip->fAnimTime += fTime;

	int	iMaxFrame = m_pCurClip->iLengthX * m_pCurClip->iLengthY;
	float fLimitTime = m_pCurClip->fAnimLimitTime / iMaxFrame;

	if ( m_pCurClip->fAnimTime >= fLimitTime )
	{
		m_pCurClip->fAnimTime -= fLimitTime;
		++m_pCurClip->iFrameX;

		if ( m_pCurClip->iFrameX >= m_pCurClip->iLengthX )
		{
			m_pCurClip->iFrameX = 0;
			++m_pCurClip->iFrameY;

			if ( m_pCurClip->iFrameY >= m_pCurClip->iStartY + m_pCurClip->iLengthY )
			{
				m_pCurClip->iFrameY = m_pCurClip->iStartY;
				m_bClipEnd = true;

				switch ( m_pCurClip->eOption )
				{
				case A2DO_ONCE_RETURN:
					ChangeAnimation( m_strDefaultAnim );
					break;
				case A2DO_ONCE_DESTROY:
					m_pGameObject->Die();
					break;
				case A2DO_COUNT_RETURN:
					break;
				case A2DO_COUNT_DESTROY:
					break;
				case A2DO_TIME_RETURN:
					break;
				case A2DO_TIME_DESTROY:
					break;
				}
			}
		}
	}

	// 상수버퍼 갱신
	m_tCBuffer.iType = m_pCurClip->eType;
	m_tCBuffer.iFrameX = m_pCurClip->iFrameX;
	m_tCBuffer.iFrameY = m_pCurClip->iFrameY;
	m_tCBuffer.iLengthX = m_pCurClip->iFrameMaxX;
	m_tCBuffer.iLengthY = m_pCurClip->iFrameMaxY;

	CRenderer2D*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer2D>();

	pRenderer->UpdateCBuffer( "Animation2D", 10, sizeof( ANIMATION2DBUFFER ),
		SCT_VERTEX | SCT_PIXEL, &m_tCBuffer );

	SAFE_RELEASE( pRenderer );

	return 0;
}

int CAnimation2D::LateUpdate( float fTime )
{
	return 0;
}

void CAnimation2D::Collision( float fTime )
{
}

void CAnimation2D::Render( float fTime )
{
}

CAnimation2D * CAnimation2D::Clone()
{
	return new CAnimation2D( *this );
}

void CAnimation2D::Save( FILE * pFile )
{
	CComponent::Save( pFile );

	int	iLength = m_strCurrentAnim.length();

	fwrite( &iLength, 4, 1, pFile );
	fwrite( m_strCurrentAnim.c_str(), 1, iLength, pFile );

	iLength = m_strDefaultAnim.length();

	fwrite( &iLength, 4, 1, pFile );
	fwrite( m_strDefaultAnim.c_str(), 1, iLength, pFile );

	iLength = m_mapClip.size();

	fwrite( &iLength, 4, 1, pFile );

	unordered_map<string, PANIMATIONCLIP2D>::iterator	iter;
	unordered_map<string, PANIMATIONCLIP2D>::iterator	iterEnd = m_mapClip.end();

	for ( iter = m_mapClip.begin(); iter != iterEnd; ++iter )
	{
		// 키를 저장한다.
		iLength = iter->first.length();

		fwrite( &iLength, 4, 1, pFile );
		fwrite( iter->first.c_str(), 1, iLength, pFile );

		// 클립정보를 저장한다.
		fwrite( &iter->second->eType, 4, 1, pFile );
		fwrite( &iter->second->eOption, 4, 1, pFile );
		fwrite( &iter->second->iFrameX, 4, 1, pFile );
		fwrite( &iter->second->iFrameY, 4, 1, pFile );
		fwrite( &iter->second->iFrameMaxX, 4, 1, pFile );
		fwrite( &iter->second->iFrameMaxY, 4, 1, pFile );
		fwrite( &iter->second->iLengthX, 4, 1, pFile );
		fwrite( &iter->second->iLengthY, 4, 1, pFile );
		fwrite( &iter->second->iStartY, 4, 1, pFile );
		fwrite( &iter->second->fAnimTime, 4, 1, pFile );
		fwrite( &iter->second->fAnimLimitTime, 4, 1, pFile );
		fwrite( &iter->second->iCount, 4, 1, pFile );
		fwrite( &iter->second->iCountLimit, 4, 1, pFile );
		fwrite( &iter->second->fOptionTime, 4, 1, pFile );
		fwrite( &iter->second->fOptionTimeLimit, 4, 1, pFile );

		iter->second->pTexture->Save( pFile );
	}
}

void CAnimation2D::Load( FILE * pFile )
{
	CComponent::Load( pFile );

	int	iLength;
	char	strKey[256] = {};

	fread( &iLength, 4, 1, pFile );
	fread( strKey, 1, iLength, pFile );
	m_strCurrentAnim = strKey;

	fread( &iLength, 4, 1, pFile );
	fread( strKey, 1, iLength, pFile );
	m_strDefaultAnim = strKey;

	int	iCount;
	fread( &iCount, 4, 1, pFile );

	for ( int i = 0; i < iCount; ++i )
	{
		fread( &iLength, 4, 1, pFile );
		fread( strKey, 1, iLength, pFile );

		PANIMATIONCLIP2D	pClip = new ANIMATIONCLIP2D;

		// 클립정보를 저장한다.
		fread( &pClip->eType, 4, 1, pFile );
		fread( &pClip->eOption, 4, 1, pFile );
		fread( &pClip->iFrameX, 4, 1, pFile );
		fread( &pClip->iFrameY, 4, 1, pFile );
		fread( &pClip->iFrameMaxX, 4, 1, pFile );
		fread( &pClip->iFrameMaxY, 4, 1, pFile );
		fread( &pClip->iLengthX, 4, 1, pFile );
		fread( &pClip->iLengthY, 4, 1, pFile );
		fread( &pClip->iStartY, 4, 1, pFile );
		fread( &pClip->fAnimTime, 4, 1, pFile );
		fread( &pClip->fAnimLimitTime, 4, 1, pFile );
		fread( &pClip->iCount, 4, 1, pFile );
		fread( &pClip->iCountLimit, 4, 1, pFile );
		fread( &pClip->fOptionTime, 4, 1, pFile );
		fread( &pClip->fOptionTimeLimit, 4, 1, pFile );

		char	strTexKey[256] = {};
		fread( &iLength, 4, 1, pFile );
		fread( strTexKey, 1, iLength, pFile );

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
			pClip->pTexture = GET_SINGLE( CResourcesManager )->LoadTextureFromFullPath( strTexKey, vecFullPath[0].c_str() );

		else
			pClip->pTexture = GET_SINGLE( CResourcesManager )->LoadTextureFromFullPath( strTexKey, vecFullPath );

		m_mapClip.insert( make_pair( strKey, pClip ) );
	}
}
