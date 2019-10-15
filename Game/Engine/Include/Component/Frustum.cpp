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
	// 8개의 정점을 월드공간으로 변환해준다.
	Vector3	vPos[8] = {};

	for (int i = 0; i < 8; ++i)
	{
		vPos[i] = m_vPos[i].TransformCoord(matInvVP.mat);
	}

	// 구해준 정점을 이용해서 6개의 평면정보를 만들어낸다.
	// 평면을 만들어주는 방법은 2가지가 있다.
	// 1. 법선벡터를 알고 평면위에 존재하는 임의의 한 점을 알고 있을 경우
	// 2. 평면을 구성하는 점 3개를 알고 있을 경우
	// 왼쪽 평면 생성
	m_Plane[FD_LEFT] = XMPlaneFromPoints(vPos[2].Convert(), vPos[6].Convert(),
		vPos[4].Convert());

	// 오른쪽 평면 생성
	m_Plane[FD_RIGHT] = XMPlaneFromPoints(vPos[1].Convert(), vPos[5].Convert(),
		vPos[7].Convert());

	// 위쪽 평면 생성
	m_Plane[FD_TOP] = XMPlaneFromPoints(vPos[4].Convert(), vPos[5].Convert(),
		vPos[1].Convert());

	// 아래쪽 평면 생성
	m_Plane[FD_BOTTOM] = XMPlaneFromPoints(vPos[2].Convert(), vPos[3].Convert(),
		vPos[7].Convert());

	// 앞쪽 평면 생성
	m_Plane[FD_NEAR] = XMPlaneFromPoints(vPos[0].Convert(), vPos[1].Convert(),
		vPos[3].Convert());

	// 뒤쪽 평면 생성
	m_Plane[FD_FAR] = XMPlaneFromPoints(vPos[5].Convert(), vPos[4].Convert(),
		vPos[6].Convert());
}

bool CFrustum::FrustumInPoint(const Vector3 & vPos)
{
	for (int i = 0; i < FD_END; ++i)
	{
		XMVECTOR Plane = m_Plane[i].Convert();
		// XMPlaneDotCoord 는 평면의 방정식을 계산한 값이 XMVECTOR의
		// x, y, z, w에 모두 들어온다. 그런데 이걸 직접 얻어오기가 까다롭기
		// 때문에 XMVectorGetX 함수를 이용해서 x값을 얻어오면 결과적으로
		// 평면의 방정식을 연산한 값을 얻어올 수 있다.
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
		// XMPlaneDotCoord 는 평면의 방정식을 계산한 값이 XMVECTOR의
		// x, y, z, w에 모두 들어온다. 그런데 이걸 직접 얻어오기가 까다롭기
		// 때문에 XMVectorGetX 함수를 이용해서 x값을 얻어오면 결과적으로
		// 평면의 방정식을 연산한 값을 얻어올 수 있다.
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
		// XMPlaneDotCoord 는 평면의 방정식을 계산한 값이 XMVECTOR의
		// x, y, z, w에 모두 들어온다. 그런데 이걸 직접 얻어오기가 까다롭기
		// 때문에 XMVectorGetX 함수를 이용해서 x값을 얻어오면 결과적으로
		// 평면의 방정식을 연산한 값을 얻어올 수 있다.
		if (XMVectorGetX(XMPlaneDotCoord(Plane, tSphere.vCenter.Convert())) - EPSILON >
			tSphere.fRadius)
			return false;
	}

	return true;
}
