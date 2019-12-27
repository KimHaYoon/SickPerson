#include "Scene.h"
#include "Layer.h"
#include "SceneScript.h"
#include "../Device.h"
#include "../GameObject/GameObject.h"
#include "../Component/Camera.h"
#include "../Component/Transform.h"
#include "../Component/Light.h"
#include "../Component/LightDir.h"
#include "../Component/LightPoint.h"
#include "../Component/LightSpot.h"
#include "../Component/Renderer.h"
#include "../Component/Material.h"

GAME_USING

CScene::CScene() :
	m_pMainCameraTr( NULL ),
	m_pMainCameraObj( NULL ),
	m_pMainCamera( NULL ),
	m_pSkyObject( NULL )
{
	SetTag( "Scene" );
	SetTypeName( "CScene" );
	SetTypeID<CScene>();
	m_vecLayer.reserve( 10 );
}

CScene::~CScene()
{
	CGameObject::EraseObj( m_pSkyObject );
	SAFE_RELEASE( m_pSkyObject );

	CGameObject::EraseObj( m_pMainCameraObj );
	SAFE_RELEASE( m_pMainCameraTr );
	SAFE_RELEASE( m_pMainCamera );
	SAFE_RELEASE( m_pMainCameraObj );
	Safe_Release_Map( m_mapCamera );

	CGameObject::ErasePrototype( this );
	Safe_Release_VecList( m_LightList );
	Safe_Release_VecList( m_vecSceneScript );
	Safe_Release_VecList( m_vecLayer );
}

CGameObject * CScene::CreatePrototype( const string & strKey )
{
	return CGameObject::CreatePrototype( strKey, this );
}

CGameObject * CScene::CreateCamera( const string & strKey,
	const Vector3 & vPos, float fViewAngle, float fAspect,
	float fNear, float fFar )
{
	CGameObject*	pCameraObj = FindCamera( strKey );

	if ( pCameraObj )
		return pCameraObj;

	pCameraObj = CGameObject::CreateObject( strKey );

	CTransform*	pTransform = pCameraObj->GetTransform();

	pTransform->SetWorldPos( vPos );

	SAFE_RELEASE( pTransform );

	CCamera* pCamera = pCameraObj->AddComponent<CCamera>( "Camera" );

	pCamera->SetPerspectiveProj( fViewAngle, fAspect, fNear, fFar );

	SAFE_RELEASE( pCamera );

	pCameraObj->AddRef();
	m_mapCamera.insert( make_pair( strKey, pCameraObj ) );

	return pCameraObj;
}

CGameObject * CScene::CreateOrthoCamera( const string & strKey,
	const Vector3 & vPos, const RESOLUTION & tRS, float fNear, float fFar )
{
	CGameObject*	pCameraObj = FindCamera( strKey );

	if ( pCameraObj )
		return pCameraObj;

	pCameraObj = CGameObject::CreateObject( strKey );

	CTransform*	pTransform = pCameraObj->GetTransform();

	pTransform->SetWorldPos( vPos );

	SAFE_RELEASE( pTransform );

	CCamera* pCamera = pCameraObj->AddComponent<CCamera>( "Camera" );

	pCamera->SetOrthoProj( tRS, fNear, fFar );

	SAFE_RELEASE( pCamera );

	pCameraObj->AddRef();
	m_mapCamera.insert( make_pair( strKey, pCameraObj ) );

	return pCameraObj;
}

