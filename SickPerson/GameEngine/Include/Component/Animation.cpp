#include "Animation.h"
#include "AnimationClip.h"
#include "../Device.h"
#include "../Core/PathManager.h"
#include "../GameObject/GameObject.h"
#include "../Resources/FbxLoader.h"

Engine_USING

CAnimation::CAnimation()
{
	// 기본 채널은 2개를 지원한다.
	m_vecChannel.resize(2);
	m_fFrameTime = 0.f;
	m_pBoneTex = NULL;
	m_bEnd = false;
	m_iCurChannel = 0;
	m_iFrameMode = 0;
	m_iNextChannel = -1;
	m_fAnimationGlobalTime = 0.f;
	m_fClipProgress = 0.f;
	m_fChangeTime = 0.f;
	m_fChangeLimitTime = 0.25f;
	m_pBoneRV = NULL;
	m_pLastAddClip = NULL;

	SetTag("Animation");
	SetTypeName("CAnimation");
	SetTypeID<CAnimation>();
	m_eType = CT_ANIMATION;
}

CAnimation::CAnimation(const CAnimation & animation) :
	CComponent(animation)
{
	m_bEnd = false;
	m_pLastAddClip = NULL;
	m_iFrameMode = animation.m_iFrameMode;
	m_fFrameTime = animation.m_fFrameTime;
	m_iCurChannel = 0;
	m_iNextChannel = -1;
	m_fAnimationGlobalTime = 0.f;
	m_fClipProgress = 0.f;
	m_fChangeTime = 0.f;
	m_fChangeLimitTime = animation.m_fChangeLimitTime;

	for (size_t i = 0; i < animation.m_vecBones.size(); ++i)
	{
		PBONE	pBone = new BONE;

		*pBone = *animation.m_vecBones[i];

		pBone->matOffset = new Matrix;
		pBone->matBone = new Matrix;

		*pBone->matOffset = *animation.m_vecBones[i]->matOffset;
		*pBone->matBone = *animation.m_vecBones[i]->matBone;

		m_vecBones.push_back(pBone);
	}

	m_vecChannel.resize(2);

	unordered_map<string, CAnimationClip*>::const_iterator	iter;
	unordered_map<string, CAnimationClip*>::const_iterator	iterEnd = animation.m_mapClip.end();

	for (iter = animation.m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		CAnimationClip*	pClip = iter->second->Clone();

		m_mapClip.insert(make_pair(iter->first, pClip));
	}

	m_pBoneTex = NULL;
	CreateBoneTexture();

	m_strCurrentClip = animation.m_strCurrentClip;
	m_strDefaultClip = animation.m_strDefaultClip;

	m_vecChannel[m_iCurChannel].pClip = FindClip(m_strCurrentClip);
}

CAnimation::~CAnimation()
{
	for (size_t i = 0; i < m_vecBones.size(); ++i)
	{
		SAFE_DELETE(m_vecBones[i]->matBone);
		SAFE_DELETE(m_vecBones[i]->matOffset);
		SAFE_DELETE(m_vecBones[i]);
	}

	m_vecBones.clear();

	SAFE_RELEASE(m_pBoneRV);
	SAFE_RELEASE(m_pBoneTex);
	Safe_Release_Map(m_mapClip);
}

CAnimationClip * CAnimation::FindClip(const string & strKey)
{
	unordered_map<string, class CAnimationClip*>::iterator	iter = m_mapClip.find(strKey);

	if (iter == m_mapClip.end())
		return NULL;

	return iter->second;
}

bool CAnimation::IsAnimationEnd() const
{
	return m_bEnd;
}

CAnimationClip * CAnimation::GetCurrentClip() const
{
	return m_vecChannel[m_iCurChannel].pClip;
}

CAnimationClip * CAnimation::GetLastAddClip() const
{
	if (m_pLastAddClip)
		m_pLastAddClip->AddRef();

	return m_pLastAddClip;
}

const unordered_map<string, class CAnimationClip*>* CAnimation::GetAllClip() const
{
	return &m_mapClip;
}

