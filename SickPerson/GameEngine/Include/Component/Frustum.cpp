#include "Frustum.h"

Engine_USING

#define	EPSILON	2

CFrustum::CFrustum()
{
	m_vPos[0] = Vector3(-1.f, 1.f, 0.f);
	m_vPos[1] = Vector3(1.f, 1.f, 0.f);
	m_vPos[2] = Vector3(-1.f, -1.f, 0.f);
	m_vPos[3] = Vector3(1.f, -1.f, 0.f);

	m_vPos[4] = Vector3(-1.f, 1.f, 1.f);
	m_vPos[5] = Vector3(1.f, 1.f, 1.f);
	m_vPos[6] = Vector3(-1.f, -1.f, 1.f);
	m_vPos[7] = Vector3(1.f, -1.f, 1.f);
	memset(m_Plane, 0, sizeof(Vector4) * FD_END);
}

CFrustum::~CFrustum()
{
}

void CFrustum::UpdateFrustum(const Matrix & matInvVP)
{
	// 8���� ������ ����������� ��ȯ���ش�.
	Vector3	vPos[8] = {};

	for (int i = 0; i < 8; ++i)
	{
		vPos[i] = m_vPos[i].TransformCoord(matInvVP.mat);
	}

	// ������ ������ �̿��ؼ� 6���� ��������� ������.
	// ����� ������ִ� ����� 2������ �ִ�.
	// 1. �������͸� �˰� ������� �����ϴ� ������ �� ���� �˰� ���� ���
	// 2. ����� �����ϴ� �� 3���� �˰� ���� ���
	// ���� ��� ����
	m_Plane[FD_LEFT] = XMPlaneFromPoints(vPos[2].Convert(), vPos[6].Convert(),
		vPos[4].Convert());

	// ������ ��� ����
	m_Plane[FD_RIGHT] = XMPlaneFromPoints(vPos[1].Convert(), vPos[5].Convert(),
		vPos[7].Convert());

	// ���� ��� ����
	m_Plane[FD_TOP] = XMPlaneFromPoints(vPos[4].Convert(), vPos[5].Convert(),
		vPos[1].Convert());

	// �Ʒ��� ��� ����
	m_Plane[FD_BOTTOM] = XMPlaneFromPoints(vPos[2].Convert(), vPos[3].Convert(),
		vPos[7].Convert());

	// ���� ��� ����
	m_Plane[FD_NEAR] = XMPlaneFromPoints(vPos[0].Convert(), vPos[1].Convert(),
		vPos[3].Convert());

	// ���� ��� ����
	m_Plane[FD_FAR] = XMPlaneFromPoints(vPos[5].Convert(), vPos[4].Convert(),
		vPos[6].Convert());
}

bool CFrustum::FrustumInPoint(const Vector3 & vPos)
{
	for (int i = 0; i < FD_END; ++i)
	{
		XMVECTOR Plane = m_Plane[i].Convert();
		// XMPlaneDotCoord �� ����� �������� ����� ���� XMVECTOR��
		// x, y, z, w�� ��� ���´�. �׷��� �̰� ���� �����Ⱑ ��ٷӱ�
		// ������ XMVectorGetX �Լ��� �̿��ؼ� x���� ������ ���������
		// ����� �������� ������ ���� ���� �� �ִ�.
		if (XMVectorGetX(XMPlaneDotCoord(Plane, vPos.Convert())) - EPSILON > 0)
			return false;
	}

	return true;
}

bool CFrustum::FrustumInSphere(const Vector3 & vCenter, float fRadius)
{
	for (int i = 0; i < FD_END; ++i)
	{
		XMVECTOR Plane = m_Plane[i].Convert();
		// XMPlaneDotCoord �� ����� �������� ����� ���� XMVECTOR��
		// x, y, z, w�� ��� ���´�. �׷��� �̰� ���� �����Ⱑ ��ٷӱ�
		// ������ XMVectorGetX �Լ��� �̿��ؼ� x���� ������ ���������
		// ����� �������� ������ ���� ���� �� �ִ�.
		float	fDist = XMVectorGetX(XMPlaneDotCoord(Plane, vCenter.Convert()));
		if (fDist - EPSILON > fRadius)
			return false;
	}

	return true;
}

bool CFrustum::FrustumInSphere(const SPHERE & tSphere)
{
	for (int i = 0; i < FD_END; ++i)
	{
		XMVECTOR Plane = m_Plane[i].Convert();
		// XMPlaneDotCoord �� ����� �������� ����� ���� XMVECTOR��
		// x, y, z, w�� ��� ���´�. �׷��� �̰� ���� �����Ⱑ ��ٷӱ�
		// ������ XMVectorGetX �Լ��� �̿��ؼ� x���� ������ ���������
		// ����� �������� ������ ���� ���� �� �ִ�.
		if (XMVectorGetX(XMPlaneDotCoord(Plane, tSphere.vCenter.Convert())) - EPSILON >
			tSphere.fRadius)
			return false;
	}

	return true;
}
