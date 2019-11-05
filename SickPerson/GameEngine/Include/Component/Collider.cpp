#include "Collider.h"
#include "Transform.h"
#include "../Resources/Mesh.h"
#include "../Resources/ResourcesManager.h"
#include "../Rendering/Shader.h"
#include "../Rendering/ShaderManager.h"
#include "../GameObject/GameObject.h"
#include "../Core/TimerManager.h"
#include "../Core/Timer.h"
#include "../Rendering/RenderManager.h"
#include "../Rendering/RenderState.h"

Engine_USING

CCollider::CCollider()
{
	m_eType = CT_COLLIDER;
#ifdef _DEBUG
	m_pMesh = NULL;
	m_pShader = NULL;
	m_pLayout = NULL;
	m_vColor = Vector4::Green;
	m_eViewType = VT_NORMAL;
	m_pWireFrame = NULL;
#endif // _DEBUG
	m_strGroup = "Default";
	m_vecSectionIndex.reserve(8);

}

CCollider::CCollider(const CCollider & coll) :
	CComponent(coll)
{
	m_strGroup = coll.m_strGroup;
	m_eCollType = coll.m_eCollType;
	m_vecSectionIndex.reserve(8);
#ifdef _DEBUG
	m_pMesh = coll.m_pMesh;
	m_pShader = coll.m_pShader;
	m_pLayout = coll.m_pLayout;
	m_eViewType = coll.m_eViewType;
	m_pWireFrame = coll.m_pWireFrame;
	if (m_pMesh)
		m_pMesh->AddRef();

	if(m_pShader)
		m_pShader->AddRef();

	if (m_pWireFrame)
		m_pWireFrame->AddRef();
#endif
}

CCollider::~CCollider()
{
	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_CollList.end();

	CTimer*	pTimer = GET_SINGLE(CTimerManager)->FindTimer("MainThread");

	for (iter = m_CollList.begin(); iter != iterEnd; ++iter)
	{
		CGameObject*	pObj = (*iter)->GetGameObject();
		pObj->OnCollisionLeave(*iter, this, pTimer->GetDeltaTime());
		SAFE_RELEASE(pObj);
		(*iter)->EraseCollList(this);
	}

	SAFE_RELEASE(pTimer);

#ifdef _DEBUG
	SAFE_RELEASE(m_pWireFrame);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);
#endif
}

Vector3 CCollider::GetColliderMin() const
{
	return m_vCollMin;
}

Vector3 CCollider::GetColliderMax() const
{
	return m_vCollMax;
}

Vector3 CCollider::GetColliderCenter()
{
	m_vCenter = (m_vCollMin + m_vCollMax) / 2.f;
	return m_vCenter;
}

float CCollider::GetColliderRadius()
{
	m_fRadius = (m_vCollMin - m_vCollMax).Length() / 2.f;
	return m_fRadius;
}

COLLIDER_TYPE CCollider::GetColliderType() const
{
	return m_eCollType;
}

void CCollider::AddCollList(CCollider * pCollider)
{
	m_CollList.push_back(pCollider);
}

bool CCollider::CheckCollList(CCollider * pCollider)
{
	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_CollList.end();

	for (iter = m_CollList.begin(); iter != iterEnd; ++iter)
	{
		if (*iter == pCollider)
			return true;
	}

	return false;
}

bool CCollider::CheckCollList(const string & strTag)
{
	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_CollList.end();

	for (iter = m_CollList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
			return true;
	}

	return false;
}

void CCollider::EraseCollList(CCollider * pCollider)
{
	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_CollList.end();

	for (iter = m_CollList.begin(); iter != iterEnd; ++iter)
	{
		if (*iter == pCollider)
		{
			m_CollList.erase(iter);
			return;
		}
	}
}

void CCollider::SetViewType(VIEW_TYPE eType)
{
#ifdef _DEBUG
	m_eViewType = eType;
#endif // _DEBUG
}

string CCollider::GetCollisionGroup() const
{
	return m_strGroup;
}

void CCollider::SetCollisionGroup(const string & strGroup)
{
	m_strGroup = strGroup;
}

void CCollider::AddSectionIndex(int iIndex)
{
	m_vecSectionIndex.push_back(iIndex);
}

void CCollider::ClearSectionIndex()
{
	m_vecSectionIndex.clear();
}

void CCollider::CheckCollisionSection(float fTime)
{
	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_CollList.end();

	for (iter = m_CollList.begin(); iter != iterEnd;)
	{
		if ((*iter)->GetTag() == "MouseRay" ||
			(*iter)->GetTag() == "MousePoint")
		{
			++iter;
			continue;
		}

		bool	bPair = false;

		for (size_t i = 0; i < m_vecSectionIndex.size(); ++i)
		{
			for (size_t j = 0; j < (*iter)->m_vecSectionIndex.size(); ++j)
			{
				if (m_vecSectionIndex[i] == (*iter)->m_vecSectionIndex[j])
				{
					bPair = true;
					break;
				}
			}

			if (bPair)
				break;
		}

		if (!bPair)
		{
			m_pGameObject->OnCollisionLeave(this, *iter, fTime);
			CGameObject*	pDestObj = (*iter)->GetGameObject();

			pDestObj->OnCollisionLeave(*iter, this, fTime);

			SAFE_RELEASE(pDestObj);

			(*iter)->EraseCollList(this);
			iter = m_CollList.erase(iter);
			iterEnd = m_CollList.end();
		}

		else
			++iter;
	}
}

bool CCollider::Init()
{
	return true;
}

void CCollider::Input(float fTime)
{
}

int CCollider::Update(float fTime)
{
	Vector3	vPos = m_pTransform->GetWorldPos();

	m_vMove = vPos - m_vPrevPos;

	m_vPrevPos = vPos;

	return 0;
}

