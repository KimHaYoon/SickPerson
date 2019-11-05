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
	ID3D11SamplerState*		m_pSampler;
	string					m_strKey;

public:
	string GetKey()	const;

public:
	bool CreateSampler(const string& strKey, D3D11_FILTER eFilter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_MODE eAddrU = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE eAddrV = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE eAddrW = D3D11_TEXTURE_ADDRESS_WRAP);
	void SetSampler(int iRegister,
		int iShaderConstantType);
};

Engine_END
