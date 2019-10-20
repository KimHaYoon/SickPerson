#include "TestScene.h"
#include "Layer.h"
#include "Scene.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Pyramid.h"



CTestScene::CTestScene()
{
}


CTestScene::~CTestScene()
{
}

bool CTestScene::Init()
{
	CLayer*	pLayer = m_pScene->GetLayer( "Default" );
	CGameObject* pPyramidObj = CGameObject::CreateObject( "Pyramid", pLayer );
	CPyramid* pPyramid = pPyramidObj->AddComponent<CPyramid>( "PyramidCom" );
	SAFE_RELEASE( pPyramid );
	SAFE_RELEASE( pPyramidObj );
	SAFE_RELEASE( pLayer );
	return true;
}

int CTestScene::Update( float fTime )
{
	return 0;
}