CGameObject * CScene::FindCamera( const string & strKey )
{
	unordered_map<string, CGameObject*>::iterator	iter = m_mapCamera.find( strKey );

	if ( iter == m_mapCamera.end() )
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

CGameObject * CScene::GetMainCameraObj() const
{
	m_pMainCameraObj->AddRef();
	return m_pMainCameraObj;
}

CTransform * CScene::GetMainCameraTr() const
{
	m_pMainCameraTr->AddRef();
	return m_pMainCameraTr;
}

CCamera * CScene::GetMainCamera() const
{
	m_pMainCamera->AddRef();
	return m_pMainCamera;
}

CLight * CScene::CreateLight( const string & strTag, LIGHT_TYPE eType )
{
	CGameObject*	pLightObj = new CGameObject;

	pLightObj->SetTag( strTag );

	if ( !pLightObj->Init() )
	{
		SAFE_RELEASE( pLightObj );
		return NULL;
	}

	CLight*	pLight = NULL;

	switch ( eType )
	{
	case LT_DIR:
		pLight = pLightObj->AddComponent<CLightDir>( strTag );
		break;
	case LT_POINT:
		pLight = pLightObj->AddComponent<CLightPoint>( strTag );
		break;
	case LT_SPOT:
		pLight = pLightObj->AddComponent<CLightSpot>( strTag );
		break;
	}

	m_LightList.push_back( pLightObj );

	return pLight;
}

CLight * CScene::GetGlobalLight( const string & strTag )
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_LightList.end();

	for ( iter = m_LightList.begin(); iter != iterEnd; ++iter )
	{
		if ( ( *iter )->GetTag() == strTag )
		{
			CLight*	pLight = ( *iter )->FindComponentFromType<CLight>( CT_LIGHT );
			return pLight;
		}
	}

	return NULL;
}

bool CScene::Init()
{
	CLayer*	pLayer = CreateLayer( "Default" );

	SAFE_RELEASE( pLayer );

	pLayer = CreateLayer( "UI", INT_MAX );

	SAFE_RELEASE( pLayer );

	// 메인 카메라 생성
	m_pMainCameraObj = CreateCamera( "MainCamera",
		Vector3( 0.f, 0.f, -5.f ), PI / 3.f,
		DEVICE_RESOLUTION.iWidth / ( float )DEVICE_RESOLUTION.iHeight, 0.03f, 5000.f );
	m_pMainCamera = m_pMainCameraObj->FindComponentFromTypeID<CCamera>();
	m_pMainCameraTr = m_pMainCameraObj->GetTransform();


	// 전역 조명
	CLight*	pGlobalLight = CreateLight( "GlobalLight", LT_SPOT );
	CTransform*	pLightTr = pGlobalLight->GetTransform();

	pLightTr->SetWorldPos( 0.f, -1.f, 0.f );
	pLightTr->SetWorldRot( PI / 2.f, 0.f, 0.f );

	SAFE_RELEASE( pLightTr );
	SAFE_RELEASE( pGlobalLight );

	// SkyBox
	m_pSkyObject = CGameObject::CreateObject( "Sky" );

	m_pSkyObject->SetScene( this );

	CTransform*	pSkyTr = m_pSkyObject->GetTransform();

	pSkyTr->SetWorldScale( 50000.f, 50000.f, 50000.f );

	SAFE_RELEASE( pSkyTr );

	CRenderer*	pRenderer = m_pSkyObject->AddComponent<CRenderer>( "SkyRenderer" );

	pRenderer->SetMesh( "Sphere" );
	pRenderer->SetShader( SKY_SHADER );
	pRenderer->SetInputLayout( "VertexPos" );

	CMaterial*	pMaterial = pRenderer->CreateMaterial();

	pMaterial->SetDiffuseTexInfo( SAMPLER_LINEAR, "Sky", 0, 0, L"Sky3.dds" );

	SAFE_RELEASE( pMaterial );

	pRenderer->SetRenderState( CULLING_NONE );
	pRenderer->SetRenderState( DEPTH_LESS_EQUAL );

	SAFE_RELEASE( pRenderer );

	return true;
}

