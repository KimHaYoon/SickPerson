#include "Core.h"

Engine_USING

int APIENTRY wWinMain( _In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow )
{
	if ( !GET_SINGLE( CCore )->Init( hInstance, L"NewBegin", L"NewBegin", IDI_ICON1, 1280, 720, true ) )
	{
		DESTROY_SINGLE( CCore );
		return 0;
	}

	CScene*	pScene = GET_SINGLE( CSceneManager )->GetCurrentScene();

	CStartScene*	pStartScene = pScene->CreateSceneScript<CStartScene>( "StartScene" );

	SAFE_RELEASE( pStartScene );

	/*CMainScene*	pMainScene = pScene->CreateSceneScript<CMainScene>("MainScene");

	SAFE_RELEASE(pMainScene);*/

	SAFE_RELEASE( pScene );

	int iRet = GET_SINGLE( CCore )->Run();

	DESTROY_SINGLE( CCore );

	return 0;
}


