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
		// 여기에 찾아놓은 길대로 이동하는 코드를 작성한다.
		// 한칸 이동할때마다 앞의 노드를 하나씩 빼줘야 한다.(pop_front())
		int	idx = m_Road.front();

		// 이 노드의 중점 위치를 구한다.
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

	// 사각형의 대각선 길이를 구해놓는다.
	float	fLength = sqrtf(m_iTileSizeX * m_iTileSizeX + m_iTileSizeY * m_iTileSizeY) / 2.f;

	while (true)
	{
		if (m_vecOpen.empty())
			return;

		tNode = m_vecOpen.back();
		iNodeIndex = tNode.iIndex;
		pNode = tNode.pTile;
		m_vecOpen.pop_back();
		// 기준 노드를 중심으로 8방향의 타일 인덱스를 구해준다.
		int	iIndex[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };

		idxX = iNodeIndex % m_iTileNumX;
		idxY = iNodeIndex / m_iTileNumX;
		GetOpenListIndex(iIndex, idxX, idxY);

		// 8개의 노드를 열린목록에 추가해준다.
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

			// 비용을 넣어준다.
			if (i == 1 || i == 2 || i == 5 || i == 6)
				iEdge = fLength + m_vecTile[iNodeIndex]->m_iEdge;

			else if(i == 0 || i == 7)
				iEdge = m_iTileSizeY + m_vecTile[iNodeIndex]->m_iEdge;

			else
				iEdge = m_iTileSizeX + m_vecTile[iNodeIndex]->m_iEdge;

			// 열린목록에 있는지 판단한다. 없을 경우 그냥 추가 있을 경우 간선의
			// 비용을 비교하여 더 작은 노드로 교체해준다.
			bool	bChange = true;

			for (size_t j = 0; j < m_vecOpen.size(); ++j)
			{
				// 같은 노드의 인덱스가 있을 경우 간선 값을 비교한다.
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
				// 같은 노드의 인덱스가 있을 경우 간선 값을 비교한다.
				if (m_vecClose[j] == iIndex[i])
				{
					if (iEdge > m_vecTile[iIndex[i]]->m_iEdge)
						bChange = false;

					break;
				}
			}

			if (!bChange)
				continue;

			// 이 노드의 중점 위치를 구한다.
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

		// 기준 노드를 닫힌 목록에 넣어준다.
		m_vecClose.push_back(iNodeIndex);

		// 열린 목록을 정렬해준다.
		sort(m_vecOpen.begin(), m_vecOpen.end(), CNavigation::Sort);

	}
}

void CNavigation::GetOpenListIndex(int iIndex[8], int x, int y)
{
	// 가장 위. 위 타일은 y 인덱스 기준으로 2칸 위이기 때문에 -2를 해보고 0보다 작
	// 으면 위의 타일이 없기 때문에 -1, 그게 아니면 인덱스를 구해준다.
	iIndex[0] = y - 2 < 0 ? -1 : (y - 2) * m_iTileNumX + x;

	// 그림상 왼쪽 위 타일을 구해야 한다. 왼쪽 위 타일은 y 1칸위, x 1칸 전에 위치해
	// 있으므로 -1들을 해주어서 0보다 클 경우 존재하는 타일이므로 인덱스를 추가해
	// 준다.
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
