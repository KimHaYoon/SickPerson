#include "Camera.h"
#include "Transform.h"
#include "../Device.h"
#include "../GameObject/GameObject.h"

GAME_USING

CCamera::CCamera() :
	m_matView( NULL ),
	m_matProj( NULL ),
	m_pAttach( NULL )
{
	SetTag( "Camera" );
	SetTypeName( "CCamera" );
	SetTypeID<CCamera>();
	m_eType = CT_CAMERA;
	m_tWorldRS.iWidth = UINT_MAX;
	m_tWorldRS.iHeight = UINT_MAX;
	m_vPivot.x = 0.5f;
	m_vPivot.y = 0.5f;
}

CCamera::CCamera( const CCamera & camera ) :
	CComponent( camera )
{
	m_matView = new Matrix;
	m_matProj = new Matrix;

	*m_matView = *camera.m_matView;
	*m_matProj = *camera.m_matProj;

	m_tWorldRS = camera.m_tWorldRS;
	m_vPivot = camera.m_vPivot;

	m_pAttach = NULL;
}

CCamera::~CCamera()
{
	SAFE_RELEASE( m_pAttach );
	SAFE_DELETE( m_matView );
	SAFE_DELETE( m_matProj );
}

void CCamera::SetPivot( float x, float y )
{
	m_vPivot = Vector2( x, y );
}

void CCamera::SetPivot( const Vector2 & vPivot )
{
	m_vPivot = vPivot;
}

void CCamera::SetWorldResolution( UINT x, UINT y )
{
	m_tWorldRS.iWidth = x;
	m_tWorldRS.iHeight = y;
}

void CCamera::SetWorldResolution( const RESOLUTION & tRS )
{
	m_tWorldRS = tRS;
}

void CCamera::SetAttach( CGameObject * pAttach )
{
	SAFE_RELEASE( m_pAttach );
	if ( pAttach )
	{
		m_pAttach = pAttach->GetTransform();
		m_vPrevPos = m_pAttach->GetWorldPos();
	}
}

void CCamera::SetAttach( CComponent * pAttach )
{
	SAFE_RELEASE( m_pAttach );
	if ( pAttach )
	{
		m_pAttach = pAttach->GetTransform();
		m_vPrevPos = m_pAttach->GetWorldPos();
	}
}

Matrix CCamera::GetViewMatrix() const
{
	return *m_matView;
}

Matrix CCamera::GetProjMatrix() const
{
	return *m_matProj;
}

void CCamera::SetOrthoProj( const RESOLUTION & tRS,
	float fNear, float fFar )
{
	/*
	2/(r-l)      0            0           0
	0            2/(t-b)      0           0
	0            0            1/(zf-zn)   0
	(l+r)/(l-r)  (t+b)/(b-t)  zn/(zn-zf)  l
	*/
	*m_matProj = XMMatrixOrthographicOffCenterLH( 0.f, tRS.iWidth,
		tRS.iHeight, 0.f, fNear, fFar );

	m_eCameraType = CT_ORTHO;
}

void CCamera::SetPerspectiveProj( float fViewAngle, float fAspect, float fNear, float fFar )
{
	*m_matProj = XMMatrixPerspectiveFovLH( fViewAngle,
		fAspect, fNear, fFar );

	m_eCameraType = CT_PERSPECTIVE;
}

bool CCamera::Init()
{
	m_matView = new Matrix;
	m_matProj = new Matrix;

	*m_matView = XMMatrixIdentity();
	*m_matProj = XMMatrixIdentity();

	return true;
}

void CCamera::Input( float fTime )
{
}

int CCamera::Update( float fTime )
{
	return 0;
}

int CCamera::LateUpdate( float fTime )
{
	if ( m_pAttach )
	{
		if ( m_eCameraType == CT_ORTHO )
		{
			if ( m_tWorldRS.iWidth != UINT_MAX && m_tWorldRS.iHeight != UINT_MAX )
			{
				RESOLUTION	tRS = DEVICE_RESOLUTION;
				Vector3 vAttachPos = m_pAttach->GetWorldPos();
				Vector3	vPos = m_pTransform->GetWorldPos();

				// 타겟이 이동할 수 있는 범위를 만들어준다.
				float	fLeft = m_vPivot.x * tRS.iWidth;
				float	fTop = m_vPivot.y * tRS.iHeight;
				float	fRight = m_tWorldRS.iWidth - tRS.iWidth * ( 1.f - m_vPivot.x );
				float	fBottom = m_tWorldRS.iHeight - tRS.iHeight * ( 1.f - m_vPivot.y );

				// 좌우로 움직일 수 있는 영역 안에 타겟이 있을 경우
				// 움직일 수 있도록 해준다.
				if ( vAttachPos.x <= fLeft )
					vPos.x = 0.f;

				else if ( vAttachPos.x >= fRight )
					vPos.x = m_tWorldRS.iWidth - tRS.iWidth;

				else
					vPos.x = vAttachPos.x - fLeft;

				if ( vAttachPos.y <= fTop )
					vPos.y = 0.f;

				else if ( vAttachPos.y >= fBottom )
					vPos.y = m_tWorldRS.iHeight - tRS.iHeight;

				else
					vPos.y = vAttachPos.y - fTop;

				m_pTransform->SetWorldPos( vPos );

			}
		}

		else
		{
			Vector3 vPos = m_pAttach->GetWorldPos();
			Vector3	vMove = vPos - m_vPrevPos;
			m_vPrevPos = vPos;

			m_pTransform->MoveWorld( vMove );
		}
	}

	*m_matView = XMMatrixIdentity();

	Vector3 vPos = m_pTransform->GetWorldPos();
	m_matView->m[3][0] = -vPos.x;
	m_matView->m[3][1] = -vPos.y;
	m_matView->m[3][2] = -vPos.z;

	return 0;
}

void CCamera::Collision( float fTime )
{
}

void CCamera::Render( float fTime )
{
}

CCamera * CCamera::Clone()
{
	return new CCamera( *this );
}

void CCamera::Save( FILE * pFile )
{
	CComponent::Save( pFile );

	fwrite( m_matView, sizeof( Matrix ), 1, pFile );
	fwrite( m_matProj, sizeof( Matrix ), 1, pFile );
}

void CCamera::Load( FILE * pFile )
{
	CComponent::Load( pFile );

	fread( m_matView, sizeof( Matrix ), 1, pFile );
	fread( m_matProj, sizeof( Matrix ), 1, pFile );
}
