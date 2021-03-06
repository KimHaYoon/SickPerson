#pragma once
#include "../Obj.h"

Engine_BEGIN

class Engine_DLL CScene :
	public CObj
{
private:
	friend class CSceneManager;

private:
	CScene();
	~CScene();

private:
	vector<class CLayer*>		m_vecLayer;
	vector<class CSceneScript*>	m_vecSceneScript;
	class CGameObject*	m_pMainCameraObj;
	class CTransform*	m_pMainCameraTr;
	class CCamera*		m_pMainCamera;
	class CGameObject*	m_pUICameraObj;
	class CTransform*	m_pUICameraTr;
	class CCamera*		m_pUICamera;
	unordered_map<string, class CGameObject*>	m_mapCamera;
	list<class CGameObject*>	m_LightList;
	class CGameObject*	m_pSkyObject;

public:
	class CGameObject* CreatePrototype(const string& strKey);
	CGameObject* CreateCamera(const string& strKey,
		const Vector3& vPos, float fViewAngle, float fAspect,
		float fNear, float fFar);
	CGameObject* CreateOrthoCamera(const string& strKey,
		const Vector3& vPos, const RESOLUTION& tRS,
		float fNear, float fFar);
	CGameObject* FindCamera(const string& strKey);

	CGameObject* GetMainCameraObj()	const;
	CTransform* GetMainCameraTr()	const;
	CCamera* GetMainCamera()	const;

	void ChangeMainCamera(const string& strKey);

	CGameObject* GetUICameraObj()	const;
	CTransform* GetUICameraTr()	const;
	CCamera* GetUICamera()	const;


	class CLight* CreateLight(const string& strTag, LIGHT_TYPE eType);
	class CLight* GetGlobalLight(const string& strTag = "GlobalLight");
	const list<class CGameObject*>* GetLightList();

public:
	bool Init();
	void Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Collision(float fTime);
	void Render(float fTime);
	void Save(char* pFileName, const string& strPathKey = DATA_PATH);
	void Save(FILE* pFile);
	void SaveFromFullPath(const char* pFullPath);
	void Load(char* pFileName, const string& strPathKey = DATA_PATH);
	void Load(FILE* pFile);
	void LoadFromFullPath(const char* pFullPath);

public:
	class CLayer* CreateLayer(const string& strTag = "", int iZOrder = 0);
	class CLayer* GetLayer(const string& strTag);
	template <typename T>
	T* CreateSceneScript(const string& strTag)
	{
		T*	pScript = new T;

		pScript->SetScene(this);
		pScript->SetTag(strTag);

		if (!pScript->Init())
		{
			SAFE_RELEASE(pScript);
			return NULL;
		}

		pScript->AddRef();
		m_vecSceneScript.push_back(pScript);

		return pScript;
	}

public:
	static bool SortZ(class CLayer* p1, class CLayer* p2);
};

Engine_END