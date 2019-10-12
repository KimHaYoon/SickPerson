#include "SceneScript.h"

Engine_USING

CSceneScript::CSceneScript()
{
}

CSceneScript::~CSceneScript()
{
}

void CSceneScript::SetScene(CScene * pScene)
{
	m_pScene = pScene;
}

void CSceneScript::Input(float fTime)
{
}

int CSceneScript::Update(float fTime)
{
	return 0;
}

int CSceneScript::LateUpdate(float fTime)
{
	return 0;
}

void CSceneScript::Collision(float fTime)
{
}

void CSceneScript::Render(float fTime)
{
}
