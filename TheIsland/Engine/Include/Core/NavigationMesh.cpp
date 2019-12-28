#include "NavigationMesh.h"
#include "PathManager.h"

GAME_USING

CNavigationCell::CNavigationCell()
{
	m_eType = NSLT_NONE;
	m_iParentIdx = -1;
	m_iIndex = -1;
	m_fG = -1.f;
	m_fH = -1.f;
	m_fTotal = -1.f;
}

CNavigationCell::~CNavigationCell()
{
}


CNavigationMesh::CNavigationMesh()
{
}

CNavigationMesh::~CNavigationMesh()
{
	Safe_Delete_VecList(m_vecCell);
}

Vector3 CNavigationMesh::GetMin() const
{
	return m_vMin;
}

Vector3 CNavigationMesh::GetMax() const
{
	return m_vMax;
}

const list<Vector3>* CNavigationMesh::GetPathList() const
{
	return &m_PathList;
}

bool CNavigationMesh::Init()
{
	m_vMin.x = 10000000.f;
	m_vMin.y = 10000000.f;
	m_vMin.z = 10000000.f;

	m_vMax.x = -10000000.f;
	m_vMax.y = -10000000.f;
	m_vMax.z = -10000000.f;

	return true;
}

void CNavigationMesh::AddCell(const Vector3 vPos[3])
{
	CNavigationCell*	pCell = new CNavigationCell;

	for (int i = 0; i < 3; ++i)
	{
		pCell->m_vPos[i] = vPos[i];
	}

	// Edge를 구해준다. 0, 1, 2 순서로 시계방향으로 들어온다고 가정한다.
	pCell->m_vEdge[0] = vPos[1] - vPos[0];
	pCell->m_vEdgeCenter[0] = (vPos[1] + vPos[0]) / 2.f;

	pCell->m_vEdge[1] = vPos[2] - vPos[1];
	pCell->m_vEdgeCenter[1] = (vPos[2] + vPos[1]) / 2.f;

	pCell->m_vEdge[2] = vPos[2] - vPos[0];
	pCell->m_vEdgeCenter[2] = (vPos[2] + vPos[0]) / 2.f;

	pCell->m_vCenter = (vPos[0] + vPos[1] + vPos[2]) / 3.f;

	pCell->m_iIndex = m_vecCell.size();

	for (int i = 0; i < 3; ++i)
	{
		if (m_vMin.x > vPos[i].x)
			m_vMin.x = vPos[i].x;
		if (m_vMin.y > vPos[i].y)
			m_vMin.y = vPos[i].y;
		if (m_vMin.z > vPos[i].z)
			m_vMin.z = vPos[i].z;
		
		if (m_vMax.x < vPos[i].x)
			m_vMax.x = vPos[i].x;
		if (m_vMax.y < vPos[i].y)
			m_vMax.y = vPos[i].y;
		if (m_vMax.z < vPos[i].z)
			m_vMax.z = vPos[i].z;
	}

	m_vecCell.push_back(pCell);
}

void CNavigationMesh::AddAdj(int iCellIdx, int iAdjIdx)
{
	if (iCellIdx < 0 || iCellIdx >= m_vecCell.size())
		return;

	//else if (m_vecCell[iCellIdx]->m_vecAdj.size() >= 3)
	//	return;

	ADJINFO	tInfo = {};
	tInfo.iIndex = iAdjIdx;
	m_vecCell[iCellIdx]->m_vecAdj.push_back(tInfo);
}

