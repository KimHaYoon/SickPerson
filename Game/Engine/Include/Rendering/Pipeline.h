#pragma once
#include "../Obj.h"

Engine_BEGIN

class Engine_DLL CPipeline : public CObj
{
	DECLARE_SINGLE( CPipeline )

private:
	unordered_map<string, ID3D12PipelineState*> m_mapPipline;

public:
	bool Init();

public:
	ID3D12PipelineState*  CreatePipeline( const string& strTag, const class CShader* pShader, const class CRenderState* pRenderState[RST_END], const D3D12_INPUT_LAYOUT_DESC* pInputLayout );
};

Engine_END