#include "Terrain2D.h"
#include "Tile.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"

Engine_USING

CTerrain2D::CTerrain2D()
{
	SetTag("Terrain2D");
	SetTypeName("CTerrain2D");
	SetTypeID<CTerrain2D>();
	m_eType = CT_TERRAIN2D;

	m_eTileType = TILE_NONE;
	m_iTileNumX = 0;
	m_iTileNumY = 0;
	m_iTileSizeX = 0;
	m_iTileSizeY = 0;
}

CTerrain2D::CTerrain2D(const CTerrain2D & terrain)	:
	CComponent(terrain)
{
	*this = terrain;

	m_vecTile.clear();
	for (size_t i = 0; i < terrain.m_vecTile.size(); ++i)
	{
		m_vecTile.push_back(terrain.m_vecTile[i]->Clone());
	}
}

CTerrain2D::~CTerrain2D()
{
	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		CGameObject::EraseObj(m_vecTile[i]);
	}

	Safe_Release_VecList(m_vecTile);
}

unsigned int CTerrain2D::GetTileNumX() const
{
	return m_iTileNumX;
}

unsigned int CTerrain2D::GetTileNumY() const
{
	return m_iTileNumY;
}

unsigned int CTerrain2D::GetTileSizeX() const
{
	return m_iTileSizeX;
}

unsigned int CTerrain2D::GetTileSizeY() const
{
	return m_iTileSizeY;
}

TERRAIN2D_TILE_TYPE CTerrain2D::GetTileType() const
{
	return m_eTileType;
}

const vector<class CGameObject*>* CTerrain2D::GetTileList()
{
	return &m_vecTile;
}

void CTerrain2D::SetTileInfo(TERRAIN2D_TILE_TYPE eType,
	unsigned int iNumX, unsigned int iNumY, unsigned int iSizeX,
	unsigned int iSizeY)
{
	m_eTileType = eType;
	m_iTileNumX = iNumX;
	m_iTileNumY = iNumY;
	m_iTileSizeX = iSizeX;
	m_iTileSizeY = iSizeY;

	Safe_Release_VecList(m_vecTile);

	m_vecTile.reserve(iNumX * iNumY);

	switch (eType)
	{
	case TILE_ISOMETRIC:
		SetIsometricTile();
		break;
	case TILE_RECT:
		SetRectTile();
		break;
	}
}

void CTerrain2D::LoadTileInfo(const char * pFileName, const string & strPathKey)
{
}

void CTerrain2D::SaveTileInfo(const char * pFileName, const string & strPathKey)
{
}

void CTerrain2D::LoadTileInfoFromFullPath(const char * pFileName)
{
}

void CTerrain2D::SaveTileInfoToFullPath(const char * pFileName)
{
}

bool CTerrain2D::Init()
{
	return true;
}

void CTerrain2D::Input(float fTime)
{
	for (int i = 0; i < m_iTileNumY; ++i)
	{
		for (int j = 0; j < m_iTileNumX; ++j)
		{
			m_vecTile[i * m_iTileNumX + j]->Input(fTime);
		}
	}
}

int CTerrain2D::Update(float fTime)
{
	for (int i = 0; i < m_iTileNumY; ++i)
	{
		for (int j = 0; j < m_iTileNumX; ++j)
		{
			m_vecTile[i * m_iTileNumX + j]->Update(fTime);
		}
	}

	return 0;
}

int CTerrain2D::LateUpdate(float fTime)
{
	for (int i = 0; i < m_iTileNumY; ++i)
	{
		for (int j = 0; j < m_iTileNumX; ++j)
		{
			m_vecTile[i * m_iTileNumX + j]->LateUpdate(fTime);
		}
	}

	return 0;
}

void CTerrain2D::Collision(float fTime)
{
	/*for (int i = 0; i < m_iTileNumY; ++i)
	{
		for (int j = 0; j < m_iTileNumX; ++j)
		{
			m_vecTile[i * m_iTileNumX + j]->Collision(fTime);
		}
	}*/
}

void CTerrain2D::Render(float fTime)
{
	for (int i = 0; i < m_iTileNumY; ++i)
	{
		for (int j = 0; j < m_iTileNumX; ++j)
		{
			m_vecTile[i * m_iTileNumX + j]->Render(fTime);
		}
	}
}