void CNavigationMesh::CreateGridMapAdj(int iLineRectCount)
{
	size_t	iSize = m_vecCell.size();

	for (size_t i = 0; i < iSize; i += 2)
	{
		// 오른쪽 상단 삼각형의 인접정보는 바로 위 사각형의 왼쪽 하단, 자기자신 사각형의 왼쪽하단,
		// 오른쪽 사각형의 왼쪽 하단이다.
		int	idx = i - (iLineRectCount * 2) + 1;
		if (idx >= 0 && idx < m_vecCell.size())
		{
			ADJINFO	tInfo = {};
			tInfo.iIndex = idx;
			tInfo.iEdgeIndex = 0;
			m_vecCell[i]->m_vecAdj.push_back(tInfo);
		}

		idx = i + 1;
		if(idx < m_vecCell.size())
		{
			ADJINFO	tInfo = {};
			tInfo.iIndex = idx;
			tInfo.iEdgeIndex = 1;
			m_vecCell[i]->m_vecAdj.push_back(tInfo);
		}

		if ((i / 2) % (iLineRectCount - 1) != 0)
		{
			idx = i + 3;
			ADJINFO	tInfo = {};
			tInfo.iIndex = idx;
			tInfo.iEdgeIndex = 2;
			m_vecCell[i]->m_vecAdj.push_back(tInfo);
		}

		// 왼쪽 하단 삼각형의 인접정보는 자기자신의 오른쪽 상단, 왼쪽 사각형의 오른쪽 상단,
		// 아래쪽 사각형의 오른쪽 상단 삼각형이 된다.
		{
			ADJINFO	tInfo = {};
			tInfo.iIndex = i;
			tInfo.iEdgeIndex = 0;
			m_vecCell[i + 1]->m_vecAdj.push_back(tInfo);
		}

		if (i / 2 > 0)
		{
			idx = i - 2;
			if(idx >= 0) 
			{
				ADJINFO	tInfo = {};
				tInfo.iIndex = idx;
				tInfo.iEdgeIndex = 1;
				m_vecCell[i + 1]->m_vecAdj.push_back(tInfo);
			}
		}

		idx = i + (iLineRectCount * 2);
		if (idx >= 0 && idx < m_vecCell.size())
		{
			ADJINFO	tInfo = {};
			tInfo.iIndex = idx;
			tInfo.iEdgeIndex = 2;
			m_vecCell[i + 1]->m_vecAdj.push_back(tInfo);
		}
	}

	string	strText;

	for (int i = 0; i < m_vecCell.size(); ++i)
	{
		char	strNumber[128] = {};
		itoa(i, strNumber, 10);
		strText += strNumber;
		strText += " Adj : ";

		memset(strNumber, 0, 128);

		_cprintf("%d Cell Adj : ", i);

		for (size_t j = 0; j < m_vecCell[i]->m_vecAdj.size(); ++j)
		{
			itoa(m_vecCell[i]->m_vecAdj[j].iIndex, strNumber, 10);
			strText += strNumber;
			strText += ", ";
			_cprintf("%d, ", m_vecCell[i]->m_vecAdj[j].iIndex);
		}

		_cprintf("%d Cell Edge Adj : ", i);
		for (size_t j = 0; j < m_vecCell[i]->m_vecAdj.size(); ++j)
		{
			itoa(m_vecCell[i]->m_vecAdj[j].iEdgeIndex, strNumber, 10);
			strText += strNumber;
			strText += ", ";
			_cprintf("%d, ", m_vecCell[i]->m_vecAdj[j].iEdgeIndex);
		}

		_cprintf("\n");
	}
}

