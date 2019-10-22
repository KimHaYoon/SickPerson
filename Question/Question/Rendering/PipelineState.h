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
	D3D12_GRAPHICS_PIPELINE_STATE_DESC*		m_pDesc;
	string									m_strTag;

public:
	bool Init();

	void SetInputLayout( D3D12_INPUT_LAYOUT_DESC* pInputLayout );
	void SetRenderState( class CRenderState* pRenderState[RST_END]);
	void CreatePipeline();			// ������ �ϱ����� ȣ��

	void SetTag( const string& strTag );

	ID3D12PipelineState*	GetPipelineState()	const;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC*	GetPipelineStateDesc() const;
};

