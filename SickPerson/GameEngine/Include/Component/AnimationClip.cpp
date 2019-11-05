#include "AnimationClip.h"

Engine_USING

CAnimationClip::CAnimationClip() :
	m_iAnimationLimitFrame(24)
{
}

CAnimationClip::CAnimationClip(const CAnimationClip & clip)
{
	*this = clip;
	m_iRefCount = 1;

	m_tInfo.vecCallback.clear();

	m_vecKeyFrame.clear();

	for (size_t i = 0; i < clip.m_vecKeyFrame.size(); ++i)
	{
		PBONEKEYFRAME	pBoneKeyFrame = new BONEKEYFRAME;

		pBoneKeyFrame->iBoneIndex = clip.m_vecKeyFrame[i]->iBoneIndex;

		m_vecKeyFrame.push_back(pBoneKeyFrame);
		for (size_t j = 0; j < clip.m_vecKeyFrame[i]->vecKeyFrame.size(); ++j)
		{
			PKEYFRAME	pFrame = new KEYFRAME;
			*pFrame = *clip.m_vecKeyFrame[i]->vecKeyFrame[j];
			pBoneKeyFrame->vecKeyFrame.push_back(pFrame);
		}
	}
}

CAnimationClip::~CAnimationClip()
{
	for (size_t i = 0; i < m_vecKeyFrame.size(); ++i)
	{
		Safe_Delete_VecList(m_vecKeyFrame[i]->vecKeyFrame);
	}

	Safe_Delete_VecList(m_vecKeyFrame);

	Safe_Delete_VecList(m_tInfo.vecCallback);
}

ANIMATION3DCLIP CAnimationClip::GetClipInfo() const
{
	return m_tInfo;
}

PKEYFRAME CAnimationClip::GetKeyFrame(int iBone, int iFrame) const
{
	assert(iBone >= 0 && iBone < m_vecKeyFrame.size());

	return m_vecKeyFrame[iBone]->vecKeyFrame[iFrame];
}

bool CAnimationClip::IsEmptyFrame(int iBone) const
{
	return m_vecKeyFrame[iBone]->vecKeyFrame.empty();
}

void CAnimationClip::SetClipInfo(const string & strName, ANIMATION_OPTION eOption,
	int iFrameMode, int iStartFrame, int iEndFrame, float fStartTime, float fEndTime)
{
	m_tInfo.eOption = eOption;
	m_tInfo.strName = strName;
	m_tInfo.iStartFrame = iStartFrame;
	m_tInfo.iEndFrame = iEndFrame;
	m_tInfo.iFrameLength = iEndFrame - iStartFrame;
	m_tInfo.fStartTime = fStartTime;
	m_tInfo.fEndTime = fEndTime;
	m_tInfo.fTimeLength = fEndTime - fStartTime;
	m_iAnimationLimitFrame = iFrameMode;
}

