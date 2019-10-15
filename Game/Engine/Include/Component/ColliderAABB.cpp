#include "ColliderAABB.h"
#include "Transform.h"
#include "Camera.h"
#include "../Resources/ResourcesManager.h"
#include "../Rendering/ShaderManager.h"
#include "../Scene/Scene.h"

Engine_USING

CColliderAABB::CColliderAABB()
{
	SetTag("ColliderAABB");
	SetTypeName("CColliderAABB");
	SetTypeID<CColliderAABB>();
	m_eCollType = CT_AABB;
}

CColliderAABB::CColliderAABB(const CColliderAABB & collider)	:
	CCollider(collider)
{
	m_tRelativeInfo = collider.m_tRelativeInfo;
}

CColliderAABB::~CColliderAABB()
{
}

AABB CColliderAABB::GetInfo() const
{
	return m_tInfo;
}

AABB CColliderAABB::GetRelativeInfo() const
{
	return m_tRelativeInfo;
}

void CColliderAABB::SetAABB(const Vector3 & vMin, const Vector3 & vMax)
{
	m_tRelativeInfo.vMin = vMin;
	m_tRelativeInfo.vMax = vMax;
}

bool CColliderAABB::Init()
{
	CCollider::Init();

#ifdef _DEBUG
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("AABB");
	m_pShader = GET_SINGLE(CShaderManager)->FindShader(COLLIDER_SHADER);
	m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout("VertexColor");
#endif // _DEBUG

	return true;
}

void CColliderAABB::Input(float fTime)
{
}

int CColliderAABB::Update(float fTime)
{
	CCollider::Update(fTime);
	return 0;
}

int CColliderAABB::LateUpdate(float fTime)
{
	CCollider::LateUpdate(fTime);

	m_tInfo.vMin = m_tRelativeInfo.vMin + m_pTransform->GetWorldPos();
	m_tInfo.vMax = m_tRelativeInfo.vMax + m_pTransform->GetWorldPos();

	m_vCollMin = m_tInfo.vMin;
	m_vCollMax = m_tInfo.vMax;

	return 0;
}

void CColliderAABB::Collision(float fTime)
{
}

void CColliderAABB::Render(float fTime)
{
#ifdef _DEBUG
	Matrix	matScale, matTrans, matRot;

	Vector3	vLength = m_tInfo.vMax - m_tInfo.vMin;
	Vector3	vPos = (m_tInfo.vMax + m_tInfo.vMin) / 2.f;

	matScale.mat = XMMatrixScaling(vLength.x, vLength.y, vLength.z);
	matTrans.mat = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);

	CCamera*	pCamera = m_pScene->GetMainCamera();

	m_tTransform.matWorld = matScale *
		matRot * matTrans;

	m_tTransform.matView = pCamera->GetViewMatrix();
	m_tTransform.matProj = pCamera->GetProjMatrix();
	m_tTransform.matWV = m_tTransform.matWorld * m_tTransform.matView;
	m_tTransform.matWVP = m_tTransform.matWV * m_tTransform.matProj;
	m_tTransform.matWP = m_tTransform.matWorld * m_tTransform.matProj;
	m_tTransform.vPivot = Vector3(0.5f, 0.5f, 0.5f);
	m_tTransform.vLength = Vector3::One;
	m_tTransform.vLength.z = 0.f;

	m_tTransform.matWorld = XMMatrixTranspose(m_tTransform.matWorld.mat);
	m_tTransform.matView = XMMatrixTranspose(m_tTransform.matView.mat);
	m_tTransform.matProj = XMMatrixTranspose(m_tTransform.matProj.mat);
	m_tTransform.matWV = XMMatrixTranspose(m_tTransform.matWV.mat);
	m_tTransform.matWVP = XMMatrixTranspose(m_tTransform.matWVP.mat);
	m_tTransform.matWP = XMMatrixTranspose(m_tTransform.matWP.mat);

	SAFE_RELEASE(pCamera);
#endif // _DEBUG

	CCollider::Render(fTime);
}

CColliderAABB * CColliderAABB::Clone()
{
	return new CColliderAABB(*this);
}

bool CColliderAABB::Collision(CCollider * pDest)
{
	switch (pDest->GetColliderType())
	{
	case CT_AABB:
		return CollisionAABBToAABB(m_tInfo, ((CColliderAABB*)pDest)->m_tInfo);
	}

	return false;
}
