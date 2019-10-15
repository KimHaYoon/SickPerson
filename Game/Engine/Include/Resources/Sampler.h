#pragma once
#include "../Obj.h"

Engine_BEGIN

class Engine_DLL CSampler :
	public CObj
{
private:
	friend class CResourcesManager;

private:
	CSampler();
	~CSampler();

private:
	ID3D12SamplerState*		m_pSampler;
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

Engine_END
