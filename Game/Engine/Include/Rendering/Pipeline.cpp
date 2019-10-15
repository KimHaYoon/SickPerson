#include "Pipeline.h"
#include "Root.h"
#include "Shader.h"
#include "RenderManager.h"
#include "RasterizerState.h"
#include "DepthStencilState.h"
#include "BlendState.h"
#include "../Device.h"

Engine_USING

DEFINITION_SINGLE( CPipeline )

CPipeline::CPipeline()
{
}


CPipeline::~CPipeline()
{
	Safe_Release_Map( m_mapPipline );
}

bool CPipeline::Init()
{
	return true;
}

ID3D12PipelineState* CPipeline::CreatePipeline( const string & strTag, const CShader * pShader, const CRenderState * pRenderState[RST_END], const D3D12_INPUT_LAYOUT_DESC* pInputLayout )
{
	ID3D12PipelineState* pPipeline = NULL;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory( &d3dPipelineStateDesc, sizeof( D3D12_GRAPHICS_PIPELINE_STATE_DESC ) );
	d3dPipelineStateDesc.pRootSignature = GET_SINGLE( CRoot )->FindRootSignature( strTag );
	d3dPipelineStateDesc.VS = pShader->GetVertexShader();
	d3dPipelineStateDesc.PS = pShader->GetPixelShader();
	d3dPipelineStateDesc.GS = pShader->GetGeoShader();

	d3dPipelineStateDesc.RasterizerState = ( ( CRasterizerState* )&pRenderState[RST_RASTERIZER] )->GetRasterizerDesc();
	d3dPipelineStateDesc.BlendState = ( ( CBlendState* )&pRenderState[RST_BLEND] )->GetBlendDesc();
	d3dPipelineStateDesc.DepthStencilState = ( ( CDepthStencilState* )&pRenderState[RST_DEPTH] )->GetDepthStencilDesc();
	d3dPipelineStateDesc.InputLayout = *pInputLayout;

	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;

	if ( FAILED( DEVICE->CreateGraphicsPipelineState( &d3dPipelineStateDesc, __uuidof( ID3D12PipelineState ), ( void ** )&pPipeline ) ) )
		return;

	m_mapPipline[strTag] = pPipeline;

	return pPipeline;
}
