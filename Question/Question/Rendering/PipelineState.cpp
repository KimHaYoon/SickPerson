#include "PipelineState.h"
#include "../RootSignature.h"
#include "../Device.h"

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
	
	return true;
}

void CPipelineState::SetInputLayout( D3D12_INPUT_LAYOUT_DESC tInputLayout )
{
	m_tDesc.InputLayout = tInputLayout;
}

void CPipelineState::CreatePipeline()
{
	DEVICE->CreateGraphicsPipelineState( &m_tDesc, __uuidof( ID3D12PipelineState* ), &m_pPipelineState );
}
