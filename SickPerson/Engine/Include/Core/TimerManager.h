#pragma once

#include "../Engine.h"

Engine_BEGIN

class Engine_DLL CTimerManager
{
private:
	unordered_map<string, class CTimer*>	m_mapTimer;

public:
	bool Init();
	class CTimer* CreateTimer(const string& strKey);
	class CTimer* FindTimer(const string& strKey);

	DECLARE_SINGLE(CTimerManager)
};

Engine_END