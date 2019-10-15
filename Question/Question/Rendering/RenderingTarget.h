#pragma once
#include "../Obj.h"

class CRenderingTarget :
	public CObj
{
	friend class CRenderManager;

private:
	CRenderingTarget();
	~CRenderingTarget();

private:
	ID3D12Texture2D*			m_pTargetTex;
	ID3D12RenderTargetView*		m_pTargetView;
	ID3D12ShaderResourceView*	m_pTargetSRV;
	ID3D12Texture2D*			m_pDepthTex;
	ID3D12DepthStencilView*		m_pDepthView;

	ID3D12RenderTargetView*		m_pOldTargetView;
	ID3D12DepthStencilView*		m_pOldDepth;
	float		m_fClearColor[4];
	Vector3		m_vPos;
	Vector3		m_vScale;
	bool		m_bDebug;
	class CMesh*	m_pMesh;
	class CShader*	m_pShader;
	ID3D12InputLayout*	m_pLayout;

public:
	ID3D12RenderTargetView* GetRenderTargetView()	const;
	ID3D12DepthStencilView* GetDepthStencilView()	const;
	ID3D12ShaderResourceView*	GetShaderResourceView() const;
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