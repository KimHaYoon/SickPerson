#include "CollisionManager.h"
#include "Input.h"
#include "../Device.h"
#include "../Component/ColliderRect.h"
#include "../Component/ColliderPoint.h"
#include "../Component/ColliderRay.h"
#include "../Component/UI.h"
#include "../Component/Camera.h"
#include "../Component/Renderer.h"
#include "../Component/Transform.h"
#include "../GameObject/GameObject.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../Resources/Mesh.h"

Engine_USING

DEFINITION_SINGLE(CCollisionManager)

CCollisionManager::CCollisionManager()
{
	m_iMouseCollCount = 0;
	m_iMouseCollCapasity = 1000;
	m_pMouseCollArray = new CCollider*[m_iMouseCollCapasity];
}

CCollisionManager::~CCollisionManager()
{
	SAFE_DELETE_ARRAY(m_pMouseCollArray);
	Safe_Delete_Map(m_mapCollisionGroup);
}

bool CCollisionManager::CreateGroup(const string & strKey, int iSectionX, int iSectionY,
	int iSectionZ, const Vector3& vSize, COLLLIST_TYPE eType)
{
	PCOLLISIONGROUP	pGroup = FindGroup(strKey);

	if (pGroup)
		return false;

	pGroup = new COLLISIONGROUP;

	pGroup->iSectionX = iSectionX;
	pGroup->iSectionY = iSectionY;
	pGroup->iSectionZ = iSectionZ;
	pGroup->vSize = vSize;

	Vector3	vGroupMin = Vector3(vSize.x * iSectionX / -2.f, vSize.y * iSectionY / -2.f,
		vSize.z * iSectionZ / -2.f);
	Vector3	vGroupMax = Vector3(vSize.x * iSectionX / 2.f, vSize.y * iSectionY / 2.f,
		vSize.z * iSectionZ / 2.f);
	pGroup->vMin = vGroupMin;
	pGroup->vMax = vGroupMax;
	pGroup->iSectionMax = iSectionX * iSectionY * iSectionZ;

	pGroup->pSectionList = new PCOLLISIONSECTION[pGroup->iSectionMax];

	for (int i = 0; i < iSectionZ; ++i)
	{
		for (int j = 0; j < iSectionY; ++j)
		{
			for (int k = 0; k < iSectionX; ++k)
			{
				int iIndex = i * iSectionX * iSectionY + j * iSectionX + k;

				pGroup->pSectionList[iIndex] = new COLLISIONSECTION;
			}
		}
	}

	m_mapCollisionGroup.insert(make_pair(strKey, pGroup));

	return true;
}

bool CCollisionManager::ModifyGroup(const string & strKey, int iSectionX, int iSectionY,
	int iSectionZ, const Vector3 & vSize, COLLLIST_TYPE eType)
{
	PCOLLISIONGROUP	pGroup = FindGroup(strKey);

	if (!pGroup)
		return false;

	EraseGroup(strKey);

	pGroup = new COLLISIONGROUP;

	pGroup->iSectionX = iSectionX;
	pGroup->iSectionY = iSectionY;
	pGroup->iSectionZ = iSectionZ;
	pGroup->vSize = vSize;

	Vector3	vGroupMin = Vector3(vSize.x * iSectionX / -2.f, vSize.y * iSectionY / -2.f,
		vSize.z * iSectionZ / -2.f);
	Vector3	vGroupMax = Vector3(vSize.x * iSectionX / 2.f, vSize.y * iSectionY / 2.f,
		vSize.z * iSectionZ / 2.f);
	pGroup->vMin = vGroupMin;
	pGroup->vMax = vGroupMax;
	pGroup->iSectionMax = iSectionX * iSectionY * iSectionZ;

	pGroup->pSectionList = new PCOLLISIONSECTION[pGroup->iSectionMax];

	for (int i = 0; i < iSectionZ; ++i)
	{
		for (int j = 0; j < iSectionY; ++j)
		{
			for (int k = 0; k < iSectionX; ++k)
			{
				int iIndex = i * iSectionX * iSectionY + j * iSectionX + k;

				pGroup->pSectionList[iIndex] = new COLLISIONSECTION;
			}
		}
	}

	m_mapCollisionGroup.insert(make_pair(strKey, pGroup));

	return true;
}

