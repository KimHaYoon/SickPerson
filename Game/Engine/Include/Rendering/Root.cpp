#include "Root.h"
#include "../Device.h"
Engine_USING

DEFINITION_SINGLE( CRoot )

CRoot::CRoot()
{
}


CRoot::~CRoot()
{
}


bool CRoot::Init()
{
	return true;
}


ID3D12RootSignature* CRoot::CreateRootSignature( const string & strTag, const D3D12_ROOT_PARAMETER * pParameter, const D3D12_ROOT_SIGNATURE_DESC &	tRootSignature )
{
	ID3D12RootSignature*	pRootSignature = NULL;

	ID3DBlob*	pSignatureBlob = NULL;
	ID3DBlob*	pErrBlob = NULL;

	D3D12SerializeRootSignature( &tRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &pSignatureBlob, &pErrBlob );

	if(FAILED(DEVICE->CreateRootSignature( 0, pSignatureBlob->GetBufferPointer(), pSignatureBlob->GetBufferSize(), __uuidof( ID3D12RootSignature ), ( void** )&pRootSignature )))
		return;

	if ( pSignatureBlob ) 
		pSignatureBlob->Release();
	if ( pErrBlob ) 
		pErrBlob->Release();

	m_mapRootSignature[strTag] = pRootSignature;
}

ID3D12RootSignature * CRoot::FindRootSignature( const string & strTag )
{
	auto iter = m_mapRootSignature.find( strTag );

	return iter->second;
}