void CAnimation::AddClipCallback(const string & strName, int iFrame, void(*pFunc)(float))
{
	CAnimationClip*	pClip = FindClip(strName);

	if (!pClip)
		return;

	pClip->AddCallback(iFrame, pFunc);
}

void CAnimation::AddClipCallback(const string & strName, float fProgress, void(*pFunc)(float))
{
	CAnimationClip*	pClip = FindClip(strName);

	if (!pClip)
		return;

	pClip->AddCallback(fProgress, pFunc);
}

void CAnimation::AddBone(PBONE pBone)
{
	m_vecBones.push_back(pBone);
}

void CAnimation::AddClip(ANIMATION_OPTION eOption, _tagFbxAnimationClip* pClip)
{
	CAnimationClip*	pAniClip = FindClip(pClip->strName);

	if (pAniClip)
	{
		assert(false);
		return;
	}
	pAniClip = new CAnimationClip;

	pAniClip->SetClipInfo(eOption, pClip);

	if (!pAniClip->Init())
	{
		SAFE_RELEASE(pAniClip);
		return;
	}

	switch (pClip->eTimeMode)
	{
	case FbxTime::eFrames24:
		m_iFrameMode = 24;
		break;
	case FbxTime::eFrames30:
		m_iFrameMode = 30;
		break;
	case FbxTime::eFrames60:
		m_iFrameMode = 60;
		break;
	}

	pAniClip->m_iAnimationLimitFrame = m_iFrameMode;
	m_fFrameTime = 1.f / m_iFrameMode;

	if (m_mapClip.empty())
	{
		SetDefaultClip(pClip->strName);
		SetCurrentClip(pClip->strName);

		m_vecChannel[m_iCurChannel].pClip = pAniClip;
	}

	m_mapClip.insert(make_pair(pClip->strName, pAniClip));

	m_pLastAddClip = pAniClip;
}

void CAnimation::AddClip(const string & strKey, ANIMATION_OPTION eOption, int iAnimationLimitFrame,
	int iStartFrame, int iEndFrame, float fStartTime, float fEndTime)
{
	CAnimationClip*	pAniClip = FindClip(strKey);

	if (pAniClip)
	{
		assert(false);
		return;
	}

	m_iFrameMode = iAnimationLimitFrame;

	pAniClip = new CAnimationClip;

	pAniClip->SetClipInfo(strKey, eOption, iAnimationLimitFrame,
		iStartFrame, iEndFrame, fStartTime, fEndTime);

	if (!pAniClip->Init())
	{
		SAFE_RELEASE(pAniClip);
		return;
	}

	pAniClip->m_iAnimationLimitFrame = m_iFrameMode;
	m_fFrameTime = 1.f / m_iFrameMode;

	if (m_mapClip.empty())
	{
		SetDefaultClip(strKey);
		SetCurrentClip(strKey);

		m_vecChannel[m_iCurChannel].pClip = pAniClip;
	}

	m_mapClip.insert(make_pair(strKey, pAniClip));

	m_pLastAddClip = pAniClip;
}

void CAnimation::AddClip(const string & strKey, ANIMATION_OPTION eOption, int iStartFrame,
	int iEndFrame)
{
	CAnimationClip*	pAniClip = FindClip(strKey);

	if (pAniClip)
	{
		assert(false);
		return;
	}

	pAniClip = new CAnimationClip;

	pAniClip->m_iAnimationLimitFrame = m_iFrameMode;
	m_fFrameTime = 1.f / m_iFrameMode;

	float	fStartTime, fEndTime;
	fStartTime = iStartFrame * m_fFrameTime;
	fEndTime = iEndFrame * m_fFrameTime;

	pAniClip->SetClipInfo(strKey, eOption, m_iFrameMode,
		iStartFrame, iEndFrame, fStartTime, fEndTime);

	if (!pAniClip->Init())
	{
		SAFE_RELEASE(pAniClip);
		return;
	}


	if (m_mapClip.empty())
	{
		SetDefaultClip(strKey);
		SetCurrentClip(strKey);

		m_vecChannel[m_iCurChannel].pClip = pAniClip;
	}

	m_mapClip.insert(make_pair(strKey, pAniClip));

	m_pLastAddClip = pAniClip;
}