bool CCollisionManager::EraseGroup(const string & strKey)
{
	unordered_map<string, PCOLLISIONGROUP>::iterator	iter = m_mapCollisionGroup.find(strKey);

	if (iter == m_mapCollisionGroup.end())
		return false;

	SAFE_DELETE(iter->second);

	m_mapCollisionGroup.erase(iter);

	return true;
}

PCOLLISIONGROUP CCollisionManager::FindGroup(const string & strKey)
{
	unordered_map<string, PCOLLISIONGROUP>::iterator	iter = m_mapCollisionGroup.find(strKey);

	if (iter == m_mapCollisionGroup.end())
		return NULL;

	return iter->second;
}

bool CCollisionManager::Init()
{
	CreateGroup("Default", 5, 5, 5, Vector3(10.f, 10.f, 10.f));
	CreateGroup("UI", 5, 5, 1,
		Vector3(DEVICE_RESOLUTION.iWidth / 5.f, DEVICE_RESOLUTION.iHeight / 5.f, 1.f), CLT_2D);

	PCOLLISIONGROUP	pUIGroup = FindGroup("UI");

	pUIGroup->vMin = Vector3(0.f, 0.f, 0.f);
	pUIGroup->vMax = Vector3(DEVICE_RESOLUTION.iWidth,
		DEVICE_RESOLUTION.iHeight, 1.f);

	return true;
}