CTerrain2D * CTerrain2D::Clone()
{
	return new CTerrain2D(*this);
}

void CTerrain2D::Save(FILE * pFile)
{
	CComponent::Save(pFile);

	fwrite(&m_eTileType, 4, 1, pFile);
	fwrite(&m_iTileNumX, 4, 1, pFile);
	fwrite(&m_iTileNumY, 4, 1, pFile);
	fwrite(&m_iTileSizeX, 4, 1, pFile);
	fwrite(&m_iTileSizeY, 4, 1, pFile);

	int	iCount = m_vecTile.size();

	fwrite(&iCount, 4, 1, pFile);

	for (int i = 0; i < iCount; ++i)
	{
		m_vecTile[i]->Save(pFile);
	}
}

void CTerrain2D::Load(FILE * pFile)
{
	CComponent::Load(pFile);

	fread(&m_eTileType, 4, 1, pFile);
	fread(&m_iTileNumX, 4, 1, pFile);
	fread(&m_iTileNumY, 4, 1, pFile);
	fread(&m_iTileSizeX, 4, 1, pFile);
	fread(&m_iTileSizeY, 4, 1, pFile);

	int	iCount;

	fread(&iCount, 4, 1, pFile);

	Safe_Release_VecList(m_vecTile);

	for (int i = 0; i < iCount; ++i)
	{
		CGameObject*	pTileObj = CGameObject::CreateObject("Tile");
		
		pTileObj->SetScene(m_pScene);
		pTileObj->SetLayer(m_pLayer);

		pTileObj->Load(pFile);
		
		m_vecTile.push_back(pTileObj);
	}
}

void CTerrain2D::SetRectTile()
{
}

void CTerrain2D::SetIsometricTile()
{
	for (unsigned int i = 0; i < m_iTileNumY; ++i)
	{
		for (unsigned int j = 0; j < m_iTileNumX; ++j)
		{
			CGameObject*	pTileObj = CGameObject::CreateObject("Tile");

			pTileObj->SetScene(m_pScene);
			pTileObj->SetLayer(m_pLayer);

			CTransform*	pTransform = pTileObj->GetTransform();

			pTransform->SetPivot(0.f, 0.f, 0.f);
			pTransform->SetWorldScale(m_iTileSizeX, m_iTileSizeY, 1.f);
			if (i % 2 == 0)
				pTransform->SetWorldPos(j * m_iTileSizeX, i * (m_iTileSizeY / 2), 0.f);

			else
				pTransform->SetWorldPos(m_iTileSizeX / 2.f + j * m_iTileSizeX, i * (m_iTileSizeY / 2), 0.f);

			SAFE_RELEASE(pTransform);

			CTile*	pTile = pTileObj->AddComponent<CTile>("Tile");

			SAFE_RELEASE(pTile);

			m_vecTile.push_back(pTileObj);

			//SAFE_RELEASE(pTileObj);
		}
	}
}

int CTerrain2D::GetTileIndex(const Vector3 & vPos)
{
	return GetTileIndex(vPos.x, vPos.y);
}

int CTerrain2D::GetTileIndex(const Vector2 & vPos)
{
	return GetTileIndex(vPos.x, vPos.y);
}

int CTerrain2D::GetTileIndex(const POINT & ptPos)
{
	return GetTileIndex(ptPos.x, ptPos.y);
}

int CTerrain2D::GetTileIndex(float x, float y)
{
	if (m_iTileNumX == 0 || m_iTileNumY == 0 || m_iTileSizeX == 0 ||
		m_iTileSizeY == 0)
		return -1;

	int	idxX = GetTileIndexX(x, y);
	if (idxX == -1)
		return -1;

	int	idxY = GetTileIndexY(x, y);
	if (idxY == -1)
		return -1;

	return idxY * m_iTileNumX + idxX;
}

int CTerrain2D::GetTileIndexX(float x, float y)
{
	if (m_iTileNumX == 0 || m_iTileNumY == 0 || m_iTileSizeX == 0 ||
		m_iTileSizeY == 0)
		return -1;

	if (m_eTileType == TILE_RECT)
		return (int)x / m_iTileSizeX;

	int	iY = GetTileIndexY(x, y);

	if (iY < 0 || iY >= m_iTileNumY)
		return -1;

	int	idx = -1;
	if (iY % 2 == 0)
		idx = (int)x / m_iTileSizeX;

	else
		idx = (int)(x - m_iTileSizeX / 2.f) / m_iTileSizeX;

	if (idx < 0 || idx >= m_iTileNumX)
		return -1;

	return idx;
}