void CNavigationMesh::CreateAdj()
{
	size_t	iSize = m_vecCell.size();

	FILE*	pFile = NULL;

	fopen_s(&pFile, "NaviAdj.txt", "wt");

	Vector3	vOrigin[2][3];
	Vector3	vEdgeDir[2][3];
	float	fLength[2][3];

	for (size_t i = 0; i < iSize; ++i)
	{
		Vector3	vSrc[3];
		Vector3	vSrcOrigin[3];
		float	fSrcLength[3] = {};
		for (int k = 0; k < 3; ++k)
		{
			vEdgeDir[0][k] = m_vecCell[i]->m_vEdge[k];

			fLength[0][k] = vEdgeDir[0][k].Length();

			vEdgeDir[0][k] = vEdgeDir[0][k].Normalize();
		}

		vOrigin[0][0] = m_vecCell[i]->m_vPos[0];
		vOrigin[0][1] = m_vecCell[i]->m_vPos[1];
		vOrigin[0][2] = m_vecCell[i]->m_vPos[2];

		for (size_t j = i + 1; j < iSize; ++j)
		{
			// 삼각형의 3개의 엣지들을 서로 내적하여 평행한지 비교한다.
			Vector3	vDest[3];
			float fDestLength[3];
			Vector3	vDestOrigin[3];
			int	iPosIdx[3][2] = { 1, 0, 2, 1, 2, 0 };
			bool	bInsert = false;

			vOrigin[1][0] = m_vecCell[j]->m_vPos[0];
			vOrigin[1][1] = m_vecCell[j]->m_vPos[1];
			vOrigin[1][2] = m_vecCell[j]->m_vPos[2];

			for (int k = 0; k < 3; ++k)
			{
				vEdgeDir[1][k] = m_vecCell[j]->m_vEdge[k];

				fLength[1][k] = vEdgeDir[1][k].Length();

				vEdgeDir[1][k] = vEdgeDir[1][k].Normalize();
			}

			for (int k = 0; k < 3; ++k)
			{
				if (bInsert)
					break;

				for (int l = 0; l < 3; ++l)
				{
					// A삼각형과 B삼각형의 엣지를 구성하는 두 정점이 같을 경우
					// 이 삼각형은 무조건 붙어있는 삼각형이다.
					if ((vOrigin[0][iPosIdx[k][0]] == vOrigin[1][iPosIdx[l][0]] ||
						vOrigin[0][iPosIdx[k][0]] == vOrigin[1][iPosIdx[l][1]]) &&
						(vOrigin[0][iPosIdx[k][1]] == vOrigin[1][iPosIdx[l][0]] ||
							vOrigin[0][iPosIdx[k][1]] == vOrigin[1][iPosIdx[l][1]]))
					{
						ADJINFO	tInfo = {};
						tInfo.iIndex = j;
						tInfo.iEdgeIndex = k;
						m_vecCell[i]->m_vecAdj.push_back(tInfo);

						tInfo.iIndex = i;
						tInfo.iEdgeIndex = l;
						m_vecCell[j]->m_vecAdj.push_back(tInfo);
						bInsert = true;
						break;
					}

					// 두 엣지에 대해서 상대 엣지를 구성하는 2개의 점이 대상 엣지
					// 위에 둘다 존재하는지를 판단한다. 둘다 존재한다면
					// 무조건 붙어있는 것이다.
					// (B1 - A1, B2 - A1), (B1 - A2, B2 - A2)
					// 내적을 통해서 빼준 벡터의 크기가 그대로 나온다면
					// 직선상에 존재하는 것이다. 단, 둘다 크기가 나올 경우는
					// 무조건 존재하는 것이고 둘중 하나만 나온다면 크기가 0이라면
					// 하나의 점만 직선위에 존재하고 두 점 위치가 같다는 것이므로
					// 해당 삼각형은 인접 삼각형이 아니다.
					// B1 - A1 처리
					if (CheckOnEdge(i, j, vOrigin[1][iPosIdx[l][0]],
						vOrigin[0][iPosIdx[k][0]], vEdgeDir[0][k], fLength[0][k], k, l))
					{
						bInsert = true;
						break;
					}

					// B2 - A1 처리
					else if (CheckOnEdge(i, j, vOrigin[1][iPosIdx[l][1]],
						vOrigin[0][iPosIdx[k][0]], vEdgeDir[0][k], fLength[0][k], k, l))
					{
						bInsert = true;
						break;
					}

					// B1 - A2 처리
					else if (CheckOnEdge(i, j, vOrigin[1][iPosIdx[l][0]],
						vOrigin[0][iPosIdx[k][1]], vEdgeDir[0][k] * -1.f, fLength[0][k], k, l))
					{
						bInsert = true;
						break;
					}

					// B2 - A2 처리
					else if (CheckOnEdge(i, j, vOrigin[1][iPosIdx[l][1]],
						vOrigin[0][iPosIdx[k][1]], vEdgeDir[0][k] * -1.f, fLength[0][k], k, l))
					{
						bInsert = true;
						break;
					}
				}
			}
		}

		string	strText;

		char	strNumber[128] = {};
		itoa(i, strNumber, 10);
		strText += strNumber;
		strText += " Adj : ";

		memset(strNumber, 0, 128);

		_cprintf("%d Cell Adj : ", i);

		for (size_t j = 0; j < m_vecCell[i]->m_vecAdj.size(); ++j)
		{
			itoa(m_vecCell[i]->m_vecAdj[j].iIndex, strNumber, 10);
			strText += strNumber;
			strText += ", ";
			_cprintf("%d, ", m_vecCell[i]->m_vecAdj[j].iIndex);
		}
		
		_cprintf("\n");
		strText += "\n";

		fputs(strText.c_str(), pFile);
	}

	fclose(pFile);
}