void CAnimation::AddClip(const string & strKey, CAnimationClip * pClip)
{
	CAnimationClip*	pClone = pClip->Clone();

	if (m_mapClip.empty())
	{
		SetDefaultClip(strKey);
		SetCurrentClip(strKey);

		m_vecChannel[m_iCurChannel].pClip = pClone;
	}

	m_mapClip.insert(make_pair(strKey, pClone));

	m_pLastAddClip = pClone;
}

void CAnimation::AddClip(const wchar_t * pFullPath)
{
	char	strFullPath[MAX_PATH] = {};

	WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, strFullPath, lstrlen(pFullPath),
		0, 0);

	AddClip(strFullPath);
}

void CAnimation::AddClip(const char * pFullPath)
{
	CFbxLoader	loader;

	loader.LoadAnimation(pFullPath);

	const vector<PFBXANIMATIONCLIP>*	pClips = loader.GetClips();

	vector<PFBXANIMATIONCLIP>::const_iterator	iter;
	vector<PFBXANIMATIONCLIP>::const_iterator	iterEnd = pClips->end();

	for (iter = pClips->begin(); iter != iterEnd; ++iter)
	{
		AddClip(AO_LOOP, *iter);
	}
}

bool CAnimation::CreateBoneTexture()
{
	SAFE_RELEASE(m_pBoneTex);
	D3D11_TEXTURE2D_DESC	tDesc = {};
	tDesc.ArraySize = 1;
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tDesc.Height = 1;
	tDesc.Width = m_vecBones.size() * 4;
	tDesc.MipLevels = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.SampleDesc.Count = 1;

	if (FAILED(DEVICE->CreateTexture2D(&tDesc, NULL, &m_pBoneTex)))
		return false;

	if (FAILED(DEVICE->CreateShaderResourceView(m_pBoneTex, NULL, &m_pBoneRV)))
		return false;

	return true;
}

void CAnimation::SetDefaultClip(const string & strClip)
{
	m_strDefaultClip = strClip;
}

void CAnimation::SetCurrentClip(const string & strClip)
{
	m_iCurChannel = 0;
	m_strCurrentClip = strClip;
}

bool CAnimation::ChangeClip(const string & strClip)
{
	if (m_strCurrentClip == strClip)
		return false;

	CAnimationClip*	pClip = FindClip(strClip);

	if (pClip == m_vecChannel[m_iCurChannel].pClip)
	{
		m_iNextChannel = -1;
		return false;
	}

	else if (!pClip)
	{
		m_iNextChannel = -1;
		return false;
	}

	m_iNextChannel = m_iCurChannel == 0 ? 1 : 0;

	m_vecChannel[m_iNextChannel].pClip = pClip;
	m_vecChannel[m_iNextChannel].fWeight = 1.f;
	m_strCurrentClip = strClip;
	//pClip->Release();

	return true;
}

ID3D11ShaderResourceView * CAnimation::GetBoneTexture() const
{
	return m_pBoneRV;
}

bool CAnimation::Save(const wchar_t * pFileName, const string & strPathKey)
{
	char	strFileName[MAX_PATH] = {};

	WideCharToMultiByte(CP_ACP, 0, pFileName, -1, strFileName, lstrlen(pFileName),
		NULL, NULL);

	return Save(strFileName, strPathKey);
}

bool CAnimation::Save(const char * pFileName, const string & strPathKey)
{
	const char*	pPath = GET_SINGLE(CPathManager)->FindPathToMultiByte(strPathKey);

	string	strFullPath;

	if (pPath)
		strFullPath = pPath;

	strFullPath += pFileName;

	return SaveFromFullPath(strFullPath.c_str());
}

