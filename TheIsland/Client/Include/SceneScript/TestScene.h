#pragma once
#include "Scene/SceneScript.h"

GAME_USING

class CTestScene : public CSceneScript
{
public:
	CTestScene();
	~CTestScene();

public:
	virtual bool Init();
	virtual int Update( float fTime );
};