void CNavigationMesh::FindPath(const Vector3 & vStart, const Vector3 & vEnd)
{
	CNavigationCell*	pStartCell = FindNavigationCell(vStart);
	CNavigationCell*	pEndCell = FindNavigationCell(vEnd);

	if (!pStartCell || !pEndCell)
		return;

	m_PathList.clear();
	m_OpenList.clear();
	m_CloseList.clear();
	while (!m_Path.empty())
	{
		m_Path.pop();
	}

	for (size_t i = 0; i < m_vecCell.size(); ++i)
	{
		m_vecCell[i]->m_fG = -1.f;
		m_vecCell[i]->m_fH = -1.f;
		m_vecCell[i]->m_fTotal = -1.f;
		m_vecCell[i]->m_iParentIdx = -1;
		m_vecCell[i]->m_eType = NSLT_NONE;
	}

	// 시작 Cell을 Open목록에 넣어준다.
	pStartCell->m_eType = NSLT_OPEN;
	pStartCell->m_fG = 0.f;
	pStartCell->m_fH = vStart.Distance(vEnd);
	pStartCell->m_fTotal = pStartCell->m_fG + pStartCell->m_fH;

	m_OpenList.push_back(pStartCell);

	m_bFindEnd = false;

	while (!m_OpenList.empty() && !m_bFindEnd)
	{
		CNavigationCell*	pCell = m_OpenList.back();
		m_OpenList.pop_back();

		pCell->m_eType = NSLT_CLOSE;

		m_CloseList.push_back(pCell);

		AddOpenList(pCell, pEndCell, vEnd, vStart);
	}
}

