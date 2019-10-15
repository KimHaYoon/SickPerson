#include "Sampler.h"
#include "../Device.h"

CSampler::CSampler() :
	m_pSampler(NULL)
{
}

CSampler::~CSampler()
{
	SAFE_RELEASE(m_pSampler);
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

	D3D12_STATIC_SAMPLER_DESC	tDesc = {};

	tDesc.Filter = eFilter;
	tDesc.AddressU = eAddrU;
	tDesc.AddressV = eAddrV;
	tDesc.AddressW = eAddrW;
	tDesc.MipLODBias = 0;
	tDesc.MaxAnisotropy = 1;
	tDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	tDesc.MinLOD = 0;
	tDesc.MaxLOD = D3D12_FLOAT32_MAX;


	if (FAILED(DEVICE->CreateSamplerState(&tDesc, &m_pSampler)))
		return false;

	return true;
}

void CSampler::SetSampler(int iRegister, int iShaderConstantType)
{
	if (iShaderConstantType & SCT_VERTEX)
		CMLIST->VSSetSamplers(iRegister, 1, &m_pSampler);

	if (iShaderConstantType & SCT_PIXEL)
		CMLIST->PSSetSamplers(iRegister, 1, &m_pSampler);
}
