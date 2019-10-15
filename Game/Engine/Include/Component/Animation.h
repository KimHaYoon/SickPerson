#pragma once
#include "Component.h"

Engine_BEGIN

typedef struct Engine_DLL _tagBone
{
	string		strName;
	int			iDepth;
	int			iParentIndex;
	PMatrix		matOffset;
	PMatrix		matBone;
}BONE, *PBONE;

typedef struct Engine_DLL _tagAnimationChannel
{
	class CAnimationClip*	pClip;
	float		fWeight;
	int			iChangeFrame;

	_tagAnimationChannel() :
		pClip(NULL),
		fWeight(1.f),
		iChangeFrame(0)
	{
	}
}ANIMCHANNEL, *PANIMCHANNEL;

class Engine_DLL CAnimation :
	public CComponent
{
private:
	friend class CGameObject;
	friend class CMesh;

private:
	CAnimation();
	CAnimation(const CAnimation& animation);
	~CAnimation();

private:
	vector<PBONE>		m_vecBones;
	ID3D12Resource*		m_pBoneTex;
	ID3D12ShaderResourceView*	m_pBoneRV;
	int					m_iFrameMode;
	unordered_map<string, class CAnimationClip*>	m_mapClip;
	vector<ANIMCHANNEL>	m_vecChannel;
	int					m_iCurChannel;
	int					m_iNextChannel;
	float				m_fFrameTime;
	bool				m_bEnd;
	float				m_fAnimationGlobalTime;
	float				m_fClipProgress;
	float				m_fChangeTime;
	float				m_fChangeLimitTime;

private:
	string		m_strDefaultClip;
	string		m_strCurrentClip;
	class CAnimationClip*	m_pLastAddClip;

public:
	class CAnimationClip* FindClip(const string& strKey);
	bool IsAnimationEnd()	const;
	class CAnimationClip* GetCurrentClip()	const;
	class CAnimationClip* GetLastAddClip()	const;
	const unordered_map<string, class CAnimationClip*>* GetAllClip()	const;


public:
	void AddClipCallback(const string& strName, int iFrame, void(*pFunc)(float));
	void AddClipCallback(const string& strName, float fProgress, void(*pFunc)(float));
	void AddBone(PBONE pBone);
	void AddClip(ANIMATION_OPTION eOption,
		struct _tagFbxAnimationClip* pClip);
	void AddClip(const string& strKey, ANIMATION_OPTION eOption,
		int iAnimationLimitFrame, int iStartFrame, int iEndFrame,
		float fStartTime, float fEndTime);
	void AddClip(const string& strKey, ANIMATION_OPTION eOption,
		int iStartFrame, int iEndFrame);
	void AddClip(const string& strKey, class CAnimationClip* pClip);
	void AddClip(const wchar_t* pFullPath);
	void AddClip(const char* pFullPath);
	bool CreateBoneTexture();
	void SetDefaultClip(const string& strClip);
	void SetCurrentClip(const string& strClip);
	bool ChangeClip(const string& strClip);
	ID3D12ShaderResourceView* GetBoneTexture()	const;
	bool Save(const wchar_t* pFileName, const string& strPathKey = MESH_PATH);
	bool Save(const char* pFileName, const string& strPathKey = MESH_PATH);
	bool SaveFromFullPath(const wchar_t* pFullPath);
	bool SaveFromFullPath(const char* pFullPath);
	bool Load(const wchar_t* pFileName, const string& strPathKey = MESH_PATH);
	bool Load(const char* pFileName, const string& strPathKey = MESH_PATH);
	bool LoadFromFullPath(const wchar_t* pFullPath);
	bool LoadFromFullPath(const char* pFullPath);
	bool ModifyClip(const string& strKey, const string& strChangeKey,
		ANIMATION_OPTION eOption, int iStartFrame, int iEndFrame);
	bool DeleteClip(const string& strKey);
	bool ReturnDefaultClip();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CAnimation* Clone();

public:
	template <typename T>
	void AddClipCallback(const string& strName, int iFrame, T* pObj, void(T::*pFunc)(float))
	{
		CAnimationClip*	pClip = FindClip(strName);

		if (!pClip)
			return;

		pClip->AddCallback(iFrame, pObj, pFunc);
	}

	template <typename T>
	void AddClipCallback(const string& strName, float fProgress, T* pObj, void(T::*pFunc)(float))
	{
		CAnimationClip*	pClip = FindClip(strName);

		if (!pClip)
			return;

		pClip->AddCallback(fProgress, pObj, pFunc);
	}
};

Engine_END