int CTerrain2D::GetTileIndexY(float x, float y)
{
	if (m_iTileNumX == 0 || m_iTileNumY == 0 || m_iTileSizeX == 0 ||
		m_iTileSizeY == 0)
		return -1;

	if (m_eTileType == TILE_RECT)
		return (int)y / m_iTileSizeY;

	// x, y 좌표를 타일의 1 by 1 공간으로 만들어준다.
	float	fX = x / m_iTileSizeX;
	float	fY = y / m_iTileSizeY;

	// 0 ~ 1 사이의 타일 내에서의 거리값을 구한다.
	fX -= (int)fX;
	fY -= (int)fY;

	// 사각 타일에서의 Y 인덱스 값을 구해준다.
	int	iStartY = (int)y / m_iTileSizeY;

	// 사각 타일에서의 X 인덱스 값을 구해준다.
	int	iStartX = (int)x / m_iTileSizeX;

	// 위쪽 라인일 경우
	if (fY < 0.5f)
	{
		// 좌표지점이 좌 상단 지점일 경우
		if (fY < 0.5f - fX)
		{
			if (iStartY == 0)
				return -1;

			else if (iStartY == 1)
			{
				if (iStartX == 0)
					return -1;

				else
					return 1;
			}

			else
			{
				if (iStartX == 0)
					return -1;

				else
					return (iStartY - 1) * 3 - (iStartY - 2);
			}
		}

		// 좌표지점이 우 상단 지점일 경우
		else if (fY < fX - 0.5f)
		{
			if (iStartY == 0)
				return -1;

			else if (iStartY == 1)
			{
				if (iStartX > m_iTileNumX - 1)
					return -1;

				return 1;
			}

			else
			{
				if (iStartX > m_iTileNumX - 1)
					return -1;

				return (iStartY - 1) * 3 - (iStartY - 2);
			}
		}

		else
		{
			if (iStartY == 0)
				return 0;

			else if (iStartY == 1)
				return 2;

			else
				return (iStartY - 1) * 3 - (iStartY - 2) + 1;
		}
	}

	// 아래쪽 라인일 경우
	else if (fY > 0.5f)
	{
		// 좌표지점이 좌 하단 지점일 경우
		if (fY - 0.5f > fX)
		{
			if (iStartY == 0)
			{
				if (iStartX == 0)
					return -1;

				else
					return 1;
			}

			else if (iStartY == 1)
			{
				if (iStartX == 0)
					return -1;

				else
					return 3;
			}

			else
			{
				if (iStartX == 0)
					return -1;

				return (iStartY - 1) * 3 - (iStartY - 2) + 2;
			}
		}

		// 좌표지점이 우 하단 지점일 경우
		else if (fY - 0.5f > 1.f - fX)
		{
			if (iStartY == 0)
			{
				if (iStartX > m_iTileNumX - 1)
					return -1;

				else
					return 1;
			}

			else if (iStartY == 1)
			{
				if (iStartX > m_iTileNumX - 1)
					return -1;

				else
					return 3;
			}

			else
			{
				if (iStartX > m_iTileNumX - 1)
					return -1;

				return (iStartY - 1) * 3 - (iStartY - 2) + 2;
			}
		}

		else
		{
			if (iStartY == 0)
				return 0;

			else if (iStartY == 1)
				return 2;

			else
				return (iStartY - 1) * 3 - (iStartY - 2) + 1;
		}
	}

	else
	{
		if (iStartY == 0)
			return 0;

		else if (iStartY == 1)
			return 2;

		else
			return (iStartY - 1) * 3 - (iStartY - 2) + 1;
	}

	return -1;
}

CTile * CTerrain2D::GetTile(float x, float y)
{
	int	idx = GetTileIndex(x, y);

	if (idx == -1)
		return NULL;

	return m_vecTile[idx]->FindComponentFromType<CTile>(CT_TILE);
}
