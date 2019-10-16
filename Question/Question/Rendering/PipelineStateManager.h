#pragma once
#include "../Engine.h"

class CPipelineStateManager
{
private:
	unordered_map<string, class CPipelineState*> m_mapPipelineState;

public:
	bool Init();

	void CreatePipelineState(const string& strKey);
	class CPipelineState*	FindPipelineState( const string& strKey );

	DECLARE_SINGLE(CPipelineStateManager)
};

