#pragma once
#include "../Obj.h"

Engine_BEGIN

class Engine_DLL CShader :
	public CObj
{
private:
	friend class CShaderManager;

private:
	CShader();
	~CShader();

private:
	ID3D11VertexShader*		m_pVS;
	ID3DBlob*	m_pVSBlob;

	ID3D11PixelShader*		m_pPS;
	ID3DBlob*	m_pPSBlob;

	ID3D11GeometryShader*	m_pGS;
	ID3DBlob*	m_pGSBlob;

	string		m_strKey;

public:
	string GetKey()	const;
	void* GetShaderByteCode();
	int GetShaderByteCodeLength();

public:
	bool LoadShader(const string& strKey, TCHAR* pFileName,
		char* pEntry[ST_MAX],
		const string& strPathKey = SHADER_PATH);
	bool LoadVertexShader(const string& strKey, TCHAR* pFileName,
		char* pEntry,
		const string& strPathKey = SHADER_PATH);
	bool LoadPixelShader(const string& strKey, TCHAR* pFileName,
		char* pEntry,
		const string& strPathKey = SHADER_PATH);
	bool LoadGeometryShader(const string& strKey, TCHAR* pFileName,
		char* pEntry,
		const string& strPathKey = SHADER_PATH);

public:
	void SetShader();
};

Engine_END
