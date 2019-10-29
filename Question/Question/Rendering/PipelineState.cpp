#include "PipelineState.h"
#include "../RootSignature.h"
#include "../Device.h"
#include "BlendState.h"
#include "RasterizerState.h"
#include "DepthStencilState.h"
#include "Shader.h"

CPipelineState::CPipelineState():
	m_pPipelineState(NULL)
{
}


CPipelineState::~CPipelineState()
{
	SAFE_RELEASE( m_pPipelineState );
}

bool CPipelineState::Init()
{
	//D3D12_GRAPHICS_PIPELINE_STATE_DESC	±¸Á¶Ã¼ Ã¤¿ì±â
	//m_tDesc = new D3D12_GRAPHICS_PIPELINE_STATE_DESC;

	m_tDesc.pRootSignature = ROOTSIGNATURE;
	m_tDesc.VS = D3D12_SHADER_BYTECODE{};			// Á¤Á¡¼ÎÀÌ´õ
	m_tDesc.PS = D3D12_SHADER_BYTECODE{};			// ÇÈ¼¿¼ÎÀÌ´õ
	m_tDesc.GS = D3D12_SHADER_BYTECODE{};			// ±âÇÏ¼ÎÀÌ´õ
	m_tDesc.HS = D3D12_SHADER_BYTECODE{};			// µ¤°³¼ÎÀÌ´õ
	m_tDesc.DS = D3D12_SHADER_BYTECODE{};			// ¿µ¿ª¼ÎÀÌ´õ
	m_tDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	m_tDesc.NumRenderTargets = 1;
	m_tDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_tDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	m_tDesc.SampleDesc.Count = 1;
	m_tDesc.SampleDesc.Quality = 0;
	m_tDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	m_tDesc.NodeMask = 0;
	m_tDesc.SampleMask = UINT_MAX;
	m_tDesc.StreamOutput = D3D12_STREAM_OUTPUT_DESC{};
	m_tDesc.CachedPSO = D3D12_CACHED_PIPELINE_STATE{};

	
	return true;
}

void CPipelineState::SetInputLayout( D3D12_INPUT_LAYOUT_DESC * pInputLayout )
{
	m_tDesc.InputLayout = *pInputLayout;
}

void CPipelineState::SetRenderState( CRenderState * pRenderState[RST_END] )
{
	m_tDesc.RasterizerState = ( ( CRasterizerState* )pRenderState[RST_RASTERIZER] )->GetDesc();
	m_tDesc.DepthStencilState = ( ( CDepthStencilState* )pRenderState[RST_DEPTH] )->GetDesc();
	m_tDesc.BlendState = ( ( CBlendState* )pRenderState[RST_BLEND] )->GetDesc();
}

void CPipelineState::SetRenderState( CRenderState * pRenderState )
{
	if ( pRenderState->GetType() == RST_BLEND )
	{
		m_tDesc.RasterizerState = ((CRasterizerState*)pRenderState)->GetDesc();
	}

	else if (pRenderState->GetType() == RST_DEPTH )
	{
		m_tDesc.DepthStencilState = ( ( CDepthStencilState* )pRenderState )->GetDesc();
	}

	else
	{
		m_tDesc.BlendState = ( ( CBlendState* )pRenderState )->GetDesc();
	}
}

void CPipelineState::SetShader( CShader * pShader )
{
	m_tDesc.VS = pShader->GetVertexShader();
	m_tDesc.PS = pShader->GetPixelShader();
	m_tDesc.GS = pShader->GetGeoShader();
}

void CPipelineState::CreatePipeline()
{
	HRESULT hResult = DEVICE->CreateGraphicsPipelineState( &m_tDesc, __uuidof( ID3D12PipelineState ), (void**)&m_pPipelineState );
	m_pPipelineState->SetName( ( LPCWSTR )m_strTag.c_str() );
}

void CPipelineState::SetTag( const string & strTag )
{
	m_strTag = strTag;
}

ID3D12PipelineState * CPipelineState::GetPipelineState() const
{
	return m_pPipelineState;
}

D3D12_GRAPHICS_PIPELINE_STATE_DESC * CPipelineState::GetPipelineStateDesc() const
{
	return (D3D12_GRAPHICS_PIPELINE_STATE_DESC*)&m_tDesc;
}