void CNavigationMesh::AddOpenList(CNavigationCell * pCell, CNavigationCell * pEndCell,
	const Vector3& vEnd, const Vector3& vStart)
{
	for (size_t i = 0; i < pCell->m_vecAdj.size(); ++i)
	{
		CNavigationCell*	pAdjCell = m_vecCell[pCell->m_vecAdj[i].iIndex];

		if (pAdjCell->m_eType != NSLT_CLOSE)
		{
			int	iParentIndex = pCell->m_iIndex;

			if (pAdjCell == pEndCell)
			{
				pEndCell->m_fG = -1.f;
				pEndCell->m_fH = -1.f;
				pEndCell->m_fTotal = -1.f;
				pEndCell->m_iParentIdx = iParentIndex;
				pAdjCell->m_eType = NSLT_NONE;

				m_bFindEnd = true;

				while (pAdjCell->m_iParentIdx != -1)
				{
					m_Path.push(pAdjCell->m_iIndex);

					pAdjCell = m_vecCell[pAdjCell->m_iParentIdx];
				}

				FILE*	pFile = NULL;

				fopen_s(&pFile, "PathIndex.txt", "wt");

				vector<int>	vecPathIndex;

				while (!m_Path.empty())
				{
					int	idx = m_Path.top();
					m_Path.pop();

					vecPathIndex.push_back(idx);

					char	text[256] = {};
					sprintf_s(text, "Path Index : %d\n", idx);
					
					fputs(text, pFile);
				}

				int	iPrevIndex = pAdjCell->m_iIndex;
				float	fCellWidth = (m_vecCell[0]->m_vPos[1].x - m_vecCell[0]->m_vPos[0].x);
				int	iWidth = (int)(m_vMax.x - m_vMin.x) / fCellWidth;

				vector<int>	vecCenter;

				for (size_t i = 0; i < vecPathIndex.size() - 1; ++i)
				{
					int	iEdgeIndex = -1;
					pAdjCell = m_vecCell[vecPathIndex[i]];

					for (size_t j = 0; j < pAdjCell->m_vecAdj.size(); ++j)
					{
						if (pAdjCell->m_vecAdj[j].iIndex == vecPathIndex[i + 1])
						{
							iEdgeIndex = pAdjCell->m_vecAdj[j].iEdgeIndex;
							break;
						}
					}

					m_PathList.push_back(pAdjCell->m_vEdgeCenter[iEdgeIndex]);
					vecCenter.push_back(iEdgeIndex);

					iPrevIndex = vecPathIndex[i];

					//int	iResult = vecPathIndex[i] - iPrevIndex;

					//// 오른쪽일 경우
					//if (iResult == 3)
					//{
					//	vecCenter.push_back(1);
					//	m_PathList.push_back(pAdjCell->m_vEdgeCenter[1]);
					//}

					//// 왼쪽일 경우
					//else if (iResult == -3)
					//{
					//	vecCenter.push_back(2);
					//	m_PathList.push_back(pAdjCell->m_vEdgeCenter[2]);
					//}

					//// 대각선으로 붙어있을 경우
					//else if (iResult == 1)
					//{
					//	vecCenter.push_back(2);
					//	m_PathList.push_back(pAdjCell->m_vEdgeCenter[2]);
					//}

					//else if (iResult == -1)
					//{
					//	vecCenter.push_back(0);
					//	m_PathList.push_back(pAdjCell->m_vEdgeCenter[0]);
					//}

					//// 위로 붙어있을 경우
					//else if (iResult == iWidth * 2 - 1)
					//{
					//	vecCenter.push_back(1);
					//	m_PathList.push_back(pAdjCell->m_vEdgeCenter[1]);
					//}

					//// 아래일 경우
					//else if (iResult == -(iWidth * 2 - 1))
					//{
					//	vecCenter.push_back(0);
					//	m_PathList.push_back(pAdjCell->m_vEdgeCenter[0]);
					//}

				}

				m_PathList.push_back(vEnd);
				
				// 정해진 경로의 인덱스 수만큼 반복하며 해당 삼각형을 관통하는지 판단한다.
				Vector3	vStartPos = vStart;
				vector<int>	vecLastPath;
				m_PathList.clear();

				for (size_t i = 2; i < vecPathIndex.size() - 1; ++i)
				{
					int	idx = vecPathIndex[i];

					// 엣지와 시작점으로부터 엣지 센터까지의 직선을 만들고
					// 엣지와 만들어준 직선이 서로 교차하는지 판단한다.
					Vector3	vDir = m_vecCell[idx]->m_vEdgeCenter[vecCenter[i]] - vStartPos;
					float	fDist = vDir.Length() + 0.1f;
					vDir = vDir.Normalize();

					Vector3	vEndPos = vStartPos + vDir * fDist;

					int	iPathCount = 0;

					for (int j = 0; j < 3; ++j)
					{
						Vector3	vEdgeStart;
						Vector3	vEdgeEnd;

						if (j == 0)
						{
							vEdgeStart = m_vecCell[idx]->m_vPos[0];
							vEdgeEnd = m_vecCell[idx]->m_vPos[1];
						}

						else if (j == 1)
						{
							vEdgeStart = m_vecCell[idx]->m_vPos[2];
							vEdgeEnd = m_vecCell[idx]->m_vPos[1];
						}

						else if (j == 2)
						{
							vEdgeStart = m_vecCell[idx]->m_vPos[0];
							vEdgeEnd = m_vecCell[idx]->m_vPos[2];
						}

						Vector3	vIntersect;
						if (CheckPathDir(vStartPos, vEndPos, vEdgeStart,
							vEdgeEnd, vIntersect))
						{
							++iPathCount;

							if (iPathCount == 2)
							{
								break;
							}
						}
					}

					if (iPathCount < 2)
					{
						vecLastPath.push_back(vecPathIndex[i - 1]);
						vStartPos = m_vecCell[vecPathIndex[i - 1]]->m_vEdgeCenter[vecCenter[i - 1]];
						m_PathList.push_back(vStartPos);
					}
				}

				m_PathList.push_back(vEnd);

				fclose(pFile);

				return;
			}

			// 간선 비용은 삼각형의 중점간 거리로 처리한다.
			float	fG = pAdjCell->m_vCenter.Distance(pCell->m_vCenter);
			float	fH = pAdjCell->m_vCenter.Distance(vEnd);
			float	fTotal = fG + fH;

			if (pAdjCell->m_eType == NSLT_NONE)
			{
				pAdjCell->m_fG = fG;
				pAdjCell->m_fH = fH;
				pAdjCell->m_fTotal = fTotal;
				pAdjCell->m_iParentIdx = iParentIndex;
				pAdjCell->m_eType = NSLT_OPEN;
				m_OpenList.push_back(pAdjCell);
			}

			// 열린 목록에 들어가있는 Cell일 경우 값을 비교하여 더 작은 값으로 교체해
			// 준다.
			else if (pAdjCell->m_fG > fG)
			{
				pAdjCell->m_fG = fG;
				pAdjCell->m_fH = fH;
				pAdjCell->m_fTotal = fTotal;
				pAdjCell->m_iParentIdx = iParentIndex;
			}

			sort(m_OpenList.begin(), m_OpenList.end(), CNavigationMesh::Sort);
		}
	}
}

