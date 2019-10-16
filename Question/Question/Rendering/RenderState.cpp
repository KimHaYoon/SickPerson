#include "RenderState.h"


CRenderState::CRenderState()
{
	int i = 0;
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
