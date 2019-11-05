#include "Player.h"
#include "Core/Input.h"
#include "Component/Transform.h"
#include "Component/ThirdCamera.h"
#include "Component/Arm.h"
#include "Component/Navigation3D.h"
#include "Component/Renderer.h"
#include "Component/Transform.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"

#define MAKING

CPlayer::CPlayer() : 
	m_pAnimation(NULL)
{
	SetTag("Player");
	SetTypeName("CPlayer");
	SetTypeID<CPlayer>();
}

CPlayer::CPlayer(const CPlayer & player) : 
	CScript(player)
{
	m_pAnimation = NULL;
	m_fMoveSpeed = player.m_fMoveSpeed;
	m_fRotSpeed = player.m_fRotSpeed;
}

CPlayer::~CPlayer()
{
	SAFE_RELEASE(m_pAnimation);
}

bool CPlayer::Init()
{
	m_fMoveSpeed = 5.f;	
	m_fRotSpeed = PI;


	// 플레이어 카메라
	CGameObject*	pCameraObj = m_pScene->GetMainCameraObj();
	CThirdCamera*	pThirdCamera = pCameraObj->AddComponent<CThirdCamera>("PlayerCamera");
	CTransform*		pThirdCameraTr = pThirdCamera->GetTransform();
	pThirdCameraTr->SetWorldPos(m_pTransform->GetWorldPos());
	SAFE_RELEASE(pThirdCameraTr);
	SAFE_RELEASE(pThirdCamera);
	CArm* pArm = pCameraObj->AddComponent<CArm>("Arm");
	pArm->SetTarget(m_pTransform);
	pArm->SetLookAtDist(Vector3(0.f, 0.1f, 0.f));
#ifdef MAKING
	pArm->SetMaxDist(50.f);
	m_fMoveSpeed = 30.f;
#endif // _DEBUG

	SAFE_RELEASE(pArm);
	SAFE_RELEASE(pCameraObj);

	// 지형을 타기위한 Navigation Component 추가
	CNavigation3D* pNavigation3D = m_pGameObject->AddComponent<CNavigation3D>("Navigation");
	SAFE_RELEASE(pNavigation3D);

	// 캐릭터 메쉬
	CRenderer*	pRenderer = m_pGameObject->AddComponent<CRenderer>("PlayerRenderer");
	pRenderer->SetMesh("Male", L"Character/male5.FBX");
	SAFE_RELEASE(pRenderer);

	m_pTransform->SetLocalRotX(-PI / 2);

	return true;
}

void CPlayer::Input(float fTime)
{
	if (KEYPUSH("MoveFront"))
	{
		m_pTransform->MoveWorld(AXIS_Z, m_fMoveSpeed, fTime);
	}

	if (KEYPUSH("MoveBack"))
	{
		m_pTransform->MoveWorld(AXIS_Z, -m_fMoveSpeed, fTime);
	}
	
	if (KEYPUSH("RotY"))
	{
		m_pTransform->RotateWorldY(m_fRotSpeed, fTime);
	}

	if (KEYPUSH("RotInvY"))
	{
		m_pTransform->RotateWorldY(-m_fRotSpeed, fTime);
	}
}

int CPlayer::Update(float fTime)
{
	return 0;
}

int CPlayer::LateUpdate(float fTime)
{
	return 0;
}

CPlayer * CPlayer::Clone()
{
	return new CPlayer(*this);
}
