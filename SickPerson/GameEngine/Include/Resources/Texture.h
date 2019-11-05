#pragma once
#include "../Obj.h"
#include "DirectXTex.h"

Engine_BEGIN

class Engine_DLL CTexture :
	public CObj
{
private:
	friend class CResourcesManager;
	friend class CMaterial;

private:
	CTexture();
	~CTexture();

private:
	vector<ScratchImage*>		m_vecImage;
	ID3D11ShaderResourceView*	m_pSRView;
	string						m_strKey;
	bool						m_bArray;
	vector<string>				m_vecFullPath;
	string						m_strPathKey;

public:
	bool LoadTexture(const string& strKey, const wchar_t* pFileName,
		const string& strPathKey = TEXTURE_PATH);
	bool LoadTexture(const string& strKey, const char* pFileName,
		const string& strPathKey = TEXTURE_PATH);
	bool LoadTexture(const string& strKey, const vector<wstring>& vecFileName,
		const string& strPathKey = TEXTURE_PATH);
	bool LoadTextureFromFullPath(const string& strKey, const char* pFullPath);
	bool LoadTextureFromFullPath(const string& strKey, const vector<string>& vecFullPath);
	void SetTexture(int iRegister, int iShaderConstantType);
	void SetTexturePathKey(const string& strPathKey);
	void ChangeTexturePath(const string& strPath);
	void SetShaderResourceView(ID3D11ShaderResourceView* pSRV);

public:
	void Save(FILE* pFile);
	void Load(FILE* pFile);
};

Engine_END
