#pragma once
#include "../Obj.h"

class CSampler :
	public CObj
{
private:
	friend class CResourcesManager;

private:
	CSampler();
	~CSampler();

private:
	D3D12_STATIC_SAMPLER_DESC		m_pSampler;
	string					m_strKey;

public:
	string GetKey()	const;

public:
	bool CreateSampler(const string& strKey, D3D12_FILTER eFilter = D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE eAddrU = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE eAddrV = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE eAddrW = D3D12_TEXTURE_ADDRESS_MODE_WRAP );
	void SetSampler(int iRegister,
		int iShaderConstantType);
};