#pragma once
#include "../Obj.h"

class CPipelineState	: public CObj
{
private:
	friend class CPipelineStateManager;
	
private:
	CPipelineState();
	~CPipelineState();

private:
	ID3D12PipelineState*					m_pPipelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC		m_tDesc;

public:
	bool Init();

	void SetInputLayout( D3D12_INPUT_LAYOUT_DESC tInputLayout );
	void CreatePipeline();
};

