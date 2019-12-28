#include "BoneSocket.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"

GAME_USING

CBoneSocket::CBoneSocket()	:
	m_pTarget(NULL),
	m_pTargetTr(NULL)
{
}

CBoneSocket::CBoneSocket(const CBoneSocket & socket)
{
	*this = socket;

	m_pTarget = NULL;
	m_pTargetTr = NULL;
}

CBoneSocket::~CBoneSocket()
{
	SAFE_RELEASE(m_pTargetTr);
	SAFE_RELEASE(m_pTarget);
}

void CBoneSocket::SetTarget(CGameObject * pTarget)
{
	if (pTarget)
	{
		SAFE_RELEASE(m_pTarget);
		SAFE_RELEASE(m_pTargetTr);
		pTarget->AddRef();
		m_pTarget = pTarget;

		m_pTargetTr = pTarget->GetTransform();
	}

	else
	{
		SAFE_RELEASE(m_pTargetTr);
		SAFE_RELEASE(m_pTarget);
	}
}

void CBoneSocket::Update(const Matrix & matWorld, const Matrix & matBone)
{
	m_pTargetTr->SetParentMatrix(matBone * matWorld);
}

CBoneSocket * CBoneSocket::Clone() const
{
	return new CBoneSocket(*this);
}