CNavigationCell * CNavigationMesh::FindGridNavigationCell(const Vector3 & vPos)
{
	// 위치를 기준으로 위, 아래 점을 잡고 위에서 아래로 향하는 벡터를 만든 후 해당 벡터를 Ray로 만든다.
	// 해당 Ray가 통과하는 삼각형을 찾아줘야 하는데 Grid가 아닐 경우 모든 삼각형에 대해 비교를 해주어야 한다.
	// Grid 기준으로 일단 구현을 해본다.
	// 어떤 사각형인지 인덱스를 구한다.
	float	fCellWidth = (m_vecCell[0]->m_vPos[1].x - m_vecCell[0]->m_vPos[0].x);
	float	fCellHeight = (m_vecCell[0]->m_vPos[0].z - m_vecCell[0]->m_vPos[2].z);
	int	idxX = (int)(vPos.x - m_vMin.x) / (int)fCellWidth;
	int idxY = (int)(m_vMax.z - vPos.z) / (int)fCellHeight;
	// 격자 맵에서의 가로 개수를 구한다.
	int	iWidth = (int)(m_vMax.x - m_vMin.x) / (int)fCellWidth;
	int	iHeight = (int)(m_vMax.z - m_vMin.z) / (int)fCellHeight;

	if (idxX < 0 || idxX >= iWidth)
		return NULL;

	else if (idxY < 0 || idxY >= iHeight)
		return NULL;

	// 사각형 인덱스에 * 2를 해주게 되면 삼각형 인덱스가 나온다.(우 상단 삼각형 인덱스가 나온다.
	int	iCellIndex = (idxY * iWidth + idxX) * 2;

	float	fX, fY;
	fX = (vPos.x - m_vecCell[iCellIndex]->m_vPos[0].x) / fCellWidth;
	fY = (m_vecCell[iCellIndex]->m_vPos[0].z - vPos.z) / fCellHeight;

	if (fX < fY)
		iCellIndex++;

	return m_vecCell[iCellIndex];
}

CNavigationCell * CNavigationMesh::FindNavigationCell(const Vector3 & vPos)
{
	Vector3	vOrigin = vPos + Vector3(0.f, 1.f, 0.f) * 5.f;

	for (size_t i = 0; i < m_vecCell.size(); ++i)
	{
		float	fDist = 0.f;
		Vector3	vIntersect;

		if (RayIntersectTriangle(vOrigin, Vector3(0.f, -1.f, 0.f), m_vecCell[i]->m_vPos[0],
			m_vecCell[i]->m_vPos[1], m_vecCell[i]->m_vPos[2], fDist, vIntersect))
		{
			return m_vecCell[i];
		}
	}

	return nullptr;
}

bool CNavigationMesh::CheckOnEdge(int iSrc, int iDest, const Vector3 & vOrigin1,
	const Vector3 & vOrigin2, const Vector3 & vEdge, float fEdgeLength, int iEdge1, int iEdge2)
{
	if ((int)(vOrigin2.x * 100) == (int)(vOrigin1.x * 100) && (int)(vOrigin2.y * 100) == (int)(vOrigin1.y * 100) &&
		(int)(vOrigin2.z * 100) == (int)(vOrigin1.z * 100))
		return false;

	Vector3	vResult = vOrigin1 - vOrigin2;

	float	fDist = vResult.Length();

	// 위에서 구해준 거리를 이용해서 원점으로부터 엣지 방향으로 지정된 거리만큼 이동된 위치에 대상 점이 있는지를
	// 판단한다.
	vResult = vOrigin2 + vEdge * fDist;

	if ((int)(vResult.x * 100) == (int)(vOrigin1.x * 100) && (int)(vResult.y * 100) == (int)(vOrigin1.y * 100) &&
		(int)(vResult.z * 100) == (int)(vOrigin1.z * 100))
	{
		vResult -= vOrigin2;
		if (vResult.Length() < fEdgeLength)
		{
			ADJINFO	tInfo = {};
			tInfo.iIndex = iDest;
			tInfo.iEdgeIndex = iEdge1;
			m_vecCell[iSrc]->m_vecAdj.push_back(tInfo);

			tInfo.iIndex = iSrc;
			tInfo.iEdgeIndex = iEdge2;
			m_vecCell[iDest]->m_vecAdj.push_back(tInfo);
			return true;
		}
	}

	return false;
}