bool CCollisionManager::AddCollider(CGameObject * pObj)
{
	if (!pObj->CheckComponentFromType(CT_COLLIDER))
		return false;

	// 카메라를 얻어온다.
	CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CTransform*	pCameraTr = pScene->GetMainCameraTr();

	Vector3	vCameraPos = pCameraTr->GetWorldPos();

	SAFE_RELEASE(pCameraTr);

	// 충돌체 리스트를 얻어온다.
	const list<CComponent*>* pList = pObj->FindComponentsFromType(CT_COLLIDER);

	if (!pObj->CheckComponentFromType(CT_UI))
	{
		CCamera*	pCamera = pScene->GetMainCamera();

		CRenderer*	pRenderer = pObj->FindComponentFromType<CRenderer>(CT_RENDERER);

		CMesh*	pMesh = pRenderer->GetMesh();

		Vector3	vCenter = pMesh->GetCenter();
		Vector3	vLength = pMesh->GetLength();

		CTransform*	pTransform = pObj->GetTransform();

		Vector3	vScale = pTransform->GetWorldScale();

		vCenter /= vScale;
		vLength /= vScale;

		vCenter += pTransform->GetWorldPos();

		float	fRadius;
		fRadius = vLength.x > vLength.y ? vLength.x : vLength.y;
		fRadius = fRadius > vLength.z ? fRadius : vLength.z;
		fRadius /= 2.f;

		SAFE_RELEASE(pTransform);

		SAFE_RELEASE(pMesh);

		if (!pCamera->FrustumInSphere(vCenter, fRadius))
			pObj->SetCulling(true);

		else
		{
			pObj->SetCulling(false);

			CGameObject*	pMouseObj = GET_SINGLE(CInput)->GetMouseObj();
			CColliderRay*	pRay = pMouseObj->FindComponentFromTag<CColliderRay>("MouseRay");

			SAFE_RELEASE(pMouseObj);

			list<CComponent*>::const_iterator	iter;
			list<CComponent*>::const_iterator	iterEnd = pList->end();

			/*for (iter = pList->begin(); iter != iterEnd; ++iter)
			{
			CCollider*	pColl = (CCollider*)*iter;
			if (pColl->CheckCollList("MouseRay"))
			{
			pObj->OnCollisionLeave(pColl, pRay, 0.f);
			pRay->OnCollisionLeave(pRay, pColl, 0.f);

			pRay->EraseCollList(pColl);
			pColl->EraseCollList(pRay);
			}
			}*/

			SAFE_RELEASE(pRay);
		}

		SAFE_RELEASE(pCamera);
		SAFE_RELEASE(pRenderer);
	}

	SAFE_RELEASE(pScene);

	list<CComponent*>::const_iterator	iter;
	list<CComponent*>::const_iterator	iterEnd = pList->end();

	for (iter = pList->begin(); iter != iterEnd; ++iter)
	{
		CCollider*	pCollider = (CCollider*)*iter;

		if (!pObj->GetCulling() &&
			!pObj->CheckComponentFromType(CT_UI))
		{
			if (m_iMouseCollCount == m_iMouseCollCapasity)
			{
				m_iMouseCollCapasity *= 1.5f;
				CCollider**	pList = new CCollider*[m_iMouseCollCapasity];
				memcpy(pList, m_pMouseCollArray,
					sizeof(CCollider*) * m_iMouseCollCount);
				delete[]	m_pMouseCollArray;
				m_pMouseCollArray = pList;
			}

			m_pMouseCollArray[m_iMouseCollCount] = pCollider;
			++m_iMouseCollCount;
		}

		PCOLLISIONGROUP	pGroup = FindGroup(pCollider->GetCollisionGroup());
		pCollider->ClearSectionIndex();

		if (!pGroup)
		{
			assert(false);
		}

		// 어떤 영역에 속해 있는지를 구해준다.(영역 인덱스를 구해야 한다.)
		// 충돌체의 Min, Max 값을 얻어온다.
		Vector3	vColliderMin = pCollider->GetColliderMin();
		Vector3	vColliderMax = pCollider->GetColliderMax();

		// 로컬 중심의 영역을 월드공간으로 변환해준다.(카메라 더해주면 끝)
		Vector3	vSectionMin = pGroup->vMin;
		Vector3	vSectionMax = pGroup->vMax;

		if (pCollider->GetCollisionGroup() != "UI")
		{
			vSectionMin += vCameraPos;
			vSectionMax += vCameraPos;
		}

		else
		{
			int a = 10;
		}

		if (vSectionMin.x > vColliderMax.x || vSectionMax.x < vColliderMin.x ||
			vSectionMin.y > vColliderMax.y || vSectionMax.y < vColliderMin.y ||
			vSectionMin.z > vColliderMax.z || vSectionMax.z < vColliderMin.z)
			continue;

		vColliderMin -= vSectionMin;
		vColliderMax -= vSectionMin;

		// 가로, 세로 크기로 나눠준다.
		vColliderMin /= pGroup->vSize;
		vColliderMax /= pGroup->vSize;

		vector<int>	vecIndex;

		for (int z = (int)vColliderMin.z; z <= (int)vColliderMax.z; ++z)
		{
			if (z < 0 || z >= pGroup->iSectionZ)
				continue;

			for (int y = (int)vColliderMin.y; y <= (int)vColliderMax.y; ++y)
			{
				if (y < 0 || y >= pGroup->iSectionY)
					continue;

				for (int x = (int)vColliderMin.x; x <= (int)vColliderMax.x; ++x)
				{
					if (x < 0 || x >= pGroup->iSectionX)
						continue;

					int	iIndex = z * pGroup->iSectionX * pGroup->iSectionY +
						y * pGroup->iSectionX + x;

					pCollider->AddSectionIndex(iIndex);

					vecIndex.push_back(iIndex);
				}
			}
		}

		// 구해준 인덱스 섹션에 충돌체를 추가해준다.
		for (size_t i = 0; i < vecIndex.size(); ++i)
		{
			int	iIndex = vecIndex[i];
			if (pGroup->pSectionList[iIndex]->iSize == pGroup->pSectionList[iIndex]->iCapasity)
			{
				pGroup->pSectionList[iIndex]->iCapasity *= 1.5f;
				CCollider**	pArray = new CCollider*[pGroup->pSectionList[iIndex]->iCapasity];
				memcpy(pArray, pGroup->pSectionList[iIndex]->pArray,
					sizeof(CCollider*) * pGroup->pSectionList[iIndex]->iSize);

				SAFE_DELETE_ARRAY(pGroup->pSectionList[iIndex]->pArray);

				pGroup->pSectionList[iIndex]->pArray = pArray;
			}

			pGroup->pSectionList[iIndex]->pArray[pGroup->pSectionList[iIndex]->iSize] = pCollider;
			++pGroup->pSectionList[iIndex]->iSize;
		}
	}

	return true;
}

