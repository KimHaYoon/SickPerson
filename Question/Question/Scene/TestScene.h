#pragma once
#include "SceneScript.h"
class CTestScene : public CSceneScript
{
public:
	CTestScene();
	~CTestScene();

public:
	virtual bool Init();
	virtual int Update( float fTime );
};