bool CAnimation::SaveFromFullPath(const wchar_t * pFullPath)
{
	char	strFullPath[256] = {};

	WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, strFullPath,
		lstrlen(pFullPath), 0, 0);

	return SaveFromFullPath(strFullPath);
}

bool CAnimation::SaveFromFullPath(const char * pFullPath)
{
	FILE*	pFile = NULL;

	fopen_s(&pFile, pFullPath, "wb");

	if (!pFile)
		return false;

	// ===================== 본 정보 저장 =====================
	size_t	iCount = m_vecBones.size();

	fwrite(&iCount, sizeof(size_t), 1, pFile);

	size_t	iLength = 0;

	for (size_t i = 0; i < iCount; ++i)
	{
		iLength = m_vecBones[i]->strName.length();
		fwrite(&iLength, sizeof(size_t), 1, pFile);
		fwrite(m_vecBones[i]->strName.c_str(), sizeof(char), iLength, pFile);

		fwrite(&m_vecBones[i]->iDepth, sizeof(int), 1, pFile);
		fwrite(&m_vecBones[i]->iParentIndex, sizeof(int), 1, pFile);
		fwrite(&m_vecBones[i]->matOffset->mat, sizeof(XMMATRIX), 1, pFile);
		fwrite(&m_vecBones[i]->matBone->mat, sizeof(XMMATRIX), 1, pFile);
	}

	fwrite(&m_iFrameMode, sizeof(int), 1, pFile);
	fwrite(&m_fFrameTime, sizeof(float), 1, pFile);
	fwrite(&m_fAnimationGlobalTime, sizeof(float), 1, pFile);
	fwrite(&m_fClipProgress, sizeof(float), 1, pFile);
	fwrite(&m_fChangeTime, sizeof(float), 1, pFile);
	fwrite(&m_fChangeLimitTime, sizeof(float), 1, pFile);

	// 애니메이션 클립정보를 저장한다.
	iCount = m_mapClip.size();

	fwrite(&iCount, sizeof(size_t), 1, pFile);

	iLength = m_strDefaultClip.length();
	fwrite(&iLength, sizeof(size_t), 1, pFile);
	fwrite(m_strDefaultClip.c_str(), sizeof(char), iLength, pFile);

	iLength = m_strCurrentClip.length();
	fwrite(&iLength, sizeof(size_t), 1, pFile);
	fwrite(m_strCurrentClip.c_str(), sizeof(char), iLength, pFile);

	unordered_map<string, CAnimationClip*>::iterator	iter;
	unordered_map<string, CAnimationClip*>::iterator	iterEnd = m_mapClip.end();

	for (iter = m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		// 애니메이션 클립 키를 저장한다.
		iLength = iter->first.length();
		fwrite(&iLength, sizeof(size_t), 1, pFile);
		fwrite(iter->first.c_str(), sizeof(char), iLength, pFile);

		iter->second->Save(pFile);
	}

	fclose(pFile);

	return true;
}

bool CAnimation::Load(const wchar_t * pFileName, const string & strPathKey)
{
	char	strFileName[MAX_PATH] = {};

	WideCharToMultiByte(CP_ACP, 0, pFileName, -1, strFileName, lstrlen(pFileName),
		NULL, NULL);

	return Load(strFileName, strPathKey);
}

bool CAnimation::Load(const char * pFileName, const string & strPathKey)
{
	const char*	pPath = GET_SINGLE(CPathManager)->FindPathToMultiByte(strPathKey);

	string	strFullPath;

	if (pPath)
		strFullPath = pPath;

	strFullPath += pFileName;

	return LoadFromFullPath(strFullPath.c_str());
}

bool CAnimation::LoadFromFullPath(const wchar_t * pFullPath)
{
	char	strFullPath[256] = {};

	WideCharToMultiByte(CP_ACP, 0, pFullPath, -1, strFullPath,
		lstrlen(pFullPath), 0, 0);

	return LoadFromFullPath(strFullPath);
}