bool CNavigationMesh::CheckPathDir(const Vector3 & vSrc1, const Vector3 & vSrc2, 
	const Vector3 & vDest1, const Vector3 & vDest2, Vector3 & vIntersect)
{
	double t;
	double s;
	double under = (vDest2.z - vDest1.z)*(vSrc2.x - vSrc1.x) - (vDest2.x - vDest1.x)*(vSrc2.z - vSrc1.z);
	if (under == 0) 
		return false;

	double _t = (vDest2.x - vDest1.x)*(vSrc1.z - vDest1.z) - (vDest2.z - vDest1.z)*(vSrc1.x - vDest1.x);
	double _s = (vSrc2.x - vSrc1.x)*(vSrc1.z - vDest1.z) - (vSrc2.z - vSrc1.z)*(vSrc1.x - vDest1.x);

	t = _t / under;
	s = _s / under;

	if (t<0.0 || t>1.0 || s<0.0 || s>1.0) 
		return false;

	if (_t == 0 && _s == 0)
		return false;

	vIntersect.x = vSrc1.x + t * (double)(vSrc2.x - vSrc1.x);
	vIntersect.z = vSrc1.z + t * (double)(vSrc2.z - vSrc1.z);

	return true;
}

void CNavigationMesh::Save(const char * pFileName, const string & strPathKey)
{
	const char* pPath = GET_SINGLE(CPathManager)->FindPathToMultiByte(strPathKey);

	string	strPath;

	if (pPath)
		strPath = pPath;

	strPath += pFileName;

	SaveFromFullPath(strPath.c_str());
}

void CNavigationMesh::SaveFromFullPath(const char * pFullPath)
{
	FILE*	pFile = NULL;

	fopen_s(&pFile, pFullPath, "wb");

	if (!pFile)
		return;

	fwrite(&m_vMin, sizeof(Vector3), 1, pFile);
	fwrite(&m_vMax, sizeof(Vector3), 1, pFile);

	// 셀 수 저장
	size_t	iSize = m_vecCell.size();

	fwrite(&iSize, sizeof(size_t), 1, pFile);

	// 각각의 셀 정보 저장
	for (size_t i = 0; i < iSize; ++i)
	{
		fwrite(m_vecCell[i]->m_vPos, sizeof(Vector3), 3, pFile);
		fwrite(m_vecCell[i]->m_vEdge, sizeof(Vector3), 3, pFile);
		fwrite(m_vecCell[i]->m_vEdgeCenter, sizeof(Vector3), 3, pFile);
		fwrite(&m_vecCell[i]->m_vCenter, sizeof(Vector3), 1, pFile);
		fwrite(&m_vecCell[i]->m_iIndex, sizeof(int), 1, pFile);

		size_t	iAdjSize = m_vecCell[i]->m_vecAdj.size();

		fwrite(&iAdjSize, sizeof(size_t), 1, pFile);

		fwrite(&m_vecCell[i]->m_vecAdj[0], sizeof(ADJINFO), iAdjSize, pFile);
	}

	fclose(pFile);
}

void CNavigationMesh::Load(const char * pFileName, const string & strPathKey)
{
	const char* pPath = GET_SINGLE(CPathManager)->FindPathToMultiByte(strPathKey);

	string	strPath;

	if (pPath)
		strPath = pPath;

	strPath += pFileName;

	LoadFromFullPath(strPath.c_str());
}

