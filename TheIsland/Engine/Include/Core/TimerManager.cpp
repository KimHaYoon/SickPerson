#include "TimerManager.h"
#include "Timer.h"

GAME_USING

DEFINITION_SINGLE( CTimerManager )

CTimerManager::CTimerManager()
{
}

CTimerManager::~CTimerManager()
{
	Safe_Release_Map( m_mapTimer );
}

bool CTimerManager::Init()
{
	CTimer*	pMainThreadTimer = CreateTimer( "MainThread" );

	SAFE_RELEASE( pMainThreadTimer );

	return true;
}

CTimer * CTimerManager::CreateTimer( const string & strKey )
{
	CTimer*	pTimer = FindTimer( strKey );

	if ( pTimer )
		return pTimer;

	pTimer = new CTimer;

	if ( !pTimer->Init() )
	{
		SAFE_RELEASE( pTimer );
		return NULL;
	}

	pTimer->AddRef();

	m_mapTimer.insert( make_pair( strKey, pTimer ) );

	return pTimer;
}

CTimer * CTimerManager::FindTimer( const string & strKey )
{
	unordered_map<string, CTimer*>::iterator	iter = m_mapTimer.find( strKey );

	if ( iter == m_mapTimer.end() )
		return NULL;

	iter->second->AddRef();

	return iter->second;
}
