#pragma once

#include "../Engine.h"

GAME_BEGIN

class GAME_DLL CPathManager
{
private:
	unordered_map<string, wstring>	m_mapPath;
	string		m_strFindPath;

public:
	bool Init();
	bool AddPath( const string& strKey, TCHAR* pPath,
		const string& strBaseKey = BASE_PATH );
	const wchar_t* FindPath( const string& strKey );
	const char* FindPathToMultiByte( const string& strKey );

	DECLARE_SINGLE( CPathManager )
};

GAME_END