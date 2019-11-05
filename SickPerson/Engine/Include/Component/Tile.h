#pragma once
#include "Component.h"

Engine_BEGIN

class Engine_DLL CTile :
	public CComponent
{
private:
	friend class CGameObject;
	friend class CTerrain2D;
	friend class CNavigation;

private:
	CTile();
	CTile(const CTile& tile);
	~CTile();

private:
	TERRAIN2D_TILE_OPTION	m_eOption;
	int			m_iParentIndex;
	int			m_iEdge;
	int			m_iHuristic;
	int			m_iTotal;
//#ifdef _DEBUG
	class CMesh*		m_pMesh;
	class CShader*		m_pShader;
	ID3D11InputLayout*	m_pLayout;
	Vector4				m_vLineColor;
//#endif // _DEBUG


public:
	TERRAIN2D_TILE_OPTION GetOption()	const;

public:
	void SetTileOption(TERRAIN2D_TILE_OPTION eOption);
	void SetLineColor(const Vector4& vColor);
	Vector4 GetLineColor()	const;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CTile* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

Engine_END
