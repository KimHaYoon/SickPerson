#include "Navigation.h"
#include "../GameObject/GameObject.h"
#include "Transform.h"
#include "Terrain2D.h"
#include "Tile.h"
#include "Animation2D.h"

Engine_USING

CNavigation::CNavigation()	:
	m_pAnimation(NULL)
{
	SetTag("Navigation");
	SetTypeName("CNavigation");
	SetTypeID<CNavigation>();
	m_eType = CT_NAVIGATION;
}

CNavigation::CNavigation(const CNavigation & Nav)	:
	CComponent(Nav)
{
	FindTileSet();
	m_pAnimation = FindComponentFromType<CAnimation2D>(CT_ANIMATION2D);
	m_strMoveAnimationKey = Nav.m_strMoveAnimationKey;
}

CNavigation::~CNavigation()
{
	SAFE_RELEASE(m_pAnimation);
}

void CNavigation::SetMoveAnimationKey(const string & strKey)
{
	m_strMoveAnimationKey = strKey;
}

void CNavigation::FindTileSet()
{
	CGameObject* pTerrainObj = CGameObject::FindObject(CT_TERRAIN2D);

	if (pTerrainObj)
	{
		CTerrain2D*	pTerrain = pTerrainObj->FindComponentFromType<CTerrain2D>(CT_TERRAIN2D);

		m_eTileType = pTerrain->GetTileType();
		m_iTileNumX = pTerrain->GetTileNumX();
		m_iTileNumY = pTerrain->GetTileNumY();
		m_iTileSizeX = pTerrain->GetTileSizeX();
		m_iTileSizeY = pTerrain->GetTileSizeY();
		const vector<CGameObject*>* pTileList = pTerrain->GetTileList();

		m_vecTile.reserve(pTileList->size());
		m_vecOpen.reserve(pTileList->size() / 2);
		m_vecClose.reserve(pTileList->size() / 2);

		vector<CGameObject*>::const_iterator	iter;
		vector<CGameObject*>::const_iterator	iterEnd = pTileList->end();

		for (iter = pTileList->begin(); iter != iterEnd; ++iter)
		{
			CTile*	pTile = (*iter)->FindComponentFromType<CTile>(CT_TILE);

			m_vecTile.push_back(pTile);

			SAFE_RELEASE(pTile);
		}

		SAFE_RELEASE(pTerrain);

		SAFE_RELEASE(pTerrainObj);
	}
}

bool CNavigation::Init()
{
	FindTileSet();
	m_pAnimation = FindComponentFromType<CAnimation2D>(CT_ANIMATION2D);

	return true;
}

void CNavigation::Input(float fTime)
{
}

int CNavigation::Update(float fTime)
{
	if (m_vecTile.empty())
	{
		FindTileSet();
	}

	if(!m_pAnimation)
		m_pAnimation = FindComponentFromType<CAnimation2D>(CT_ANIMATION2D);

	if (!m_Road.empty())
	{
		// ���⿡ ã�Ƴ��� ���� �̵��ϴ� �ڵ带 �ۼ��Ѵ�.
		// ��ĭ �̵��Ҷ����� ���� ��带 �ϳ��� ����� �Ѵ�.(pop_front())
		int	idx = m_Road.front();

		// �� ����� ���� ��ġ�� ���Ѵ�.
		CTile*	pTile = m_vecTile[idx];
		CTransform*	pTr = pTile->GetTransform();

		Vector3	vCurPos;

		//if (m_Road.size() > 1)
		//{
			vCurPos = pTr->GetWorldPos();
			vCurPos.x += 0.5f * m_iTileSizeX;
			vCurPos.y += 0.5f * m_iTileSizeY;
		//}

		//else
		//	vCurPos = m_vEndPos;

		Vector3	vDir = vCurPos - m_pTransform->GetWorldPos();
		float	fDist = vDir.Length();
		float	fMoveDist = 200.f * fTime;

		if (fMoveDist > fDist)
		{
			fMoveDist = fDist;
			m_Road.pop_front();
		}
		vDir = vDir.Normalize();

		m_pTransform->MoveWorld(vDir,fMoveDist);

		SAFE_RELEASE(pTr);
	}

	return 0;
}

int CNavigation::LateUpdate(float fTime)
{
	return 0;
}

void CNavigation::Collision(float fTime)
{
}

void CNavigation::Render(float fTime)
{
}

CNavigation * CNavigation::Clone()
{
	return new CNavigation(*this);
}

void CNavigation::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CNavigation::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CNavigation::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

int CNavigation::GetTileIndex(const Vector3 & vPos)
{
	return GetTileIndex(vPos.x, vPos.y);
}

int CNavigation::GetTileIndex(const Vector2 & vPos)
{
	return GetTileIndex(vPos.x, vPos.y);
}

int CNavigation::GetTileIndex(const POINT & ptPos)
{
	return GetTileIndex(ptPos.x, ptPos.y);
}

int CNavigation::GetTileIndex(float x, float y)
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

int CNavigation::GetTileIndexX(float x, float y)
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

