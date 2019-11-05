#pragma once
#include "Component.h"

Engine_BEGIN

class Engine_DLL CTerrain2D :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CTerrain2D();
	CTerrain2D(const CTerrain2D& terrain);
	~CTerrain2D();

private:
	TERRAIN2D_TILE_TYPE		m_eTileType;
	unsigned int			m_iTileNumX;
	unsigned int			m_iTileNumY;
	unsigned int			m_iTileSizeX;
	unsigned int			m_iTileSizeY;
	vector<class CGameObject*>	m_vecTile;

public:
	unsigned int GetTileNumX()	const;
	unsigned int GetTileNumY()	const;
	unsigned int GetTileSizeX()	const;
	unsigned int GetTileSizeY()	const;
	TERRAIN2D_TILE_TYPE GetTileType()	const;
	const vector<class CGameObject*>* GetTileList();

public:
	void SetTileInfo(TERRAIN2D_TILE_TYPE eType, unsigned int iNumX,
		unsigned int iNumY, unsigned int iSizeX,
		unsigned int iSizeY);
	void LoadTileInfo(const char* pFileName, const string& strPathKey = DATA_PATH);
	void SaveTileInfo(const char* pFileName, const string& strPathKey = DATA_PATH);
	void LoadTileInfoFromFullPath(const char* pFileName);
	void SaveTileInfoToFullPath(const char* pFileName);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CTerrain2D* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

private:
	void SetRectTile();
	void SetIsometricTile();

public:
	int GetTileIndex(const Vector3& vPos);
	int GetTileIndex(const Vector2& vPos);
	int GetTileIndex(const POINT& ptPos);
	int GetTileIndex(float x, float y);
	int GetTileIndexX(float x, float y);
	int GetTileIndexY(float x, float y);
	class CTile* GetTile(float x, float y);
};

Engine_END