void CScene::Input( float fTime )
{
	vector<CSceneScript*>::iterator	iter1;
	vector<CSceneScript*>::iterator	iter1End = m_vecSceneScript.end();

	for ( iter1 = m_vecSceneScript.begin(); iter1 != iter1End;)
	{
		if ( !( *iter1 )->GetEnable() )
		{
			++iter1;
			continue;
		}

		( *iter1 )->Input( fTime );

		if ( !( *iter1 )->GetAlive() )
		{
			SAFE_RELEASE( ( *iter1 ) );
			iter1 = m_vecSceneScript.erase( iter1 );
		}

		else
			++iter1;
	}

	vector<CLayer*>::iterator	iter;
	vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

	for ( iter = m_vecLayer.begin(); iter != iterEnd;)
	{
		if ( !( *iter )->GetEnable() )
		{
			++iter;
			continue;
		}

		( *iter )->Input( fTime );

		if ( !( *iter )->GetAlive() )
		{
			SAFE_RELEASE( ( *iter ) );
			iter = m_vecLayer.erase( iter );
		}

		else
			++iter;
	}

	m_pMainCameraObj->Input( fTime );
}

int CScene::Update( float fTime )
{
	vector<CSceneScript*>::iterator	iter1;
	vector<CSceneScript*>::iterator	iter1End = m_vecSceneScript.end();

	for ( iter1 = m_vecSceneScript.begin(); iter1 != iter1End;)
	{
		if ( !( *iter1 )->GetEnable() )
		{
			++iter1;
			continue;
		}

		( *iter1 )->Update( fTime );

		if ( !( *iter1 )->GetAlive() )
		{
			SAFE_RELEASE( ( *iter1 ) );
			iter1 = m_vecSceneScript.erase( iter1 );
		}

		else
			++iter1;
	}

	vector<CLayer*>::iterator	iter;
	vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

	for ( iter = m_vecLayer.begin(); iter != iterEnd;)
	{
		if ( !( *iter )->GetEnable() )
		{
			++iter;
			continue;
		}

		( *iter )->Update( fTime );

		if ( !( *iter )->GetAlive() )
		{
			SAFE_RELEASE( ( *iter ) );
			iter = m_vecLayer.erase( iter );
		}

		else
			++iter;
	}

	m_pMainCameraObj->Update( fTime );

	// 조명 업데이트
	list<CGameObject*>::iterator	iterL;
	list<CGameObject*>::iterator	iterLEnd = m_LightList.end();

	for ( iterL = m_LightList.begin(); iterL != iterLEnd;)
	{
		if ( !( *iterL )->GetAlive() )
		{
			SAFE_RELEASE( ( *iterL ) );
			iterL = m_LightList.erase( iterL );
			continue;
		}

		else if ( !( *iterL )->GetEnable() )
		{
			++iterL;
			continue;
		}

		( *iterL )->Update( fTime );
		++iterL;
	}

	// 스카이 박스 업데이트
	m_pSkyObject->Update( fTime );

	return 0;
}

int CScene::LateUpdate( float fTime )
{
	vector<CSceneScript*>::iterator	iter1;
	vector<CSceneScript*>::iterator	iter1End = m_vecSceneScript.end();

	for ( iter1 = m_vecSceneScript.begin(); iter1 != iter1End;)
	{
		if ( !( *iter1 )->GetEnable() )
		{
			++iter1;
			continue;
		}

		( *iter1 )->LateUpdate( fTime );

		if ( !( *iter1 )->GetAlive() )
		{
			SAFE_RELEASE( ( *iter1 ) );
			iter1 = m_vecSceneScript.erase( iter1 );
		}

		else
			++iter1;
	}

	vector<CLayer*>::iterator	iter;
	vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

	for ( iter = m_vecLayer.begin(); iter != iterEnd;)
	{
		if ( !( *iter )->GetEnable() )
		{
			++iter;
			continue;
		}

		( *iter )->LateUpdate( fTime );

		if ( !( *iter )->GetAlive() )
		{
			SAFE_RELEASE( ( *iter ) );
			iter = m_vecLayer.erase( iter );
		}

		else
			++iter;
	}

	m_pMainCameraObj->LateUpdate( fTime );

	// 조명 업데이트
	list<CGameObject*>::iterator	iterL;
	list<CGameObject*>::iterator	iterLEnd = m_LightList.end();

	for ( iterL = m_LightList.begin(); iterL != iterLEnd;)
	{
		if ( !( *iterL )->GetAlive() )
		{
			SAFE_RELEASE( ( *iterL ) );
			iterL = m_LightList.erase( iterL );
			continue;
		}

		else if ( !( *iterL )->GetEnable() )
		{
			++iterL;
			continue;
		}

		( *iterL )->LateUpdate( fTime );
		++iterL;
	}

	// 스카이 박스 업데이트
	m_pSkyObject->LateUpdate( fTime );

	return 0;
}

