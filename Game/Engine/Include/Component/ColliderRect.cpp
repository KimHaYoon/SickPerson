#include "ColliderRect.h"
#include "Camera.h"
#include "Transform.h"
#include "ColliderPoint.h"
#include "../Resources/Mesh.h"
#include "../Resources/ResourcesManager.h"
#include "../Rendering/Shader.h"
#include "../Rendering/ShaderManager.h"
#include "../Scene/Scene.h"

Engine_USING

CColliderRect::CColliderRect()
{
	SetTag("ColliderRect");
	SetTypeName("CColliderRect");
	SetTypeID<CColliderRect>();
	m_eCollType = CT_RECT;
	m_strGroup = "UI";
}


CColliderRect::CColliderRect(const CColliderRect & coll)	:
	CCollider(coll)
{
	m_tInfo = coll.m_tInfo;
}

CColliderRect::~CColliderRect()
{
}

void CColliderRect::SetRect(float l, float t, float w, float h)
{
	m_tInfo = RECTINFO(l, t, l + w, t + h);
}

RECTINFO CColliderRect::GetInfo() const
{
	return m_tInfo;
}

bool CColliderRect::Init()
{
	CCollider::Init();

#ifdef _DEBUG
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("RectOrthoCenterLine");
	m_pShader = GET_SINGLE(CShaderManager)->FindShader(COLLIDER_SHADER);
	m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout("VertexColor");
#endif // _DEBUG
	
	return true;
}

void CColliderRect::Input(float fTime)
{
}

int CColliderRect::Update(float fTime)
{
	CCollider::Update(fTime);

	m_tInfo.l += m_vMove.x;
	m_tInfo.r += m_vMove.x;
	m_tInfo.t += m_vMove.y;
	m_tInfo.b += m_vMove.y;

	return 0;
}

int CColliderRect::LateUpdate(float fTime)
{
	CCollider::LateUpdate(fTime);

	m_vCollMin = Vector3(m_tInfo.l, m_tInfo.t, 0.f);
	m_vCollMax = Vector3(m_tInfo.r, m_tInfo.b, 0.f);

	return 0;
}

void CColliderRect::Collision(float fTime)
{
}

void CColliderRect::Render(float fTime)
{
#ifdef _DEBUG
	Matrix	matScale, matTrans, matParent;
	float	x = m_tInfo.r - m_tInfo.l;
	float	y = m_tInfo.b - m_tInfo.t;
	matScale.mat = XMMatrixScaling(x, y, 1.f);
	matTrans.mat = XMMatrixTranslation(m_tInfo.l + x / 2.f, 
		m_tInfo.t + y / 2.f, 0.f);
	Matrix	matParent1 = m_pTransform->GetParentMatrix();
	matParent.mat = XMMatrixTranslation(matParent1._41, matParent1._42,
		matParent1._43);

	CCamera*	pCamera = m_pScene->GetUICamera();

	m_tTransform.matWorld = matScale * matTrans * matParent;

	if (m_eViewType == VT_NORMAL)
		m_tTransform.matView = pCamera->GetViewMatrix();

	else
		m_tTransform.matView = XMMatrixIdentity();

	m_tTransform.matProj = pCamera->GetProjMatrix();
	m_tTransform.matWV = m_tTransform.matWorld * m_tTransform.matView;
	m_tTransform.matWVP = m_tTransform.matWV * m_tTransform.matProj;
	m_tTransform.matWP = m_tTransform.matWorld * m_tTransform.matProj;
	m_tTransform.vPivot = m_pTransform->GetPivot();
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

CColliderRect * CColliderRect::Clone()
{
	return new CColliderRect(*this);
}

bool CColliderRect::Collision(CCollider * pDest)
{
	switch (pDest->GetColliderType())
	{
	case CT_RECT:
		return CollisionRectToRect(m_tInfo, ((CColliderRect*)pDest)->m_tInfo);
	case CT_POINT:
		return CollisionRectToPoint(m_tInfo, ((CColliderPoint*)pDest)->GetPos());
	}

	return false;
}
