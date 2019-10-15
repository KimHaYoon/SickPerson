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
	// ���ڷ� ���� �ִϸ��̼� �ɼ������� �����Ѵ�.
	m_tInfo.eOption = eOption;
	m_tInfo.strName = pClip->strName;

	// FBXANIMATIONCLIP�� �ִ� starttime �� endtime �� �̿��Ͽ� keyframe �� ���´�.
	m_tInfo.iStartFrame = pClip->tStart.GetFrameCount(pClip->eTimeMode);
	m_tInfo.iEndFrame = pClip->tEnd.GetFrameCount(pClip->eTimeMode);
	m_tInfo.iFrameLength = m_tInfo.iEndFrame - m_tInfo.iStartFrame;

	// �ð� ������ �������ش�.
	m_tInfo.fStartTime = pClip->tStart.GetSecondDouble();
	m_tInfo.fEndTime = pClip->tEnd.GetSecondDouble();
	m_tInfo.fTimeLength = m_tInfo.fEndTime - m_tInfo.fStartTime;

	// Ű ������ ����ŭ �ݺ��ϸ� ������ �������� ������ ��� ������ ��ġ, ũ��, ȸ��������
	// �̾ƿ´�.
	for (size_t i = 0; i < pClip->vecBoneKeyFrame.size(); ++i)
	{
		PBONEKEYFRAME	pBoneKeyFrame = new BONEKEYFRAME;

		pBoneKeyFrame->iBoneIndex = pClip->vecBoneKeyFrame[i].iBoneIndex;

		m_vecKeyFrame.push_back(pBoneKeyFrame);

		// �Ʒ����� Ű������ ������ �����Ѵ�.
		pBoneKeyFrame->vecKeyFrame.reserve(pClip->vecBoneKeyFrame[i].vecKeyFrame.size());

		for (size_t j = 0; j < pClip->vecBoneKeyFrame[i].vecKeyFrame.size(); ++j)
		{
			PKEYFRAME	pKeyFrame = new KEYFRAME;

			pKeyFrame->dTime = pClip->vecBoneKeyFrame[i].vecKeyFrame[j].dTime;

			// ���� ���� Ű �����ӿ� �ش��ϴ� ��� ������ ���´�.
			FbxAMatrix	mat = pClip->vecBoneKeyFrame[i].vecKeyFrame[j].matTransform;

			FbxVector4	vPos, vScale;
			FbxQuaternion	qRot;

			// ��ķκ��� ��ġ, ũ��, ȸ�� ������ ���´�.
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
	// �ִϸ��̼� �߰��� ȣ���� �ݹ��Լ��� �߰��Ѵ�.
	PANIMATIONCALLBACK	pCallback = new ANIMATIONCALLBACK;

	// ���� �������� �����Ѵ�.
	pCallback->iAnimationProgress = iFrame;
	// ���� �����ӿ��� ���� �������� ���ְ� ���̷� ����� ���� �������� �����ش�.
	pCallback->fAnimationProgress = (iFrame - m_tInfo.iStartFrame) /
		(float)m_tInfo.iFrameLength;
	pCallback->func = bind(pFunc, placeholders::_1);
	pCallback->bCall = false;

	m_tInfo.vecCallback.push_back(pCallback);
}

void CAnimationClip::AddCallback(float fProgress, void(*pFunc)(float))
{
	PANIMATIONCALLBACK	pCallback = new ANIMATIONCALLBACK;

	// �������� ������ ������ �������� �����ش�. ������ * �ð����� + ���۽ð��� �ؼ� ���� �ð���
	// �����ְ� 1�ʴ� ������ ���� �����־ �������� ���Ѵ�.
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