void CCollisionManager::Collision(float fTime)
{
	CGameObject*	pMouseObj = GET_SINGLE(CInput)->GetMouseObj();

	CTransform*	pMouseTr = pMouseObj->GetTransform();

	CColliderPoint*	pMousePoint = NULL;

	const list<CComponent*>*	pMouseCollList = pMouseObj->FindComponentsFromType(CT_COLLIDER);

	list<CComponent*>::const_iterator	MouseIter;
	list<CComponent*>::const_iterator	MouseIterEnd = pMouseCollList->end();

	for (MouseIter = pMouseCollList->begin(); MouseIter != MouseIterEnd; ++MouseIter)
	{
		if (((CCollider*)(*MouseIter))->GetColliderType() == CT_POINT)
			pMousePoint = (CColliderPoint*)(*MouseIter);
	}

	Vector3		vMousePos = pMouseTr->GetWorldPos();

	SAFE_RELEASE(pMouseTr);

	bool	bMouseColl = false;

	pMousePoint->ClearSectionIndex();

	// 마우스가 속해있는 2D 충돌 영역인덱스를 구해준다.
	if (vMousePos.x >= 0.f && vMousePos.x < DEVICE_RESOLUTION.iWidth &&
		vMousePos.y >= 0.f && vMousePos.y < DEVICE_RESOLUTION.iHeight)
	{
		PCOLLISIONGROUP	pGroup = FindGroup("UI");

		// 가로, 세로 크기로 나눠준다.
		vMousePos /= pGroup->vSize;

		int	x = (int)vMousePos.x;
		int	y = (int)vMousePos.y;
		int	z = (int)vMousePos.z;

		// 마우스와 UI 충돌
		int	iIndex = z * pGroup->iSectionX * pGroup->iSectionY +
			y * pGroup->iSectionX + x;

		pMousePoint->AddSectionIndex(iIndex);

		// ZOrder 기준으로 정렬한다.
		PCOLLISIONSECTION	pSection = pGroup->pSectionList[iIndex];
		int	iSectionSize = pSection->iSize;
		if (iSectionSize > 0)
		{
			sort(pSection->pArray, &pSection->pArray[iSectionSize],
				CCollisionManager::SortZOrder);
		}

		for (int i = 0; i < pSection->iSize; ++i)
		{
			CCollider*	pSrc = pSection->pArray[i];
			CCollider*	pDest = pMousePoint;

			CGameObject*	pSrcObj = pSrc->GetGameObject();
			CGameObject*	pDestObj = pDest->GetGameObject();

			if (pSrc->Collision(pDest))
			{
				bMouseColl = true;
				if (!pSrc->CheckCollList(pDest))
				{
					pSrc->AddCollList(pDest);
					pDest->AddCollList(pSrc);

					pSrcObj->OnCollisionEnter(pSrc, pDest, fTime);
					pDestObj->OnCollisionEnter(pDest, pSrc, fTime);
				}

				else
				{
					pSrcObj->OnCollision(pSrc, pDest, fTime);
					pDestObj->OnCollision(pDest, pSrc, fTime);
				}

				SAFE_RELEASE(pSrcObj);
				SAFE_RELEASE(pDestObj);

				break;
			}

			else
			{
				if (pSrc->CheckCollList(pDest))
				{
					pSrc->EraseCollList(pDest);
					pDest->EraseCollList(pSrc);

					pSrcObj->OnCollisionLeave(pSrc, pDest, fTime);
					pDestObj->OnCollisionLeave(pDest, pSrc, fTime);
				}
			}

			SAFE_RELEASE(pSrcObj);
			SAFE_RELEASE(pDestObj);
		}
	}

	// 3차원 오브젝트는 마우스와 UI  충돌이 안된 상태에서만 진행한다.
	if (!bMouseColl)
	{
		CColliderRay*	pRay = pMouseObj->FindComponentFromTag<CColliderRay>("MouseRay");

		if (m_iMouseCollCount >= 2)
		{
			sort(m_pMouseCollArray, m_pMouseCollArray + m_iMouseCollCount,
				CCollisionManager::SortZ);
		}

		for (int i = 0; i < m_iMouseCollCount; ++i)
		{
			CCollider*	pSrc = m_pMouseCollArray[i];
			CCollider*	pDest = pRay;

			CGameObject*	pSrcObj = pSrc->GetGameObject();
			CGameObject*	pDestObj = pDest->GetGameObject();

			if (pSrc->Collision(pDest))
			{
				bMouseColl = true;
				if (!pSrc->CheckCollList(pDest))
				{
					pSrc->AddCollList(pDest);
					pDest->AddCollList(pSrc);

					pSrcObj->OnCollisionEnter(pSrc, pDest, fTime);
					pDestObj->OnCollisionEnter(pDest, pSrc, fTime);
				}

				else
				{
					pSrcObj->OnCollision(pSrc, pDest, fTime);
					pDestObj->OnCollision(pDest, pSrc, fTime);
				}

				SAFE_RELEASE(pSrcObj);
				SAFE_RELEASE(pDestObj);

				break;
			}

			else
			{
				if (pSrc->CheckCollList(pDest))
				{
					pSrc->EraseCollList(pDest);
					pDest->EraseCollList(pSrc);

					pSrcObj->OnCollisionLeave(pSrc, pDest, fTime);
					pDestObj->OnCollisionLeave(pDest, pSrc, fTime);
				}
			}

			SAFE_RELEASE(pSrcObj);
			SAFE_RELEASE(pDestObj);
		}

		SAFE_RELEASE(pRay);
	}

	SAFE_RELEASE(pMouseObj);

	unordered_map<string, PCOLLISIONGROUP>::iterator	iter;
	unordered_map<string, PCOLLISIONGROUP>::iterator	iterEnd = m_mapCollisionGroup.end();

	bool	bCollision = false;

	for (iter = m_mapCollisionGroup.begin(); iter != iterEnd; ++iter)
	{
		PCOLLISIONGROUP	pGroup = iter->second;

		for (int i = 0; i < pGroup->iSectionMax; ++i)
		{
			for (int j = 0; j < pGroup->pSectionList[i]->iSize; ++j)
			{
				if (pGroup->pSectionList[i]->pArray[j]->GetTag() == "PlayerBody")
					int a = 1;
				pGroup->pSectionList[i]->pArray[j]->CheckCollisionSection(fTime);
			}

			if (pGroup->pSectionList[i]->iSize < 2)
			{
				pGroup->pSectionList[i]->iSize = 0;
				continue;
			}

			for (int j = 0; j < pGroup->pSectionList[i]->iSize - 1; ++j)
			{
				for (int k = j + 1; k < pGroup->pSectionList[i]->iSize; ++k)
				{
					CGameObject*	pSrc = pGroup->pSectionList[i]->pArray[j]->GetGameObject();
					CGameObject*	pDest = pGroup->pSectionList[i]->pArray[k]->GetGameObject();

					if (pSrc == pDest)
					{
						SAFE_RELEASE(pSrc);
						SAFE_RELEASE(pDest);
						continue;
					}

					if (pGroup->pSectionList[i]->pArray[j]->Collision(pGroup->pSectionList[i]->pArray[k]))
					{
						bCollision = true;

						if (!pGroup->pSectionList[i]->pArray[j]->CheckCollList(pGroup->pSectionList[i]->pArray[k]))
						{
							pGroup->pSectionList[i]->pArray[j]->AddCollList(pGroup->pSectionList[i]->pArray[k]);
							pGroup->pSectionList[i]->pArray[k]->AddCollList(pGroup->pSectionList[i]->pArray[j]);

							pSrc->OnCollisionEnter(pGroup->pSectionList[i]->pArray[j], pGroup->pSectionList[i]->pArray[k], fTime);
							pDest->OnCollisionEnter(pGroup->pSectionList[i]->pArray[k], pGroup->pSectionList[i]->pArray[j], fTime);
						}

						else
						{
							pSrc->OnCollision(pGroup->pSectionList[i]->pArray[j], pGroup->pSectionList[i]->pArray[k], fTime);
							pDest->OnCollision(pGroup->pSectionList[i]->pArray[k], pGroup->pSectionList[i]->pArray[j], fTime);
						}
					}

					else
					{
						if (pGroup->pSectionList[i]->pArray[j]->CheckCollList(pGroup->pSectionList[i]->pArray[k]))
						{
							pGroup->pSectionList[i]->pArray[j]->EraseCollList(pGroup->pSectionList[i]->pArray[k]);
							pGroup->pSectionList[i]->pArray[k]->EraseCollList(pGroup->pSectionList[i]->pArray[j]);

							pSrc->OnCollisionLeave(pGroup->pSectionList[i]->pArray[j], pGroup->pSectionList[i]->pArray[k], fTime);
							pDest->OnCollisionLeave(pGroup->pSectionList[i]->pArray[k], pGroup->pSectionList[i]->pArray[j], fTime);
						}
					}

					SAFE_RELEASE(pSrc);
					SAFE_RELEASE(pDest);
				}
			}

			pGroup->pSectionList[i]->iSize = 0;
		}
	}

	m_iMouseCollCount = 0;
}

