#pragma once

#include "../Engine.h"

Engine_BEGIN

class Engine_DLL CShaderManager
{
private:
	unordered_map<string, class CShader*>	m_mapShader;
	unordered_map<string, ID3D11InputLayout*>	m_mapLayout;
	unordered_map<string, PCONSTANTBUFFER>	m_mapCBuffer;
	vector<D3D11_INPUT_ELEMENT_DESC>	m_vecInputDesc;
	int			m_iInputSize;

public:
	bool Init();
	class CShader* LoadShader(const string& strKey, TCHAR* pFileName,
		char* pEntry[ST_MAX],
		const string& strPathKey = SHADER_PATH);

	class CShader* FindShader(const string& strKey);

	void AddInputDesc(char* pName, int iSemanticIdx, DXGI_FORMAT eFmt,
		int iInputSlot, int iSize, D3D11_INPUT_CLASSIFICATION eClass,
		int iInstanceDataStep);
	bool CreateInputLayout(const string& strKey, const string& strShaderKey);
	void SetInputLayout(const string& strKey);

	ID3D11InputLayout* FindInputLayout(const string& strKey);

	bool CreateCBuffer(const string& strKey, int iRegister,
		int iSize);
	void UpdateCBuffer(const string& strKey, void* pData,
		int iShaderConstantType);

private:
	PCONSTANTBUFFER FindCBuffer(const string& strKey);

	DECLARE_SINGLE(CShaderManager)
};

Engine_END
