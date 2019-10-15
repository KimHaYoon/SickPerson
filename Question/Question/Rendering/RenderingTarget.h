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
	ID3D12Resource*					m_pTargetTex[2];
	ID3D12DescriptorHeap*		m_pTargetView;			// ·»´õ Å¸°Ù ºä
	ID3D12DescriptorHeap*		m_pTargetSRV;
	ID3D12Resource*					m_pDepthTex;
	ID3D12DescriptorHeap*		m_pDepthView;
	UINT										m_iDSVSize;
	UINT										m_iRTVSize;

	ID3D12DescriptorHeap*		m_pOldTargetView;
	ID3D12DescriptorHeap*		m_pOldDepth;
	float		m_fClearColor[4];
	Vector3		m_vPos;
	Vector3		m_vScale;
	bool		m_bDebug;
	class CMesh*	m_pMesh;
	class CShader*	m_pShader;
	D3D12_INPUT_LAYOUT_DESC*	m_pLayout;

public:
	ID3D12DescriptorHeap* GetRenderTargetView()	const;
	ID3D12DescriptorHeap* GetDepthStencilView()	const;
	ID3D12DescriptorHeap*	GetShaderResourceView() const;
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