#pragma once

#include "../Obj.h"
#include "../Resources/FbxLoader.h"

Engine_BEGIN

typedef struct Engine_DLL _tagKeyFrame
{
	double	dTime;
	Vector3	vPos;
	Vector3	vScale;
	Vector4	vRot;
}KEYFRAME, *PKEYFRAME;

typedef struct Engine_DLL _tagBoneKeyFrame
{
	int		iBoneIndex;
	vector<PKEYFRAME>	vecKeyFrame;
}BONEKEYFRAME, *PBONEKEYFRAME;

typedef struct Engine_DLL _tagAnimationCallback
{
	int		iAnimationProgress;
	float	fAnimationProgress;
	function<void(float)> func;
	bool	bCall;
}ANIMATIONCALLBACK, *PANIMATIONCALLBACK;

typedef struct Engine_DLL _tagAnimation3DClip
{
	ANIMATION_OPTION	eOption;
	string		strName;
	float		fStartTime;
	float		fEndTime;
	float		fTimeLength;
	int			iStartFrame;
	int			iEndFrame;
	int			iFrameLength;
	vector<PANIMATIONCALLBACK>	vecCallback;

	_tagAnimation3DClip() :
		eOption(AO_LOOP),
		strName(""),
		fStartTime(0),
		fEndTime(0),
		fTimeLength(0),
		iStartFrame(0),
		iEndFrame(0),
		iFrameLength(0)
	{
	}
}ANIMATION3DCLIP, *PANIMATION3DCLIP;

class Engine_DLL CAnimationClip :
	public CObj
{
private:
	friend class CAnimation;

private:
	CAnimationClip();
	CAnimationClip(const CAnimationClip& clip);
	~CAnimationClip();

private:
	ANIMATION3DCLIP		m_tInfo;
	int					m_iFrameMode;
	vector<PBONEKEYFRAME>	m_vecKeyFrame;
	int					m_iAnimationLimitFrame;

public:
	ANIMATION3DCLIP GetClipInfo()	const;
	PKEYFRAME GetKeyFrame(int iBone, int iFrame)	const;
	bool IsEmptyFrame(int iBone)	const;

public:
	void SetClipInfo(const string& strName, ANIMATION_OPTION eOption,
		int iFrameMode, int iStartFrame, int iEndFrame,
		float fStartTime, float fEndTime);
	void SetClipInfo(ANIMATION_OPTION eOption, PFBXANIMATIONCLIP pClip);
	void AddCallback(int iFrame, void(*pFunc)(float));
	void AddCallback(float fProgress, void(*pFunc)(float));
	bool Save(FILE* pFile);
	bool Load(FILE* pFile);

public:
	bool Init();
	void Update(float fTime);
	void LateUpdate(float fTime);
	CAnimationClip* Clone()	const;

public:
	template <typename T>
	void AddCallback(int iFrame, T* pObj, void(T::*pFunc)(float))
	{
		// �ִϸ��̼� �߰��� ȣ���� �ݹ��Լ��� �߰��Ѵ�.
		PANIMATIONCALLBACK	pCallback = new ANIMATIONCALLBACK;

		// ���� �������� �����Ѵ�.
		pCallback->iAnimationProgress = iFrame;
		// ���� �����ӿ��� ���� �������� ���ְ� ���̷� ����� ���� �������� �����ش�.
		pCallback->fAnimationProgress = (iFrame - m_tInfo.iStartFrame) /
			(float)m_tInfo.iFrameLength;
		pCallback->func = bind(pFunc, pObj, placeholders::_1);
		pCallback->bCall = false;

		m_tInfo.vecCallback.push_back(pCallback);
	}

	template <typename T>
	void AddCallback(float fProgress, T* pObj, void(T::*pFunc)(float))
	{
		PANIMATIONCALLBACK	pCallback = new ANIMATIONCALLBACK;

		// �������� ������ ������ �������� �����ش�. ������ * �ð����� + ���۽ð��� �ؼ� ���� �ð���
		// �����ְ� 1�ʴ� ������ ���� �����־ �������� ���Ѵ�.
		pCallback->iAnimationProgress = (fProgress * m_tInfo.fTimeLength + m_tInfo.fStartTime) *
			m_iAnimationLimitFrame;
		pCallback->fAnimationProgress = fProgress;
		pCallback->func = bind(pFunc, pObj, placeholders::_1);
		pCallback->bCall = false;

		m_tInfo.vecCallback.push_back(pCallback);
	}
};

Engine_END