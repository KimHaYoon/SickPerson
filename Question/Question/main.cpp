#include "Core.h"
#include "resource.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/TestScene.h"

int APIENTRY wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow )
{
	if ( !GET_SINGLE( CCore )->Init( hInstance, L"HaYoon", L"HaYoon", IDI_ICON1, 1280, 720, true ) )
	{
		DESTROY_SINGLE( CCore );
		return 0;
	}
	CScene* pScene = GET_SINGLE( CSceneManager )->GetCurrentScene();
	CTestScene* pTestScene = pScene->CreateSceneScript<CTestScene>( "TestScene" );

	SAFE_RELEASE( pTestScene );
	SAFE_RELEASE( pScene );

	int iRet = GET_SINGLE( CCore )->Run();

	DESTROY_SINGLE( CCore );

	return iRet;
}