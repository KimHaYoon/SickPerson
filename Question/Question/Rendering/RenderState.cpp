#include "RenderState.h"

CRenderState::CRenderState() //:
	//m_pState( NULL ),
	//m_pOldState( NULL )
{
}

CRenderState::~CRenderState()
{
	//SAFE_RELEASE( m_pOldState );
	//SAFE_RELEASE( m_pState );
}

RENDERSTATE_TYPE CRenderState::GetType() const
{
	return m_eType;
}

string CRenderState::GetKey() const
{
	return m_strKey;
}
