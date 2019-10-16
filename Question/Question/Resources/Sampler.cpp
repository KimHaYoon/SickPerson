#include "Sampler.h"
#include "../Device.h"

CSampler::CSampler()
{
}

CSampler::~CSampler()
{
}

string CSampler::GetKey() const
{
	return m_strKey;
}

bool CSampler::CreateSampler(const string& strKey, D3D12_FILTER eFilter,
	D3D12_TEXTURE_ADDRESS_MODE eAddrU,
	D3D12_TEXTURE_ADDRESS_MODE eAddrV,
	D3D12_TEXTURE_ADDRESS_MODE eAddrW)
{
	m_strKey = strKey;
	
	m_pSampler.Filter = eFilter;
	m_pSampler.AddressU = eAddrU;
	m_pSampler.AddressV = eAddrV;
	m_pSampler.AddressW = eAddrW;
	m_pSampler.MipLODBias = 0;
	m_pSampler.MaxAnisotropy = 1;
	m_pSampler.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	m_pSampler.MinLOD = 0;
	m_pSampler.MaxLOD = D3D12_FLOAT32_MAX;


	return true;
}

void CSampler::SetSampler(int iRegister, int iShaderConstantType)
{
	if ( iShaderConstantType & SCT_VERTEX )
	{
		m_pSampler.ShaderRegister = iRegister;
		m_pSampler.RegisterSpace = 0;
		m_pSampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	}

	if ( iShaderConstantType & SCT_PIXEL )
	{
		m_pSampler.ShaderRegister = iRegister;
		m_pSampler.RegisterSpace = 0;
		m_pSampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	}
}
