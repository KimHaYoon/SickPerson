#include "ColliderSphere.h"
#include "Transform.h"
#include "ColliderRay.h"
#include "Camera.h"
#include "../Resources/ResourcesManager.h"
#include "../Rendering/ShaderManager.h"
#include "../Scene/Scene.h"
#include "../Rendering/RenderManager.h"
#include "../Rendering/RenderState.h"

Engine_USING

CColliderSphere::CColliderSphere()
{
	SetTag("ColliderSphere");
	SetTypeName("CColliderSphere");
	SetTypeID<CColliderSphere>();
	m_eCollType = CT_SPHERE;
	m_strGroup = "Default";
#ifdef _DEBUG
	m_pWireFrame = GET_SINGLE(CRenderManager)->FindRenderState(WIRE_FRAME);
#endif // _DEBUG
}

CColliderSphere::CColliderSphere(const CColliderSphere & collider) :
	CCollider(collider)
{
	m_tRelativeSphere = collider.m_tRelativeSphere;
}

CColliderSphere::~CColliderSphere()
{
}

SPHERE CColliderSphere::GetSphere() const
{
	return m_tSphere;
}

void CColliderSphere::SetSphere(const Vector3 & vCenter,
	float fRadius)
{
	m_tRelativeSphere.vCenter = vCenter;
	m_tRelativeSphere.fRadius = fRadius;
}

bool CColliderSphere::Init()
{
	CCollider::Init();

#ifdef _DEBUG
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("Sphere");
	m_pShader = GET_SINGLE(CShaderManager)->FindShader(COLLIDER_SHADER);
	m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout("VertexColor");
#endif // _DEBUG

	return true;
}

void CColliderSphere::Input(float fTime)
{
}

int CColliderSphere::Update(float fTime)
{
	CCollider::Update(fTime);

	return 0;
}

int CColliderSphere::LateUpdate(float fTime)
{
	CCollider::LateUpdate(fTime);

	m_tSphere.vCenter = m_tRelativeSphere.vCenter + m_pTransform->GetWorldPos();
	m_tSphere.fRadius = m_tRelativeSphere.fRadius;

	m_vCollMin = m_tSphere.vCenter - m_tSphere.fRadius;
	m_vCollMax = m_tSphere.vCenter + m_tSphere.fRadius;

	return 0;
}

void CColliderSphere::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderSphere::Render(float fTime)
{
#ifdef _DEBUG
	Matrix	matScale, matTrans, matRot;

	float	fLength = m_tSphere.fRadius;
	Vector3	vLength = Vector3(fLength, fLength, fLength);
	Vector3	vPos = m_tSphere.vCenter;

	matScale.mat = XMMatrixScaling(fLength, fLength, fLength);
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

CColliderSphere * CColliderSphere::Clone()
{
	return new CColliderSphere(*this);
}

bool CColliderSphere::Collision(CCollider * pDest)
{
	switch (pDest->GetColliderType())
	{
	case CT_RAY:
		return CollisionRayToSphere(((CColliderRay*)pDest)->GetRayAddress(), m_tSphere);
	case CT_SPHERE:
		return CollisionSphereToSphere(((CColliderSphere*)pDest)->m_tSphere, m_tSphere);
	}

	return false;
}