void CNavigationMesh::LoadFromFullPath(const char * pFullPath)
{
	FILE*	pFile = NULL;

	fopen_s(&pFile, pFullPath, "rb");

	if (!pFile)
		return;

	fread(&m_vMin, sizeof(Vector3), 1, pFile);
	fread(&m_vMax, sizeof(Vector3), 1, pFile);

	// 셀 수 저장
	size_t	iSize = 0;

	fread(&iSize, sizeof(size_t), 1, pFile);

	Safe_Delete_VecList(m_vecCell);

	// 각각의 셀 정보 저장
	for (size_t i = 0; i < iSize; ++i)
	{
		CNavigationCell*	pCell = new CNavigationCell;

		/*
		NAVIMESH_SELL_LIST_TYPE	m_eType;
		int			m_iParentIdx;
		float		m_fG;
		float		m_fH;
		float		m_fTotal;
		*/
		pCell->m_eType = NSLT_NONE;
		pCell->m_iParentIdx = -1;
		pCell->m_fG = 0;
		pCell->m_fH = 0;
		pCell->m_fTotal = 0;

		fread(pCell->m_vPos, sizeof(Vector3), 3, pFile);
		fread(pCell->m_vEdge, sizeof(Vector3), 3, pFile);
		fread(pCell->m_vEdgeCenter, sizeof(Vector3), 3, pFile);
		fread(&pCell->m_vCenter, sizeof(Vector3), 1, pFile);
		fread(&pCell->m_iIndex, sizeof(int), 1, pFile);

		size_t	iAdjSize = 0;

		fread(&iAdjSize, sizeof(size_t), 1, pFile);

		pCell->m_vecAdj.clear();
		pCell->m_vecAdj.resize(iAdjSize);

		fread(&pCell->m_vecAdj[0], sizeof(ADJINFO), iAdjSize, pFile);

		m_vecCell.push_back(pCell);
	}

	fclose(pFile);
}

float CNavigationMesh::GetY(const Vector3 & vPos)
{
	Vector3	vOrigin = vPos + Vector3(0.f, 1.f, 0.f) * 1.f;
	Vector3	vDir = Vector3(0.f, -1.f, 0.f);

	for (size_t i = 0; i < m_vecCell.size(); ++i)
	{
		float	fDist = 0.f;
		Vector3	vIntersect;

		if (RayIntersectTriangle(vOrigin, vDir, m_vecCell[i]->m_vPos[0],
			m_vecCell[i]->m_vPos[1], m_vecCell[i]->m_vPos[2], fDist, vIntersect))
		{
			if(fDist <= 1.5f)
				return vIntersect.y;
		}
	}

	return 0.0f;
}

Vector3 CNavigationMesh::IntersectCell(const Vector3 & vOrigin, 
	const Vector3 & vDir)
{
	for (size_t i = 0; i < m_vecCell.size(); ++i)
	{
		float	fDist = 0.f;
		Vector3	vIntersect;

		if (RayIntersectTriangle(vOrigin, vDir, m_vecCell[i]->m_vPos[0],
			m_vecCell[i]->m_vPos[1], m_vecCell[i]->m_vPos[2], fDist, vIntersect))
		{
			return vIntersect;
		}
	}

	return Vector3::Zero;
}

bool CNavigationMesh::RayIntersectTriangle(Vector3 rayOrigin, Vector3 rayDir, 
	Vector3 v0, Vector3 v1, Vector3 v2, float& t, Vector3& vIntersect)
{
	Vector3 e1, e2, h, s, q;
	float a, f, u, v;

	e1 = v1 - v0;
	e2 = v2 - v0;
	h = rayDir.Cross(e2);
	a = e1.Dot(h);

	if (a > -0.00001 && a < 0.00001) return false;

	f = 1 / a;
	s = rayOrigin - v0;

	u = f * s.Dot(h);

	if (u < 0.f || u > 1.f)
		return false;

	q = s.Cross(e1);

	v = f * rayDir.Dot(q);

	if (v < 0.f || u + v > 1.f) 
		return false;

	t = f * e2.Dot(q);

	if (t > 0.00001)
	{
		vIntersect = rayOrigin + rayDir * t;
		return true;
	}

	return false;
}

bool CNavigationMesh::Sort(CNavigationCell * pSrc, CNavigationCell * pDest)
{
	return pSrc->m_fTotal > pDest->m_fTotal;
}
 