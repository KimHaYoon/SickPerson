#pragma once
#include "Component.h"

class CRenderer :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CRenderer();
	CRenderer(const CRenderer& renderer);
	~CRenderer();

private:
	class CMesh*				m_pMesh;
	class CShader*				m_pShader;
	D3D12_INPUT_LAYOUT_DESC*	m_pInputLayout;
	class CPipelineState*		m_pPipeline;
	class CRenderState*			m_pRenderState[RST_END];
	bool	m_bAlpha;

public:
	void SetMesh(const string& strKey);
	void SetMesh(class CMesh* pMesh);
	void SetShader(const string& strKey);
	void SetShader(class CShader* pShader);
	void SetInputLayout(const string& strKey);
	void SetInputLayout( D3D12_INPUT_LAYOUT_DESC* pLayout);
	void SetRenderState( const string& strKey );

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CRenderer* Clone();

private:
	void UpdateTransform();
};