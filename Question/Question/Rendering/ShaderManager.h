#pragma once

#include "../Engine.h"

class  CShaderManager
{
private:
	unordered_map<string, class CShader*>	m_mapShader;
	unordered_map<string, D3D12_INPUT_LAYOUT_DESC>	m_mapLayout;
	unordered_map<string, PCONSTANTBUFFER>	m_mapCBuffer;
	vector<D3D12_INPUT_ELEMENT_DESC>	m_vecInputDesc;
	int			m_iInputSize;

public:
	bool Init();
	class CShader* LoadShader( const string& strKey, wchar_t* pFileName,
		char* pEntry[ST_MAX],
		const string& strPathKey = SHADER_PATH );

	class CShader* FindShader( const string& strKey );

	void AddInputDesc( char* pName, int iSemanticIdx, DXGI_FORMAT eFmt,
		int iInputSlot, int iSize, D3D12_INPUT_CLASSIFICATION eClass,
		int iInstanceDataStep );
	bool CreateInputLayout( const string& strKey, const string& strShaderKey );
	void SetInputLayout( const string& strKey );

	D3D12_INPUT_LAYOUT_DESC* FindInputLayout( const string& strKey );

	bool CreateCBuffer( const string& strKey, int iRegister,
		int iSize );
	void UpdateCBuffer( const string& strKey, void* pData,
		int iShaderConstantType );

private:
	PCONSTANTBUFFER FindCBuffer( const string& strKey );

private:
	ID3D12Resource* CreateBufferResource( void *pData, UINT nBytes, D3D12_HEAP_TYPE d3dHeapType,
		D3D12_RESOURCE_STATES d3dResourceStates, ID3D12Resource **ppd3dUploadBuffer );

	DECLARE_SINGLE( CShaderManager )
};