#pragma once
#include "../Engine.h"

Engine_BEGIN
class Engine_DLL CRoot
{
	unordered_map<string, ID3D12RootSignature*	>	m_mapRootSignature;

public:
	bool		Init();

public:
	ID3D12RootSignature* CreateRootSignature( const string& strTag, const D3D12_ROOT_PARAMETER*	pParameter, const D3D12_ROOT_SIGNATURE_DESC&	tRootSignature );
	ID3D12RootSignature*	FindRootSignature( const string& strTag );


	DECLARE_SINGLE( CRoot )
};

Engine_END