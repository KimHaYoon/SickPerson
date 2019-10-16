#pragma once
#include "../Engine.h"

class CRootSignature
{
private:
	vector<D3D12_DESCRIPTOR_RANGE > m_vecDescRange;
	vector<D3D12_ROOT_PARAMETER > m_vecRootParameter;
	D3D12_ROOT_PARAMETER*	m_pRootParameter;
	ID3D12RootSignature*			m_pRootSignature;

public:
	bool Init();

private:
	void CreateDescritorRange( D3D12_DESCRIPTOR_RANGE_TYPE eType, UINT iNumDesc,
		UINT iBaseShaderRegister, UINT iRegisterSpace, UINT iOffsetInDescFromTableStart );
	void CreateRootParameter( D3D12_ROOT_PARAMETER_TYPE eType, UINT iShaderRegister
		, UINT iRegisterSpace, D3D12_SHADER_VISIBILITY	eShaderVisibility );
	void CreateRootParameteDescTable( D3D12_ROOT_PARAMETER_TYPE eType, UINT iDescRange
		, D3D12_DESCRIPTOR_RANGE* pDescRange, D3D12_SHADER_VISIBILITY	eShaderVisibility );
	void CreateRootSignature();

public:
	ID3D12RootSignature*	GetRootSignature() const;

	DECLARE_SINGLE( CRootSignature )
};

