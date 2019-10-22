#include "PipelineStateManager.h"
#include "PipelineState.h"

DEFINITION_SINGLE(CPipelineStateManager)

CPipelineStateManager::CPipelineStateManager()
{
}


CPipelineStateManager::~CPipelineStateManager()
{
}

bool CPipelineStateManager::Init()
{
	return true;
}

CPipelineState * CPipelineStateManager::CreatePipelineState( const string & strKey )
{
	CPipelineState* pPipelineState = FindPipelineState( strKey );

	if ( pPipelineState )
		return pPipelineState;

	pPipelineState = new CPipelineState;

	if ( !pPipelineState->Init() )
	{
		SAFE_RELEASE( pPipelineState );
		return NULL;
	}

	m_mapPipelineState[strKey] = pPipelineState;

	return pPipelineState;
}

CPipelineState * CPipelineStateManager::FindPipelineState( const string & strKey )
{
	auto iter = m_mapPipelineState.find( strKey );

	if ( iter == m_mapPipelineState.end() )
		return NULL;

	return iter->second;
}
