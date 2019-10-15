#pragma once
#include "../Obj.h"

class CShader :
	public CObj
{
private:
	friend class CShaderManager;

private:
	CShader();
	~CShader();

private:
	ID3DBlob*	m_pVSBlob;
	D3D12_SHADER_BYTECODE	m_tVSByteCode;

	ID3DBlob*	m_pPSBlob;
	D3D12_SHADER_BYTECODE	m_tPSByteCode;

	ID3DBlob*	m_pGSBlob;
	D3D12_SHADER_BYTECODE	m_tGSByteCode;

	string		m_strKey;

public:
	string GetKey()	const;
	void* GetShaderByteCode();
	int GetShaderByteCodeLength();

	D3D12_SHADER_BYTECODE	GetVertexShader()	const;
	D3D12_SHADER_BYTECODE	GetPixelShader()	const;
	D3D12_SHADER_BYTECODE	GetGeoShader()	const;

public:
	bool LoadShader( const string& strKey, wchar_t* pFileName,
		char* pEntry[ST_MAX],
		const string& strPathKey = SHADER_PATH );
	bool LoadVertexShader( const string& strKey, wchar_t* pFileName,
		char* pEntry,
		const string& strPathKey = SHADER_PATH );
	bool LoadPixelShader( const string& strKey, wchar_t* pFileName,
		char* pEntry,
		const string& strPathKey = SHADER_PATH );
	bool LoadGeometryShader( const string& strKey, wchar_t* pFileName,
		char* pEntry,
		const string& strPathKey = SHADER_PATH );

public:
	void SetShader( ID3D12PipelineState * pPipeline);
};