bool CAnimation::LoadFromFullPath(const char * pFullPath)
{
	FILE*	pFile = NULL;

	fopen_s(&pFile, pFullPath, "rb");

	if (!pFile)
		return false;

	// ===================== 본 정보 불러오기 =====================
	size_t	iCount = 0;

	fread(&iCount, sizeof(size_t), 1, pFile);

	size_t	iLength = 0;

	for (size_t i = 0; i < iCount; ++i)
	{
		PBONE	pBone = new BONE;
		char	strBoneName[256] = {};
		fread(&iLength, sizeof(size_t), 1, pFile);
		fread(strBoneName, sizeof(char), iLength, pFile);
		pBone->strName = strBoneName;

		fread(&pBone->iDepth, sizeof(int), 1, pFile);
		fread(&pBone->iParentIndex, sizeof(int), 1, pFile);

		pBone->matOffset = new Matrix;
		pBone->matBone = new Matrix;

		fread(&pBone->matOffset->mat, sizeof(XMMATRIX), 1, pFile);
		fread(&pBone->matBone->mat, sizeof(XMMATRIX), 1, pFile);

		m_vecBones.push_back(pBone);
	}

	fread(&m_iFrameMode, sizeof(int), 1, pFile);
	fread(&m_fFrameTime, sizeof(float), 1, pFile);
	fread(&m_fAnimationGlobalTime, sizeof(float), 1, pFile);
	fread(&m_fClipProgress, sizeof(float), 1, pFile);
	fread(&m_fChangeTime, sizeof(float), 1, pFile);
	fread(&m_fChangeLimitTime, sizeof(float), 1, pFile);

	// 애니메이션 클립정보를 저장한다.
	fread(&iCount, sizeof(size_t), 1, pFile);

	char	strName[256] = {};
	fread(&iLength, sizeof(size_t), 1, pFile);
	fread(strName, sizeof(char), iLength, pFile);
	m_strDefaultClip = strName;

	memset(strName, 0, sizeof(char) * 256);
	fread(&iLength, sizeof(size_t), 1, pFile);
	fread(strName, sizeof(char), iLength, pFile);
	m_strCurrentClip = strName;

	for (size_t i = 0; i < iCount; ++i)
	{
		CAnimationClip*	pClip = new CAnimationClip;

		pClip->Init();

		memset(strName, 0, sizeof(char) * 256);
		fread(&iLength, sizeof(size_t), 1, pFile);
		fread(strName, sizeof(char), iLength, pFile);

		pClip->Load(pFile);

		m_mapClip.insert(make_pair(strName, pClip));

		m_pLastAddClip = pClip;
	}

	fclose(pFile);

	m_iCurChannel = 0;
	m_iNextChannel = -1;
	m_bEnd = false;
	m_fAnimationGlobalTime = 0.f;
	m_fClipProgress = 0.f;
	m_fChangeTime = 0.f;

	CreateBoneTexture();

	SetDefaultClip(m_strDefaultClip);
	SetCurrentClip(m_strCurrentClip);

	m_vecChannel[m_iCurChannel].fWeight = 1.f;
	m_vecChannel[m_iCurChannel].pClip = FindClip(m_strCurrentClip);

	return true;
}

bool CAnimation::ModifyClip(const string & strKey, const string & strChangeKey,
	ANIMATION_OPTION eOption, int iStartFrame, int iEndFrame)
{
	CAnimationClip*	pClip = FindClip(strKey);

	if (!pClip)
		return false;

	m_mapClip.erase(strKey);

	int	iLength = iEndFrame - iStartFrame;
	pClip->SetClipInfo(strChangeKey, eOption, m_iFrameMode,
		iStartFrame, iEndFrame, 0.f, iLength / (float)m_iFrameMode);

	m_mapClip.insert(make_pair(strChangeKey, pClip));

	if (m_strDefaultClip == strKey)
		m_strDefaultClip = strChangeKey;

	if (m_strCurrentClip == strKey)
		m_strCurrentClip = strChangeKey;

	return true;
}