bool CCollisionManager::SortZOrder(CCollider * pSrc, CCollider * pDest)
{
	CUI*	pUISrc = pSrc->FindComponentFromType<CUI>(CT_UI);
	CUI*	pUIDest = pDest->FindComponentFromType<CUI>(CT_UI);

	if (!pUISrc || !pUIDest)
		return false;

	int	iSrcZOrder = pUISrc->GetZOrder();
	int	iDestZOrder = pUIDest->GetZOrder();

	SAFE_RELEASE(pUISrc);
	SAFE_RELEASE(pUIDest);

	return iSrcZOrder > iDestZOrder;
}

bool CCollisionManager::SortZ(CCollider * pSrc, CCollider * pDest)
{
	Vector3	vSrcC = pSrc->GetColliderCenter();
	Vector3	vDestC = pDest->GetColliderCenter();

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();

	CTransform*	pCameraTr = pScene->GetMainCameraTr();

	Vector3	vCameraPos = pCameraTr->GetWorldPos();

	SAFE_RELEASE(pCameraTr);

	SAFE_RELEASE(pScene);

	float	fSrcDist = (vSrcC - vCameraPos).Length();
	float	fDestDist = (vDestC - vCameraPos).Length();

	float	fSrcRadius = pSrc->GetColliderRadius();
	float	fDestRadius = pDest->GetColliderRadius();

	fSrcDist -= fSrcRadius;
	fDestDist -= fDestRadius;

	return fSrcDist < fDestDist;
}
