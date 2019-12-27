#include "Sampler.h"
#include "../Device.h"

GAME_USING

CSampler::CSampler() :
	m_pSampler( NULL )
{
}

CSampler::~CSampler()
{
	SAFE_RELEASE( m_pSampler );
}

string CSampler::GetKey() const
{
	return m_strKey;
}

bool CSampler::CreateSampler( const string& strKey, D3D11_FILTER eFilter,
	D3D11_TEXTURE_ADDRESS_MODE eAddrU,
	D3D11_TEXTURE_ADDRESS_MODE eAddrV,
	D3D11_TEXTURE_ADDRESS_MODE eAddrW )
{
	m_strKey = strKey;

	D3D11_SAMPLER_DESC	tDesc = {};

	tDesc.Filter = eFilter;
	tDesc.AddressU = eAddrU;
	tDesc.AddressV = eAddrV;
	tDesc.AddressW = eAddrW;

	if ( FAILED( DEVICE->CreateSamplerState( &tDesc, &m_pSampler ) ) )
		return false;

	return true;
}

void CSampler::SetSampler( int iRegister, int iShaderConstantType )
{
	if ( iShaderConstantType & SCT_VERTEX )
		CONTEXT->VSSetSamplers( iRegister, 1, &m_pSampler );

	if ( iShaderConstantType & SCT_PIXEL )
		CONTEXT->PSSetSamplers( iRegister, 1, &m_pSampler );
}