void CAnimationClip::SetClipInfo(ANIMATION_OPTION eOption,
	PFBXANIMATIONCLIP pClip)
{
	// 인자로 들어온 애니메이션 옵션정보를 설정한다.
	m_tInfo.eOption = eOption;
	m_tInfo.strName = pClip->strName;

	// FBXANIMATIONCLIP에 있는 starttime 과 endtime 을 이용하여 keyframe 을 얻어온다.
	m_tInfo.iStartFrame = pClip->tStart.GetFrameCount(pClip->eTimeMode);
	m_tInfo.iEndFrame = pClip->tEnd.GetFrameCount(pClip->eTimeMode);
	m_tInfo.iFrameLength = m_tInfo.iEndFrame - m_tInfo.iStartFrame;

	// 시간 정보를 저장해준다.
	m_tInfo.fStartTime = pClip->tStart.GetSecondDouble();
	m_tInfo.fEndTime = pClip->tEnd.GetSecondDouble();
	m_tInfo.fTimeLength = m_tInfo.fEndTime - m_tInfo.fStartTime;

	// 키 프레임 수만큼 반복하며 각각의 프레임을 보간할 행렬 정보를 위치, 크기, 회전정보로
	// 뽑아온다.
	for (size_t i = 0; i < pClip->vecBoneKeyFrame.size(); ++i)
	{
		PBONEKEYFRAME	pBoneKeyFrame = new BONEKEYFRAME;

		pBoneKeyFrame->iBoneIndex = pClip->vecBoneKeyFrame[i].iBoneIndex;

		m_vecKeyFrame.push_back(pBoneKeyFrame);

		// 아래부터 키프레임 정보를 저장한다.
		pBoneKeyFrame->vecKeyFrame.reserve(pClip->vecBoneKeyFrame[i].vecKeyFrame.size());

		for (size_t j = 0; j < pClip->vecBoneKeyFrame[i].vecKeyFrame.size(); ++j)
		{
			PKEYFRAME	pKeyFrame = new KEYFRAME;

			pKeyFrame->dTime = pClip->vecBoneKeyFrame[i].vecKeyFrame[j].dTime;

			// 현재 본의 키 프레임에 해당하는 행렬 정보를 얻어온다.
			FbxAMatrix	mat = pClip->vecBoneKeyFrame[i].vecKeyFrame[j].matTransform;

			FbxVector4	vPos, vScale;
			FbxQuaternion	qRot;

			// 행렬로부터 위치, 크기, 회전 정보를 얻어온다.
			vPos = mat.GetT();
			vScale = mat.GetS();
			qRot = mat.GetQ();

			pKeyFrame->vScale = Vector3((float)vScale.mData[0], (float)vScale.mData[1],
				(float)vScale.mData[2]);
			pKeyFrame->vPos = Vector3((float)vPos.mData[0], (float)vPos.mData[1],
				(float)vPos.mData[2]);
			pKeyFrame->vRot = Vector4((float)qRot.mData[0], (float)qRot.mData[1],
				(float)qRot.mData[2], (float)qRot.mData[3]);

			pBoneKeyFrame->vecKeyFrame.push_back(pKeyFrame);
		}
	}
}

void CAnimationClip::AddCallback(int iFrame, void(*pFunc)(float))
{
	// 애니메이션 중간에 호출할 콜백함수를 추가한다.
	PANIMATIONCALLBACK	pCallback = new ANIMATIONCALLBACK;

	// 지정 프레임을 설정한다.
	pCallback->iAnimationProgress = iFrame;
	// 지정 프레임에서 시작 프레임을 빼주고 길이로 나누어서 현재 진행율을 구해준다.
	pCallback->fAnimationProgress = (iFrame - m_tInfo.iStartFrame) /
		(float)m_tInfo.iFrameLength;
	pCallback->func = bind(pFunc, placeholders::_1);
	pCallback->bCall = false;

	m_tInfo.vecCallback.push_back(pCallback);
}

void CAnimationClip::AddCallback(float fProgress, void(*pFunc)(float))
{
	PANIMATIONCALLBACK	pCallback = new ANIMATIONCALLBACK;

	// 진행율이 들어오면 역으로 프레임을 구해준다. 진행율 * 시간길이 + 시작시간을 해서 현재 시간을
	// 구해주고 1초당 프레임 값을 곱해주어서 프레임을 구한다.
	pCallback->iAnimationProgress = (fProgress * m_tInfo.fTimeLength + m_tInfo.fStartTime) *
		m_iAnimationLimitFrame;
	pCallback->fAnimationProgress = fProgress;
	pCallback->func = bind(pFunc, placeholders::_1);
	pCallback->bCall = false;

	m_tInfo.vecCallback.push_back(pCallback);
}

