#pragma once

#include "Scene/SceneScript.h"

Engine_USING

class InGameScene :
	public CSceneScript
{
public:
	InGameScene();
	~InGameScene();

public:
	virtual bool Init();
	virtual int Update(float fTime);
};