bool CAnimation::DeleteClip(const string & strKey)
{
	unordered_map<string, CAnimationClip*>::iterator	iter = m_mapClip.find(strKey);

	if (iter == m_mapClip.end())
		return false;

	SAFE_RELEASE(iter->second);

	if (strKey == m_strDefaultClip)
	{
		if (!m_mapClip.empty())
			SetDefaultClip(m_mapClip.begin()->first);
	}

	if (strKey == m_strCurrentClip)
	{
		m_vecChannel[m_iCurChannel].pClip = NULL;
		m_vecChannel[m_iCurChannel].fWeight = 1.f;
		SetCurrentClip(m_strDefaultClip);

		m_vecChannel[m_iCurChannel].pClip = FindClip(m_strDefaultClip);
		m_vecChannel[m_iCurChannel].fWeight = 1.f;
	}

	m_mapClip.erase(iter);

	return true;
}

bool CAnimation::ReturnDefaultClip()
{
	ChangeClip(m_strDefaultClip);

	return true;
}

bool CAnimation::Init()
{
	return true;
}

void CAnimation::Input(float fTime)
{
}

int CAnimation::Update(float fTime)
{
	if (m_mapClip.empty())
		return 0;

	m_bEnd = false;
	// 본정보를 업데이트한다.
	vector<Matrix>	vecBones;

	// 모션이 변할때
	if (m_iNextChannel != -1)
	{
		m_fChangeTime += fTime;

		bool	bChange = false;
		if (m_fChangeTime >= m_fChangeLimitTime)
		{
			m_fChangeTime = m_fChangeLimitTime;
			bChange = true;
		}

		float	fAnimationTime = m_fAnimationGlobalTime +
			m_vecChannel[m_iCurChannel].pClip->m_tInfo.fStartTime;

		vecBones.reserve(m_vecBones.size());

		// 본 수만큼 반복한다.
		for (size_t i = 0; i < m_vecBones.size(); ++i)
		{
			// 키프레임이 없을 경우
			if (m_vecChannel[m_iCurChannel].pClip->IsEmptyFrame(i))
			{
				vecBones.push_back(*m_vecBones[i]->matBone);
				continue;
			}

			int	iFrameIndex = m_vecChannel[m_iCurChannel].iChangeFrame;
			int	iNextFrameIndex = m_vecChannel[m_iNextChannel].pClip->m_tInfo.iStartFrame;

			const PKEYFRAME pCurKey = m_vecChannel[m_iCurChannel].pClip->GetKeyFrame(i, iFrameIndex);
			const PKEYFRAME pNextKey = m_vecChannel[m_iNextChannel].pClip->GetKeyFrame(i, iNextFrameIndex);

			float	fPercent = m_fChangeTime / m_fChangeLimitTime;

			XMVECTOR	vS = XMVectorLerp(pCurKey->vScale.Convert(),
				pNextKey->vScale.Convert(), fPercent);
			XMVECTOR	vT = XMVectorLerp(pCurKey->vPos.Convert(),
				pNextKey->vPos.Convert(), fPercent);
			XMVECTOR	vR = XMQuaternionSlerp(pCurKey->vRot.Convert(),
				pNextKey->vRot.Convert(), fPercent);

			XMVECTOR	vZero = XMVectorSet(0.f, 0.f, 0.f, 1.f);

			Matrix	matBone = XMMatrixAffineTransformation(vS, vZero,
				vR, vT);

			matBone = *m_vecBones[i]->matOffset *
				matBone;

			vecBones.push_back(matBone);
		}

		if (bChange)
		{
			m_iCurChannel = m_iNextChannel;
			m_iNextChannel = -1;
			m_fAnimationGlobalTime = 0.f;
			m_fChangeTime = 0.f;
		}
	}

	// 기존 모션이 계속 동작될때
	else
	{
		m_fAnimationGlobalTime += fTime;
		m_fClipProgress = m_fAnimationGlobalTime / m_vecChannel[m_iCurChannel].pClip->m_tInfo.fTimeLength;

		while (m_fAnimationGlobalTime >= m_vecChannel[m_iCurChannel].pClip->m_tInfo.fTimeLength)
		{
			m_fAnimationGlobalTime -= m_vecChannel[m_iCurChannel].pClip->m_tInfo.fTimeLength;
			m_bEnd = true;

			for (size_t i = 0; i < m_vecChannel[m_iCurChannel].pClip->m_tInfo.vecCallback.size();
				++i)
			{
				m_vecChannel[m_iCurChannel].pClip->m_tInfo.vecCallback[i]->bCall = false;
			}
		}

		float	fAnimationTime = m_fAnimationGlobalTime +
			m_vecChannel[m_iCurChannel].pClip->m_tInfo.fStartTime;

		int	iStartFrame = m_vecChannel[m_iCurChannel].pClip->m_tInfo.iStartFrame;
		int	iEndFrame = m_vecChannel[m_iCurChannel].pClip->m_tInfo.iEndFrame;

		vecBones.reserve(m_vecBones.size());

		int	iFrameIndex = (int)(fAnimationTime * m_iFrameMode);

		if (m_bEnd)
		{
			switch (m_vecChannel[m_iCurChannel].pClip->m_tInfo.eOption)
			{
			case AO_LOOP:
				iFrameIndex = iStartFrame;
				break;
			case AO_ONCE_RETURN:
				ChangeClip(m_strDefaultClip);
				break;
			case AO_ONCE_DESTROY:
				m_pGameObject->Die();
				break;
			}
		}

		else
		{
			int	iNextFrameIndex = iFrameIndex + 1;

			m_vecChannel[m_iCurChannel].iChangeFrame = iFrameIndex;

			if (iNextFrameIndex > iEndFrame)
				iNextFrameIndex = iStartFrame;

			// 본 수만큼 반복한다.
			for (size_t i = 0; i < m_vecBones.size(); ++i)
			{
				// 키프레임이 없을 경우
				if (m_vecChannel[m_iCurChannel].pClip->IsEmptyFrame(i))
				{
					vecBones.push_back(*m_vecBones[i]->matBone);
					continue;
				}

				const PKEYFRAME pCurKey = m_vecChannel[m_iCurChannel].pClip->GetKeyFrame(i, iFrameIndex);
				const PKEYFRAME pNextKey = m_vecChannel[m_iCurChannel].pClip->GetKeyFrame(i, iNextFrameIndex);

				// 현재 프레임의 시간을 얻어온다.
				double	 dFrameTime = pCurKey->dTime;

				float	fPercent = (fAnimationTime - dFrameTime) / m_fFrameTime;

				XMVECTOR	vS = XMVectorLerp(pCurKey->vScale.Convert(),
					pNextKey->vScale.Convert(), fPercent);
				XMVECTOR	vT = XMVectorLerp(pCurKey->vPos.Convert(),
					pNextKey->vPos.Convert(), fPercent);
				XMVECTOR	vR = XMQuaternionSlerp(pCurKey->vRot.Convert(),
					pNextKey->vRot.Convert(), fPercent);

				XMVECTOR	vZero = XMVectorSet(0.f, 0.f, 0.f, 1.f);

				Matrix	matBone = XMMatrixAffineTransformation(vS, vZero,
					vR, vT);

				matBone = *m_vecBones[i]->matOffset * matBone;

				vecBones.push_back(matBone);
			}
		}
	}

	if (!m_bEnd)
	{
		D3D11_MAPPED_SUBRESOURCE	tMap = {};
		CONTEXT->Map(m_pBoneTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);

		memcpy(tMap.pData, &vecBones[0], sizeof(Matrix) * vecBones.size());

		CONTEXT->Unmap(m_pBoneTex, 0);
	}

	return 0;
}

int CAnimation::LateUpdate(float fTime)
{
	return 0;
}

void CAnimation::Collision(float fTime)
{
}

void CAnimation::Render(float fTime)
{
}

CAnimation * CAnimation::Clone()
{
	return new CAnimation(*this);
}