int CCollider::LateUpdate(float fTime)
{
#ifdef _DEBUG
	m_vColor = Vector4::Black;
#endif // DEBUG

	return 0;
}

void CCollider::Collision(float fTime)
{
}

void CCollider::Render(float fTime)
{
#ifdef _DEBUG
	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
		&m_tTransform, SCT_VERTEX | SCT_PIXEL);
	GET_SINGLE(CShaderManager)->UpdateCBuffer("ColliderColor",
		&m_vColor, SCT_VERTEX);

	if (m_pWireFrame)
		m_pWireFrame->SetState();

	m_pShader->SetShader();
	GET_SINGLE(CShaderManager)->SetInputLayout("VertexColor");
	m_pMesh->Render(fTime);

	if (m_pWireFrame)
		m_pWireFrame->ResetState();
#endif // _DEBUG

}

void CCollider::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{
#ifdef _DEBUG
	m_vColor = Vector4::Red;
#endif // DEBUG
}

void CCollider::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{
#ifdef _DEBUG
	m_vColor = Vector4::Red;
#endif // DEBUG
}

void CCollider::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
#ifdef _DEBUG
	m_vColor = Vector4::Black;
#endif // DEBUG
}

bool CCollider::CollisionRectToRect(const RECTINFO & rc1, 
	const RECTINFO & rc2)
{
	if (rc1.l > rc2.r)
		return false;

	else if (rc1.r < rc2.l)
		return false;

	else if (rc1.t > rc2.b)
		return false;

	else if (rc1.b < rc2.t)
		return false;

	return true;
}

bool CCollider::CollisionRectToPoint(const RECTINFO & rc, 
	const Vector2 & point)
{
	if (rc.l > point.x)
		return false;

	else if (rc.r < point.x)
		return false;

	else if (rc.t > point.y)
		return false;

	else if (rc.b < point.y)
		return false;

	return true;
}

bool CCollider::CollisionObb2DToObb2D(const OBB2DINFO & tObb1,
	const OBB2DINFO & tObb2)
{
	// 각 상자들의 축들에 대해 분리축 테스트를 한다.
	float	r1, r2;
	float	fDist;
	
	Vector3	vCenterDir = tObb1.vCenter - tObb2.vCenter;
 	Vector3	vAxis = tObb1.vAxisX;

	r1 = tObb1.fLengthX;
	r2 = abs(tObb2.vAxisX.Dot(vAxis)) * tObb2.fLengthX +
		abs(tObb2.vAxisY.Dot(vAxis)) * tObb2.fLengthY;
	fDist = vCenterDir.Dot(vAxis);
	fDist = abs(fDist);

	if (r1 + r2 < fDist)
		return false;

	vAxis = tObb1.vAxisY;
	r1 = tObb1.fLengthY;
	r2 = abs(tObb2.vAxisX.Dot(vAxis)) * tObb2.fLengthX +
		abs(tObb2.vAxisY.Dot(vAxis)) * tObb2.fLengthY;
	fDist = vCenterDir.Dot(vAxis);
	fDist = abs(fDist);

	if (r1 + r2 < fDist)
		return false;

	vAxis = tObb2.vAxisX;
	r1 = tObb2.fLengthX;
	r2 = abs(tObb1.vAxisX.Dot(vAxis)) * tObb1.fLengthX +
		abs(tObb1.vAxisY.Dot(vAxis)) * tObb1.fLengthY;
	fDist = vCenterDir.Dot(vAxis);
	fDist = abs(fDist);

	if (r1 + r2 < fDist)
		return false;

	vAxis = tObb2.vAxisY;
	r1 = tObb2.fLengthY;
	r2 = abs(tObb1.vAxisX.Dot(vAxis)) * tObb1.fLengthX +
		abs(tObb1.vAxisY.Dot(vAxis)) * tObb1.fLengthY;
	fDist = vCenterDir.Dot(vAxis);
	fDist = abs(fDist);

	if (r1 + r2 < fDist)
		return false;

	return true;
}

bool CCollider::CollisionAABBToAABB(const AABB & src, const AABB & dest)
{
	if (src.vMin.x > dest.vMax.x)
		return false;

	else if (src.vMax.x < dest.vMin.x)
		return false;

	else if (src.vMin.y > dest.vMax.y)
		return false;

	else if (src.vMax.y < dest.vMin.y)
		return false;

	else if (src.vMin.z > dest.vMax.z)
		return false;

	else if (src.vMax.z < dest.vMin.z)
		return false;

	return true;
}

bool CCollider::CollisionRayToSphere(PRAY pRay, 
	const SPHERE & tSphere)
{
	float	b, c;
	Vector3	vM = pRay->vPos - tSphere.vCenter;
	b = 2.f * vM.Dot(pRay->vDir);
	c = vM.Dot(vM) - tSphere.fRadius * tSphere.fRadius;

	float	fDet = b * b - 4.f * c;

	if (fDet < 0.f)
		return false;

	fDet = sqrtf(fDet);

	float	t0, t1;

	t0 = (-b + fDet) / 2.f;
	t1 = (-b - fDet) / 2.f;

	if (t0 < 0.f && t1 < 0.f)
		return false;

	else if (t1 > pRay->fDist && pRay->fDist > 0.f)
		return false;

	// 가까운 값을 이용하여 처리한다.
	pRay->vIntersect = pRay->vPos + pRay->vDir * t1;

	return true;
}

bool CCollider::CollisionSphereToSphere(const SPHERE & tSrc, const SPHERE & tDest)
{
	return tSrc.vCenter.Distance(tDest.vCenter) <= tSrc.fRadius + tDest.fRadius;
}
