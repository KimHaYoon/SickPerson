#pragma once
#include "../Obj.h"

Engine_BEGIN

class Engine_DLL CRenderingTarget :
	public CObj
{
	friend class CRenderManager;

private:
	CRenderingTarget();
	~CRenderingTarget();

private:
	ID3D11Texture2D*			m_pTargetTex;
	ID3D11RenderTargetView*		m_pTargetView;
	ID3D11ShaderResourceView*	m_pTargetSRV;
	ID3D11Texture2D*			m_pDepthTex;
	ID3D11DepthStencilView*		m_pDepthView;

	ID3D11RenderTargetView*		m_pOldTargetView;
	ID3D11DepthStencilView*		m_pOldDepth;
	float		m_fClearColor[4];
	Vector3		m_vPos;
	Vector3		m_vScale;
	bool		m_bDebug;
	class CMesh*	m_pMesh;
	class CShader*	m_pShader;
	ID3D11InputLayout*	m_pLayout;

public:
	ID3D11RenderTargetView* GetRenderTargetView()	const;
	ID3D11DepthStencilView* GetDepthStencilView()	const;
	ID3D11ShaderResourceView*	GetShaderResourceView() const;
	bool GetDebugEnable()	const;

public:
	void SetDebugEnable(bool bDebug);
	bool CreateTarget(unsigned int iW, unsigned int iH, DXGI_FORMAT eFmt,
		const Vector4& vClearColor = Vector4::Zero,
		DXGI_FORMAT eDepthFmt = DXGI_FORMAT_UNKNOWN);
	void SetDebugInfo(const Vector3& vPos, const Vector3& vScale);
	void SetTarget();
	void ResetTarget();
	void SaveFile(const wchar_t* pFileName, const string& strPathKey = DATA_PATH);
	void SaveFileFromFullPath(const wchar_t* pFullPath);
	void ClearTarget();
	void DebugRender(float fTime);
	void SetShader(int iRegister);
	void ResetShader(int iRegister);
};

Engine_END