bool CAnimationClip::Save(FILE * pFile)
{
	fwrite(&m_iFrameMode, sizeof(int), 1, pFile);
	fwrite(&m_iAnimationLimitFrame, sizeof(int), 1, pFile);

	fwrite(&m_tInfo.eOption, sizeof(ANIMATION_OPTION), 1, pFile);

	size_t	iLength = m_tInfo.strName.length();

	fwrite(&iLength, sizeof(size_t), 1, pFile);
	fwrite(m_tInfo.strName.c_str(), sizeof(char), iLength, pFile);
	fwrite(&m_tInfo.fStartTime, sizeof(float), 1, pFile);
	fwrite(&m_tInfo.fEndTime, sizeof(float), 1, pFile);
	fwrite(&m_tInfo.fTimeLength, sizeof(float), 1, pFile);
	fwrite(&m_tInfo.iStartFrame, sizeof(int), 1, pFile);
	fwrite(&m_tInfo.iEndFrame, sizeof(int), 1, pFile);
	fwrite(&m_tInfo.iFrameLength, sizeof(int), 1, pFile);

	size_t	iCount = m_vecKeyFrame.size();

	fwrite(&iCount, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iCount; ++i)
	{
		fwrite(&m_vecKeyFrame[i]->iBoneIndex, sizeof(int), 1, pFile);

		size_t	iFrameCount = m_vecKeyFrame[i]->vecKeyFrame.size();

		fwrite(&iFrameCount, sizeof(size_t), 1, pFile);

		for (size_t j = 0; j < iFrameCount; ++j)
		{
			fwrite(&m_vecKeyFrame[i]->vecKeyFrame[j]->dTime, sizeof(double), 1, pFile);
			fwrite(&m_vecKeyFrame[i]->vecKeyFrame[j]->vPos, sizeof(Vector3), 1, pFile);
			fwrite(&m_vecKeyFrame[i]->vecKeyFrame[j]->vScale, sizeof(Vector3), 1, pFile);
			fwrite(&m_vecKeyFrame[i]->vecKeyFrame[j]->vRot, sizeof(Vector4), 1, pFile);
		}
	}

	return true;
}

bool CAnimationClip::Load(FILE * pFile)
{
	fread(&m_iFrameMode, sizeof(int), 1, pFile);
	fread(&m_iAnimationLimitFrame, sizeof(int), 1, pFile);

	fread(&m_tInfo.eOption, sizeof(ANIMATION_OPTION), 1, pFile);

	size_t	iLength = 0;
	char	strName[256] = {};

	fread(&iLength, sizeof(size_t), 1, pFile);
	fread(strName, sizeof(char), iLength, pFile);
	m_tInfo.strName = strName;

	fread(&m_tInfo.fStartTime, sizeof(float), 1, pFile);
	fread(&m_tInfo.fEndTime, sizeof(float), 1, pFile);
	fread(&m_tInfo.fTimeLength, sizeof(float), 1, pFile);
	fread(&m_tInfo.iStartFrame, sizeof(int), 1, pFile);
	fread(&m_tInfo.iEndFrame, sizeof(int), 1, pFile);
	fread(&m_tInfo.iFrameLength, sizeof(int), 1, pFile);

	size_t	iCount = 0;

	fread(&iCount, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iCount; ++i)
	{
		PBONEKEYFRAME	pBoneKeyFrame = new BONEKEYFRAME;

		m_vecKeyFrame.push_back(pBoneKeyFrame);

		fread(&pBoneKeyFrame->iBoneIndex, sizeof(int), 1, pFile);

		size_t	iFrameCount = 0;

		fread(&iFrameCount, sizeof(size_t), 1, pFile);

		for (size_t j = 0; j < iFrameCount; ++j)
		{
			PKEYFRAME	pFrame = new KEYFRAME;
			fread(&pFrame->dTime, sizeof(double), 1, pFile);
			fread(&pFrame->vPos, sizeof(Vector3), 1, pFile);
			fread(&pFrame->vScale, sizeof(Vector3), 1, pFile);
			fread(&pFrame->vRot, sizeof(Vector4), 1, pFile);
			pBoneKeyFrame->vecKeyFrame.push_back(pFrame);
		}
	}

	return true;
}

bool CAnimationClip::Init()
{
	return true;
}

void CAnimationClip::Update(float fTime)
{
}

void CAnimationClip::LateUpdate(float fTime)
{
}

CAnimationClip * CAnimationClip::Clone() const
{
	return new CAnimationClip(*this);
}