int CNavigation::GetTileIndexY(float x, float y)
{
	if (m_iTileNumX == 0 || m_iTileNumY == 0 || m_iTileSizeX == 0 ||
		m_iTileSizeY == 0)
		return -1;

	if (m_eTileType == TILE_RECT)
		return (int)y / m_iTileSizeY;

	// x, y ��ǥ�� Ÿ���� 1 by 1 �������� ������ش�.
	float	fX = x / m_iTileSizeX;
	float	fY = y / m_iTileSizeY;

	// 0 ~ 1 ������ Ÿ�� �������� �Ÿ����� ���Ѵ�.
	fX -= (int)fX;
	fY -= (int)fY;

	// �簢 Ÿ�Ͽ����� Y �ε��� ���� �����ش�.
	int	iStartY = (int)y / m_iTileSizeY;

	// �簢 Ÿ�Ͽ����� X �ε��� ���� �����ش�.
	int	iStartX = (int)x / m_iTileSizeX;

	// ���� ������ ���
	if (fY < 0.5f)
	{
		// ��ǥ������ �� ��� ������ ���
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

		// ��ǥ������ �� ��� ������ ���
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

	// �Ʒ��� ������ ���
	else if (fY > 0.5f)
	{
		// ��ǥ������ �� �ϴ� ������ ���
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

		// ��ǥ������ �� �ϴ� ������ ���
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

CTile * CNavigation::GetTile(float x, float y)
{
	int	idx = GetTileIndex(x, y);

	if (idx == -1)
		return NULL;

	return m_vecTile[idx]->FindComponentFromType<CTile>(CT_TILE);
}

void CNavigation::FindRoad(const Vector2 & vEndPos)
{
	FindRoad(Vector3(vEndPos.x, vEndPos.y, 0.f));
}

void CNavigation::FindRoad(const Vector3 & vEndPos)
{
	m_Road.clear();

	int	iEndIndex = GetTileIndex(vEndPos);

	if (iEndIndex == -1)
		return;

	CTile*	pEndTile = m_vecTile[iEndIndex];

	TERRAIN2D_TILE_OPTION	eEndTileOption = pEndTile->GetOption();

	if (eEndTileOption == TTO_NOMOVE)
		return;

	m_vecOpen.clear();
	m_vecClose.clear();

	Vector3	vPos = m_pTransform->GetWorldPos();
	int	idxX = GetTileIndexX(vPos.x, vPos.y);
	int	idxY = GetTileIndexY(vPos.x, vPos.y);
	int	iNodeIndex = idxY * m_iTileNumX + idxX;
	int	iStartIndex = iNodeIndex;

	if (iStartIndex == iEndIndex)
		return;

	CTile*	pStartTile = m_vecTile[iNodeIndex];

	pStartTile->m_iParentIndex = -1;

	OPENNODE	tNode = {};
	tNode.pTile = pStartTile;
	tNode.iIndex = iNodeIndex;

	m_vecOpen.push_back(tNode);

	CTile*	pNode = NULL;

	// �簢���� �밢�� ���̸� ���س��´�.
	float	fLength = sqrtf(m_iTileSizeX * m_iTileSizeX + m_iTileSizeY * m_iTileSizeY) / 2.f;

	while (true)
	{
		if (m_vecOpen.empty())
			return;

		tNode = m_vecOpen.back();
		iNodeIndex = tNode.iIndex;
		pNode = tNode.pTile;
		m_vecOpen.pop_back();
		// ���� ��带 �߽����� 8������ Ÿ�� �ε����� �����ش�.
		int	iIndex[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };

		idxX = iNodeIndex % m_iTileNumX;
		idxY = iNodeIndex / m_iTileNumX;
		GetOpenListIndex(iIndex, idxX, idxY);

		// 8���� ��带 ������Ͽ� �߰����ش�.
		for (int i = 0; i < 8; ++i)
		{
			if (iIndex[i] == -1)
				continue;

			else if (m_vecTile[iIndex[i]]->m_eOption == TTO_NOMOVE)
				continue;

			else if (iIndex[i] == iStartIndex)
				continue;

			else if (iEndIndex == iIndex[i])
			{
				int	iRoadIndex = iIndex[i];
				CTile*	pRoadTile = m_vecTile[iRoadIndex];
				pRoadTile->m_iParentIndex = iNodeIndex;
				while (pRoadTile->m_iParentIndex != -1)
				{
					m_Road.push_front(iRoadIndex);
					iRoadIndex = pRoadTile->m_iParentIndex;
					pRoadTile = m_vecTile[iRoadIndex];
				}
				//m_Road.push_front(iRoadIndex);

				m_vEndPos = vEndPos;
				if (m_pAnimation)
					m_pAnimation->ChangeAnimation(m_strMoveAnimationKey);

				return;
			}

			CTile*	pCurNode = m_vecTile[iIndex[i]];

			int	iParentIndex = iNodeIndex;
			int	iEdge = -1, iHuristic = -1;

			// ����� �־��ش�.
			if (i == 1 || i == 2 || i == 5 || i == 6)
				iEdge = fLength + m_vecTile[iNodeIndex]->m_iEdge;

			else if(i == 0 || i == 7)
				iEdge = m_iTileSizeY + m_vecTile[iNodeIndex]->m_iEdge;

			else
				iEdge = m_iTileSizeX + m_vecTile[iNodeIndex]->m_iEdge;

			// ������Ͽ� �ִ��� �Ǵ��Ѵ�. ���� ��� �׳� �߰� ���� ��� ������
			// ����� ���Ͽ� �� ���� ���� ��ü���ش�.
			bool	bChange = true;

			for (size_t j = 0; j < m_vecOpen.size(); ++j)
			{
				// ���� ����� �ε����� ���� ��� ���� ���� ���Ѵ�.
				if (m_vecOpen[j].iIndex == iIndex[i])
				{
					if (iEdge > m_vecTile[iIndex[i]]->m_iEdge)
						bChange = false;

					break;
				}
			}

			if (!bChange)
				continue;

			for (size_t j = 0; j < m_vecClose.size(); ++j)
			{
				// ���� ����� �ε����� ���� ��� ���� ���� ���Ѵ�.
				if (m_vecClose[j] == iIndex[i])
				{
					if (iEdge > m_vecTile[iIndex[i]]->m_iEdge)
						bChange = false;

					break;
				}
			}

			if (!bChange)
				continue;

			// �� ����� ���� ��ġ�� ���Ѵ�.
			CTransform*	pTr = pCurNode->GetTransform();

			Vector3	vCurPos = pTr->GetWorldPos();
			vCurPos.x += 0.5f * m_iTileSizeX;
			vCurPos.y += 0.5f * m_iTileSizeY;
			Vector3	vD = vCurPos - vEndPos;

			iHuristic = sqrtf(vD.x * vD.x + vD.y * vD.y);

			int	iTotal;
			iTotal = iEdge + iHuristic;

			pCurNode->m_iEdge = iEdge;
			pCurNode->m_iHuristic = iHuristic;
			pCurNode->m_iTotal = iTotal;
			pCurNode->m_iParentIndex = iParentIndex;

			SAFE_RELEASE(pTr);

			OPENNODE	tPushNode = {};
			tPushNode.pTile = m_vecTile[iIndex[i]];
			tPushNode.iIndex = iIndex[i];

			m_vecOpen.push_back(tPushNode);
		}

		// ���� ��带 ���� ��Ͽ� �־��ش�.
		m_vecClose.push_back(iNodeIndex);

		// ���� ����� �������ش�.
		sort(m_vecOpen.begin(), m_vecOpen.end(), CNavigation::Sort);

	}
}

void CNavigation::GetOpenListIndex(int iIndex[8], int x, int y)
{
	// ���� ��. �� Ÿ���� y �ε��� �������� 2ĭ ���̱� ������ -2�� �غ��� 0���� ��
	// ���� ���� Ÿ���� ���� ������ -1, �װ� �ƴϸ� �ε����� �����ش�.
	iIndex[0] = y - 2 < 0 ? -1 : (y - 2) * m_iTileNumX + x;

	// �׸��� ���� �� Ÿ���� ���ؾ� �Ѵ�. ���� �� Ÿ���� y 1ĭ��, x 1ĭ ���� ��ġ��
	// �����Ƿ� -1���� ���־ 0���� Ŭ ��� �����ϴ� Ÿ���̹Ƿ� �ε����� �߰���
	// �ش�.
	int	iIdxOffset = 0;
	if (y % 2 == 0)
		iIdxOffset = -1;

	if (x + iIdxOffset && y - 1 >= 0)
		iIndex[1] = (y - 1) * m_iTileNumX + x + iIdxOffset;

	iIdxOffset = 1;
	if (y % 2 == 0)
		iIdxOffset = 0;
	if (x + iIdxOffset >= 0 && y - 1 >= 0)
		iIndex[2] = (y - 1) * m_iTileNumX + x + iIdxOffset;

	if (x - 1 >= 0)
		iIndex[3] = y * m_iTileNumX + x - 1;

	if (x + 1 < m_iTileNumX)
		iIndex[4] = y * m_iTileNumX + x + 1;

	iIdxOffset = 0;
	if (y % 2 == 0)
		iIdxOffset = -1;
	if (x + iIdxOffset && y + 1 < m_iTileNumY)
		iIndex[5] = (y + 1) * m_iTileNumX + x + iIdxOffset;

	iIdxOffset = 1;
	if (y % 2 == 0)
		iIdxOffset = 0;
	if (x + iIdxOffset >= 0 && y + 1 < m_iTileNumY)
		iIndex[6] = (y + 1) * m_iTileNumX + x + iIdxOffset;

	if (y + 2 < m_iTileNumY)
		iIndex[7] = (y + 2) * m_iTileNumX + x;
}

bool CNavigation::Sort(OPENNODE& p1, OPENNODE& p2)
{
	return p1.pTile->m_iTotal > p2.pTile->m_iTotal;
}
