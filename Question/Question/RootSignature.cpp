#include "RootSignature.h"
#include "Device.h"

DEFINITION_SINGLE( CRootSignature );

CRootSignature::CRootSignature()
{
}


CRootSignature::~CRootSignature()
{
	SAFE_DELETE_ARRAY( m_pRootParameter );
	SAFE_RELEASE( m_pRootSignature );
}

bool CRootSignature::Init()
{
	CreateRootParameter( D3D12_ROOT_PARAMETER_TYPE_CBV, 0, 0, D3D12_SHADER_VISIBILITY_ALL );
	CreateRootParameter( D3D12_ROOT_PARAMETER_TYPE_CBV, 1, 0, D3D12_SHADER_VISIBILITY_ALL );
	CreateRootParameter( D3D12_ROOT_PARAMETER_TYPE_CBV, 2, 0, D3D12_SHADER_VISIBILITY_ALL );

	if ( !m_vecRootParameter.empty() )
	{
		m_pRootParameter = new D3D12_ROOT_PARAMETER[m_vecRootParameter.size()];
		for ( int i = 0; i < m_vecRootParameter.size(); ++i )
			m_pRootParameter[i] = m_vecRootParameter[i];
	}

	CreateRootSignature();

	return true;
}

void CRootSignature::CreateDescritorRange( D3D12_DESCRIPTOR_RANGE_TYPE eType, UINT iNumDesc, UINT iBaseShaderRegister, UINT iRegisterSpace, UINT iOffsetInDescFromTableStart )
{
	D3D12_DESCRIPTOR_RANGE	tDesc = {};

	tDesc.RangeType = eType;
	tDesc.NumDescriptors = iNumDesc;
	tDesc.BaseShaderRegister = iBaseShaderRegister;
	tDesc.RegisterSpace = iRegisterSpace;
	tDesc.OffsetInDescriptorsFromTableStart = iOffsetInDescFromTableStart;

	m_vecDescRange.push_back( tDesc );
}

void CRootSignature::CreateRootParameter( D3D12_ROOT_PARAMETER_TYPE eType, UINT iShaderRegister, UINT iRegisterSpace, D3D12_SHADER_VISIBILITY eShaderVisibility )
{
	D3D12_ROOT_PARAMETER	tDesc = {};

	tDesc.ParameterType = eType;
	tDesc.Descriptor.ShaderRegister = iShaderRegister;
	tDesc.Descriptor.RegisterSpace = iRegisterSpace;
	tDesc.ShaderVisibility = eShaderVisibility;

	m_vecRootParameter.push_back( tDesc );
}

void CRootSignature::CreateRootParameteDescTable( D3D12_ROOT_PARAMETER_TYPE eType, UINT iDescRange, D3D12_DESCRIPTOR_RANGE * pDescRange, D3D12_SHADER_VISIBILITY eShaderVisibility )
{
	D3D12_ROOT_PARAMETER	tDesc = {};

	tDesc.ParameterType = eType;
	tDesc.DescriptorTable.NumDescriptorRanges= iDescRange;
	tDesc.DescriptorTable.pDescriptorRanges= pDescRange;
	tDesc.ShaderVisibility = eShaderVisibility;

	m_vecRootParameter.push_back( tDesc );
}

void CRootSignature::CreateRootSignature()
{
	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory( &d3dRootSignatureDesc, sizeof( D3D12_ROOT_SIGNATURE_DESC ) );
	d3dRootSignatureDesc.NumParameters = m_vecRootParameter.size();
	d3dRootSignatureDesc.pParameters = m_pRootParameter;
	d3dRootSignatureDesc.NumStaticSamplers = 0;
	d3dRootSignatureDesc.pStaticSamplers = NULL;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;
	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	::D3D12SerializeRootSignature( &d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		&pd3dSignatureBlob, &pd3dErrorBlob );
	DEVICE->CreateRootSignature( 0, pd3dSignatureBlob->GetBufferPointer(),
		pd3dSignatureBlob->GetBufferSize(), __uuidof( ID3D12RootSignature ), ( void
			** )&m_pRootSignature );
}

ID3D12RootSignature * CRootSignature::GetRootSignature() const
{
	return m_pRootSignature;
}
