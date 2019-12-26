#include "Script.h"

GAME_USING


CScript::CScript()
{
	SetTag( "Script" );
	m_eType = CT_SCRIPT;
}

CScript::CScript( const CScript & script ) :
	CComponent( script )
{
}

CScript::~CScript()
{
}

void CScript::Input( float fTime )
{
}

int CScript::Update( float fTime )
{
	return 0;
}

int CScript::LateUpdate( float fTime )
{
	return 0;
}

void CScript::Collision( float fTime )
{
}

void CScript::Render( float fTime )
{
}

void CScript::OnCollisionEnter( CCollider * pSrc, CCollider * pDest, float fTime )
{
}

void CScript::OnCollision( CCollider * pSrc, CCollider * pDest, float fTime )
{
}

void CScript::OnCollisionLeave( CCollider * pSrc, CCollider * pDest, float fTime )
{
}
