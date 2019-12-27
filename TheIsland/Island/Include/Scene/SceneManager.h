#pragma once

#include "../Engine.h"

GAME_BEGIN

class GAME_DLL CSceneManager
{
private:
	class CScene*	m_pCurScene;
	class CScene*	m_pNextScene;

public:
	class CScene* CreateScene( const string& strTag );
	class CScene* GetCurrentScene()	const;
	class CScene* CreateNextScene( const string& strTag );

public:
	bool Init();
	void Input( float fTime );
	SCENE_CHANGE Update( float fTime );
	SCENE_CHANGE LateUpdate( float fTime );
	void Collision( float fTime );
	void Render( float fTime );

private:
	SCENE_CHANGE ChangeScene();

	DECLARE_SINGLE( CSceneManager )
};

GAME_END