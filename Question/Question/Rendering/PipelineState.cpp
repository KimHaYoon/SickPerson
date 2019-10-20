#include "PipelineState.h"
#include "../RootSignature.h"
#include "../Device.h"
#include "BlendState.h"
#include "RasterizerState.h"
#include "DepthStencilState.h"

CPipelineState::CPipelineState():
	m_pPipelineState(NULL)
{
}


CPipelineState::~CPipelineState()
{
	SAFE_RELEASE( m_pPipelineState );
	SAFE_DELETE( m_pDesc );
}

bool CPipelineState::Init()
{
	//D3D12_GRAPHICS_PIPELINE_STATE_DESC	±¸Á¶Ã¼ Ã¤¿ì±â
	m_pDesc = new D3D12_GRAPHICS_PIPELINE_STATE_DESC;

	m_pDesc->pRootSignature = ROOTSIGNATURE;
	m_pDesc->VS = D3D12_SHADER_BYTECODE{};			// Á¤Á¡¼ÎÀÌ´õ
	m_pDesc->PS = D3D12_SHADER_BYTECODE{};			// ÇÈ¼¿¼ÎÀÌ´õ
	m_pDesc->GS = D3D12_SHADER_BYTECODE{};			// ±âÇÏ¼ÎÀÌ´õ
	m_pDesc->HS = D3D12_SHADER_BYTECODE{};			// µ¤°³¼ÎÀÌ´õ
	m_pDesc->DS = D3D12_SHADER_BYTECODE{};			// ¿µ¿ª¼ÎÀÌ´õ
	m_pDesc->PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	m_pDesc->NumRenderTargets = 1;
	m_pDesc->RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	for(int i = 1; i < 8; ++i )
		m_pDesc->RTVFormats[i] = DXGI_FORMAT_UNKNOWN;
	m_pDesc->DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	m_pDesc->SampleDesc.Count = 1;
	m_pDesc->SampleDesc.Quality = 0;
	m_pDesc->Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	m_pDesc->NodeMask = 0;
	m_pDesc->SampleMask = UINT_MAX;
	m_pDesc->StreamOutput = D3D12_STREAM_OUTPUT_DESC{};
	m_pDesc->CachedPSO = D3D12_CACHED_PIPELINE_STATE{};

	
	return true;
}

void CPipelineState::SetInputLayout( D3D12_INPUT_LAYOUT_DESC * pInputLayout )
{
	m_pDesc->InputLayout = *pInputLayout;
}

void CPipelineState::SetRenderState( CRenderState * pRenderState[RST_END] )
{
	m_pDesc->RasterizerState = ( ( CRasterizerState* )pRenderState[RST_RASTERIZER] )->GetDesc();
	m_pDesc->DepthStencilState = ( ( CDepthStencilState* )pRenderState[RST_DEPTH] )->GetDesc();
	m_pDesc->BlendState = ( ( CBlendState* )pRenderState[RST_BLEND] )->GetDesc();
}

void CPipelineState::CreatePipeline()
{
	string strTag = m_strTag;
	DEVICE->CreateGraphicsPipelineState( m_pDesc, __uuidof( ID3D12PipelineState ), (void**)&m_pPipelineState );
	if(m_pPipelineState )
		CMDLIST->SetPipelineState( m_pPipelineState );
}

void CPipelineState::SetTag( const string & strTag )
{
	m_strTag = strTag;
}

ID3D12PipelineState * CPipelineState::GetPipelineState() const
{
	return m_pPipelineState;
}

D3D12_GRAPHICS_PIPELINE_STATE_DESC *  CPipelineState::GetPipelineStateDesc() const
{
	return m_pDesc;
}
