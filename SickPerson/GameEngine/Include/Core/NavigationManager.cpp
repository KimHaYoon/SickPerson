#include "NavigationManager.h"
#include "../Scene/Scene.h"
#include "../Component/Transform.h"

Engine_USING

DEFINITION_SINGLE(CNavigationManager)

CNavigationManager::CNavigationManager()
{
}


CNavigationManager::~CNavigationManager()
{
	Safe_Delete_Map(m_mapLandScapeInfo);
}

void CNavigationManager::AddLandScapeInfo(const string & strKey, int iNumX, int iNumY,
	CScene* pScene, class CTransform* pTransform, vector<Vector3>* pvecPos)
{
	PLANDSCAPEINFO	pInfo = FindLandScapeInfo(strKey);

	if (pInfo)
		return;

	pInfo = new LANDSCAPEINFO;

	pInfo->iNumX = iNumX;
	pInfo->iNumY = iNumY;
	pInfo->pvecPos = pvecPos;
	pInfo->pScene = pScene;
	pInfo->pTransform = pTransform;

	vector<Vector3>::iterator	iter = pvecPos->begin();
	vector<Vector3>::iterator	iterEnd = pvecPos->end();

	pInfo->vMin = *iter;
	pInfo->vMax = *iter;

	for (iter = pvecPos->begin() + 1; iter != iterEnd; ++iter)
	{
		if (pInfo->vMin.x > (*iter).x)
			pInfo->vMin.x = (*iter).x;

		if (pInfo->vMin.y > (*iter).y)
			pInfo->vMin.y = (*iter).y;

		if (pInfo->vMin.z > (*iter).z)
			pInfo->vMin.z = (*iter).z;

		if (pInfo->vMax.x < (*iter).x)
			pInfo->vMax.x = (*iter).x;

		if (pInfo->vMax.y < (*iter).y)
			pInfo->vMax.y = (*iter).y;

		if (pInfo->vMax.z < (*iter).z)
			pInfo->vMax.z = (*iter).z;
	}

	m_mapLandScapeInfo.insert(make_pair(strKey, pInfo));
}

void CNavigationManager::DeleteLandScapeInfo(CScene * pScene)
{
	unordered_map<string, PLANDSCAPEINFO>::iterator	iter;
	unordered_map<string, PLANDSCAPEINFO>::iterator	iterEnd = m_mapLandScapeInfo.end();

	for (iter = m_mapLandScapeInfo.begin(); iter != iterEnd;)
	{
		if (iter->second->pScene == pScene)
		{
			SAFE_DELETE(iter->second);
			iter = m_mapLandScapeInfo.erase(iter);
			iterEnd = m_mapLandScapeInfo.end();
		}

		else
			++iter;
	}
}

PLANDSCAPEINFO CNavigationManager::FindLandScapeInfo(const string & strKey)
{
	unordered_map<string, PLANDSCAPEINFO>::iterator	iter = m_mapLandScapeInfo.find(strKey);

	if (iter == m_mapLandScapeInfo.end())
		return NULL;

	return iter->second;
}

PLANDSCAPEINFO CNavigationManager::FindLandScapeInfo(const Vector3 & vPos)
{
	unordered_map<string, PLANDSCAPEINFO>::iterator	iter;
	unordered_map<string, PLANDSCAPEINFO>::iterator	iterEnd = m_mapLandScapeInfo.end();

	for (iter = m_mapLandScapeInfo.begin(); iter != iterEnd; ++iter)
	{
		if (iter->second->vMin <= vPos && vPos <= iter->second->vMax)
			return iter->second;
	}

	return NULL;
}

float CNavigationManager::GetY(const Vector3& vPos)
{
	// ���� ���� ������ ���´�.
	PLANDSCAPEINFO	pInfo = FindLandScapeInfo(vPos);

	if (!pInfo)
		return 0.f;

	// ���� ������ ���� ��� ���� ��ġ�� ������ ��� �簢���� �����ϴ�����
	// ���´�.
	Vector3	vScale = pInfo->pTransform->GetWorldScale();

	float	fWidth = (*pInfo->pvecPos)[1].x - (*pInfo->pvecPos)[0].x;
	float	fHeight = (*pInfo->pvecPos)[0].z - (*pInfo->pvecPos)[pInfo->iNumX].z;

	// ���� ������ �ϳ��� Cell ũ�⸦ ���Ѵ�.
	fWidth *= vScale.x;
	fHeight *= vScale.z;

	// ���� �������� ������ �����ش�.
	Vector3	vLandScapePos = pInfo->pTransform->GetWorldPos();

	Vector3	vOriginPos = vPos;

	vOriginPos -= vLandScapePos;

	// ��ĭ�� ũ�⸦ 1, 1 ũ��� ������ش�.
	vOriginPos.x /= fWidth;
	vOriginPos.z /= fHeight;

	// ����ó��. x ������ z ���� ������ ������ ��� ����ó���Ѵ�.
	if (vOriginPos.x <= 0 || vOriginPos.x >= pInfo->iNumX ||
		vOriginPos.z <= 0 || vOriginPos.z >= pInfo->iNumY)
		return 0.f;

	// ��� �簢���� ��ġ�� �ִ��� �ε����� ���Ѵ�.
	int	xIdx = (int)vOriginPos.x;
	int	zIdx = pInfo->iNumY - 1 - (int)vOriginPos.z - 1;

	int	iIndex = zIdx * pInfo->iNumX + xIdx;

	float	fLandScapeY[4] = {};
	fLandScapeY[0] = (*pInfo->pvecPos)[iIndex].y;
	fLandScapeY[1] = (*pInfo->pvecPos)[iIndex + 1].y;
	fLandScapeY[2] = (*pInfo->pvecPos)[iIndex + pInfo->iNumX].y;
	fLandScapeY[3] = (*pInfo->pvecPos)[iIndex + pInfo->iNumX + 1].y;

	float	fX, fY;
	fX = (vPos.x - (*pInfo->pvecPos)[iIndex].x) / fWidth;
	fY = ((*pInfo->pvecPos)[iIndex].z - vPos.z) / fHeight;

	// ���� �ﰢ���� ������ ���
	if (fX >= fY)
		return fLandScapeY[0] + (fLandScapeY[1] - fLandScapeY[0]) * fX + (fLandScapeY[3] - fLandScapeY[1]) * fY;

	return fLandScapeY[0] + (fLandScapeY[3] - fLandScapeY[2]) * fX + (fLandScapeY[2] - fLandScapeY[0]) * fY;
}
