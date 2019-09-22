#include "PathManager.h"

Engine_USING

DEFINITION_SINGLE(CPathManager)

CPathManager::CPathManager()
{
}


CPathManager::~CPathManager()
{
}

bool CPathManager::Init()
{
	TCHAR	strPath[MAX_PATH] = {};
	GetModuleFileName(NULL, strPath, MAX_PATH);

	for (int i = lstrlen(strPath) - 1; i >= 0; --i)
	{
		if (strPath[i] == '\\' || strPath[i] == '/')
		{
			memset(&strPath[i + 1], 0, sizeof(TCHAR) * (MAX_PATH - (i + 1)));
			break;
		}
	}

	m_mapPath.insert(make_pair(BASE_PATH, strPath));

	AddPath(SHADER_PATH, L"Shader\\");
	AddPath(TEXTURE_PATH, L"Texture\\");
	AddPath(DATA_PATH, L"Data\\");
	AddPath(SOUND_PATH, L"Sound\\");
	AddPath(MESH_PATH, L"Mesh\\");

	return true;
}

bool CPathManager::AddPath(const string & strKey, const TCHAR * pPath,
	const string & strBaseKey)
{
	if (FindPath(strKey))
		return false;

	const wchar_t* pBasePath = FindPath(strBaseKey);

	wstring	strPath;

	if (pBasePath)
		strPath = pBasePath;

	strPath += pPath;

	m_mapPath.insert(make_pair(strKey, strPath));

	return true;
}

const wchar_t * CPathManager::FindPath(const string & strKey)
{
	unordered_map<string, wstring>::iterator	iter = m_mapPath.find(strKey);

	if (iter == m_mapPath.end())
		return NULL;

	return iter->second.c_str();
}

const char * CPathManager::FindPathToMultiByte(const string & strKey)
{
	const wchar_t* pPath = FindPath(strKey);

	if (!pPath)
		return NULL;

	char	strPath[MAX_PATH] = {};
	WideCharToMultiByte(CP_ACP, 0, pPath, -1, strPath, lstrlen(pPath),
		NULL, NULL);

	m_strFindPath = strPath;

	return m_strFindPath.c_str();;
}
