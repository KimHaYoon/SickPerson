#include "SceneManager.h"
#include "Scene.h"

GAME_USING

DEFINITION_SINGLE( CSceneManager )

CSceneManager::CSceneManager() :
	m_pCurScene( NULL ),
	m_pNextScene( NULL )
{
}

CSceneManager::~CSceneManager()
{
	SAFE_RELEASE( m_pNextScene );
	SAFE_RELEASE( m_pCurScene );
}

CScene * CSceneManager::CreateScene( const string & strTag )
{
	CScene*	pScene = new CScene;

	if ( !pScene->Init() )
	{
		SAFE_RELEASE( pScene );
		return NULL;
	}

	pScene->AddRef();

	return pScene;
}

CScene * CSceneManager::GetCurrentScene() const
{
	m_pCurScene->AddRef();
	return m_pCurScene;
}

CScene * CSceneManager::CreateNextScene( const string & strTag )
{
	m_pNextScene = CreateScene( strTag );

	return m_pNextScene;
}

bool CSceneManager::Init()
{
	m_pCurScene = CreateScene( "DefaultScene" );
	m_pCurScene->Release();
	
	return true;
}

void CSceneManager::Input( float fTime )
{
	m_pCurScene->Input( fTime );
}

SCENE_CHANGE CSceneManager::Update( float fTime )
{
	m_pCurScene->Update( fTime );

	return ChangeScene();
}

SCENE_CHANGE CSceneManager::LateUpdate( float fTime )
{
	m_pCurScene->LateUpdate( fTime );

	return ChangeScene();
}

void CSceneManager::Collision( float fTime )
{
	m_pCurScene->Collision( fTime );
}

void CSceneManager::Render( float fTime )
{
	m_pCurScene->Render( fTime );
}

SCENE_CHANGE CSceneManager::ChangeScene()
{
	if ( m_pNextScene )
	{
		// 기존 장면을 지운다.
		SAFE_RELEASE( m_pCurScene );

		m_pCurScene = m_pNextScene;
		m_pNextScene = NULL;
		
		return SC_CHANGE;
	}

	return SC_NONE;
}