void CScene::Collision( float fTime )
{
	vector<CSceneScript*>::iterator	iter1;
	vector<CSceneScript*>::iterator	iter1End = m_vecSceneScript.end();

	for ( iter1 = m_vecSceneScript.begin(); iter1 != iter1End;)
	{
		if ( !( *iter1 )->GetEnable() )
		{
			++iter1;
			continue;
		}

		( *iter1 )->Collision( fTime );

		if ( !( *iter1 )->GetAlive() )
		{
			SAFE_RELEASE( ( *iter1 ) );
			iter1 = m_vecSceneScript.erase( iter1 );
		}

		else
			++iter1;
	}

	vector<CLayer*>::iterator	iter;
	vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

	for ( iter = m_vecLayer.begin(); iter != iterEnd;)
	{
		if ( !( *iter )->GetEnable() )
		{
			++iter;
			continue;
		}

		( *iter )->Collision( fTime );

		if ( !( *iter )->GetAlive() )
		{
			SAFE_RELEASE( ( *iter ) );
			iter = m_vecLayer.erase( iter );
		}

		else
			++iter;
	}
}

void CScene::Render( float fTime )
{
	vector<CSceneScript*>::iterator	iter1;
	vector<CSceneScript*>::iterator	iter1End = m_vecSceneScript.end();

	for ( iter1 = m_vecSceneScript.begin(); iter1 != iter1End;)
	{
		if ( !( *iter1 )->GetEnable() )
		{
			++iter1;
			continue;
		}

		( *iter1 )->Render( fTime );

		if ( !( *iter1 )->GetAlive() )
		{
			SAFE_RELEASE( ( *iter1 ) );
			iter1 = m_vecSceneScript.erase( iter1 );
		}

		else
			++iter1;
	}

	// 스카이 박스를 그리고 레이어 내에있는 오브젝트를 그린다.
	m_pSkyObject->Render( fTime );

	vector<CLayer*>::iterator	iter;
	vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

	for ( iter = m_vecLayer.begin(); iter != iterEnd;)
	{
		if ( !( *iter )->GetEnable() )
		{
			++iter;
			continue;
		}

		( *iter )->Render( fTime );

		if ( !( *iter )->GetAlive() )
		{
			SAFE_RELEASE( ( *iter ) );
			iter = m_vecLayer.erase( iter );
		}

		else
			++iter;
	}
}

CLayer * CScene::CreateLayer( const string & strTag, int iZOrder )
{
	CLayer*	pLayer = new CLayer;

	pLayer->SetTag( strTag );
	pLayer->SetZOrder( iZOrder );
	pLayer->SetScene( this );

	if ( !pLayer->Init() )
	{
		SAFE_RELEASE( pLayer );
		return NULL;
	}

	pLayer->AddRef();
	m_vecLayer.push_back( pLayer );

	if ( m_vecLayer.size() > 1 )
		sort( m_vecLayer.begin(), m_vecLayer.end(), CScene::SortZ );

	return pLayer;
}

CLayer * CScene::GetLayer( const string & strTag )
{
	vector<CLayer*>::iterator	iter;
	vector<CLayer*>::iterator	iterEnd = m_vecLayer.end();

	for ( iter = m_vecLayer.begin(); iter != iterEnd; ++iter )
	{
		if ( ( *iter )->GetTag() == strTag )
		{
			( *iter )->AddRef();
			return *iter;
		}
	}

	return NULL;
}

bool CScene::SortZ( CLayer * p1, CLayer * p2 )
{
	return p1->GetZOrder() < p2->GetZOrder();
}
