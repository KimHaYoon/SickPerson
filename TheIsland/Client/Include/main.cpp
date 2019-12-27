#include "Client.h"
#include "Core.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneScript.h"
#include "SceneScript/TestScene.h"

GAME_USING

int APIENTRY wWinMain( _In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow )
{
	if ( !GET_SINGLE( CCore )->Init( hInstance, L"The Island", L"The Island",
		IDI_ICON1, 1280, 720, true ) )
	{
		DESTROY_SINGLE( CCore );
		return 0;
	}

	CScene*	pScene = GET_SINGLE( CSceneManager )->GetCurrentScene();

	CTestScene*	pStartScene = pScene->CreateSceneScript<CTestScene>( "TestScene" );

	SAFE_RELEASE( pStartScene );

	SAFE_RELEASE( pScene );


	int iRet = GET_SINGLE( CCore )->Run();

	DESTROY_SINGLE( CCore );

	return iRet;
}
