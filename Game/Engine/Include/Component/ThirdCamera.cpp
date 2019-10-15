#include "ThirdCamera.h"
#include "Transform.h"
#include "Arm.h"
#include "../Core/Input.h"
#include "../GameObject/GameObject.h"

Engine_USING

CThirdCamera::CThirdCamera() :
	m_bRotationAuto(true)
{
	SetTag("ThirdCamera");
	SetTypeName("CThirdCamera");
	SetTypeID<CThirdCamera>();
	m_eType = CT_THIRDCAMERA;
	m_pArm = NULL;
	m_iDragCount = 0;
}

CThirdCamera::CThirdCamera(const CThirdCamera & camera) :
	CComponent(camera)
{
	m_bRotationAuto = camera.m_bRotationAuto;
	m_fZoomSpeed = camera.m_fZoomSpeed;
	m_iMoveDir = 0;
	m_pArm = NULL;
	m_iDragCount = 0;
}

CThirdCamera::~CThirdCamera()
{
	SAFE_RELEASE(m_pArm);
}

void CThirdCamera::SetRotationAuto(bool bAuto)
{
	m_bRotationAuto = bAuto;
}

void CThirdCamera::Zoom()
{
	if (m_pArm)
		m_pArm->Zoom(m_pTransform->GetWorldAxis(AXIS_Z) * m_iMoveDir, m_fZoomSpeed);

	else
		m_pTransform->MoveWorld(AXIS_Z, m_fZoomSpeed * m_iMoveDir);
}

bool CThirdCamera::Init()
{
	m_fZoomSpeed = 1.f;

	return true;
}

void CThirdCamera::Input(float fTime)
{
	m_iMoveDir = GET_SINGLE(CInput)->GetWheel();

	static bool	bTest = false;

	if (GetAsyncKeyState(VK_MBUTTON) & 0x8000)
	{
		bTest = true;
	}

	else if (bTest)
	{
		bTest = false;
		if (m_pArm)
			m_pArm->RotationY(PI);
	}

	if (KEYPUSH("MouseRButton"))
	{
		++m_iDragCount;

		if (m_iDragCount > 2)
			m_iDragCount = 2;
	}

	else
		m_iDragCount = 0;
}

int CThirdCamera::Update(float fTime)
{
	if (!m_pArm)
		m_pArm = FindComponentFromType<CArm>(CT_ARM);

	if (m_iMoveDir != 0)
		Zoom();

	// 마우스 RButton을 누른 상태에서 Drag가 시작된다.
	if (m_iDragCount == 2 && m_pArm && m_bRotationAuto)
	{
		POINT	ptDrag = GET_SINGLE(CInput)->GetMouseMove();

		if (ptDrag.x != 0)
		{
			float	fAngle = ptDrag.x / 1000.f * PI;

			m_pArm->RotationY(fAngle);
		}

		if (ptDrag.y != 0)
		{
			float	fAngle = ptDrag.y / 1000.f * PI;

			m_pArm->RotationAxis(m_pTransform->GetWorldAxis(AXIS_X), fAngle);
		}
	}

	return 0;
}

int CThirdCamera::LateUpdate(float fTime)
{
	return 0;
}

void CThirdCamera::Collision(float fTime)
{
}

void CThirdCamera::Render(float fTime)
{
}

CThirdCamera * CThirdCamera::Clone()
{
	return new CThirdCamera(*this);
}

void CThirdCamera::Save(FILE * pFile)
{
}

void CThirdCamera::Load(FILE * pFile)
{
}
