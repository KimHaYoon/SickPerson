#pragma once
#include "../Obj.h"

GAME_BEGIN

class GAME_DLL CScene : public  CObj
{
private:
	friend class CSceneManager;

private:
	CScene();
	virtual ~CScene();

private:
	vector<class CLayer*>		m_vecLayer;
	vector<class CSceneScript*>	m_vecSceneScript;
	class CGameObject*	m_pMainCameraObj;
	class CTransform*	m_pMainCameraTr;
	class CCamera*		m_pMainCamera;
	unordered_map<string, class CGameObject*>	m_mapCamera;

public:
	class CGameObject* CreatePrototype( const string& strKey );
	CGameObject* CreateCamera( const string& strKey,
		const Vector3& vPos, float fViewAngle, float fAspect,
		float fNear, float fFar );
	CGameObject* CreateOrthoCamera( const string& strKey,
		const Vector3& vPos, const RESOLUTION& tRS,
		float fNear, float fFar );
	CGameObject* FindCamera( const string& strKey );

	CGameObject* GetMainCameraObj()	const;
	CTransform* GetMainCameraTr()	const;
	CCamera* GetMainCamera()	const;

public:
	bool Init();
	void Input( float fTime );
	int Update( float fTime );
	int LateUpdate( float fTime );
	void Collision( float fTime );
	void Render( float fTime );
	void Save( char* pFileName, const string& strPathKey = DATA_PATH );
	void Save( FILE* pFile );
	void SaveFromFullPath( const char* pFullPath );
	void Load( char* pFileName, const string& strPathKey = DATA_PATH );
	void Load( FILE* pFile );
	void LoadFromFullPath( const char* pFullPath );

public:
	class CLayer* CreateLayer( const string& strTag = "", int iZOrder = 0 );
	class CLayer* GetLayer( const string& strTag );
	template <typename T>
	T* CreateSceneScript( const string& strTag )
	{
		T*	pScript = new T;

		pScript->SetScene( this );
		pScript->SetTag( strTag );

		if ( !pScript->Init() )
		{
			SAFE_RELEASE( pScript );
			return NULL;
		}

		pScript->AddRef();
		m_vecSceneScript.push_back( pScript );

		return pScript;
	}

public:
	static bool SortZ( class CLayer* p1, class CLayer* p2 );
};

GAME_END