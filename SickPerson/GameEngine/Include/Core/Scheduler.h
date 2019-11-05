#pragma once

#include "../Engine.h"

Engine_BEGIN

enum SCHEDULE_OPTION
{
	SO_LOOP,
	SO_ONCE,
	SO_TIME,
	SO_COUNT
};

typedef struct Engine_DLL _tagScheduleInfo
{
	string	strName;
	SCHEDULE_OPTION	eOption;
	function<void(float)>	func;
	bool	bPause;
	float	fTime;
	float	fTimeLimit;
	float	fDestroyTime;
	float	fDestroyTimeLimit;
	int		iCount;
	int		iCountLimit;
}SCHEDULEINFO, *PSCHEDULEINFO;

class Engine_DLL CScheduler
{
private:
	list<PSCHEDULEINFO>	m_ScheduleList;

public:
	bool Init();
	template <typename T>
	void AddSchedule(const string& strName, SCHEDULE_OPTION eOption,
		T* pObj, void (T::*pFunc)(float),
		float fTimeLimit, float fDestroyTimeLimit,
		int iDestroyCountLimit)
	{
		PSCHEDULEINFO	pInfo = new SCHEDULEINFO;

		pInfo->strName = strName;
		pInfo->eOption = eOption;
		pInfo->fTimeLimit = fTimeLimit;
		pInfo->fDestroyTimeLimit = fDestroyTimeLimit;
		pInfo->iCountLimit = iDestroyCountLimit;
		pInfo->fTime = 0.f;
		pInfo->fDestroyTime = 0.f;
		pInfo->iCount = 0;
		pInfo->func = bind(pFunc, pObj, placeholders::_1);
		pInfo->bPause = false;

		m_ScheduleList.push_back(pInfo);
	}

	void AddSchedule(const string& strName, SCHEDULE_OPTION eOption,
		void(*pFunc)(float),
		float fTimeLimit, float fDestroyTimeLimit,
		int iDestroyCountLimit);
	void Update(float fTime);
	void PauseSchedule(const string& strName);
	void ReStartSchedule(const string& strName);
	void DeleteSchedule(const string& strName);

	DECLARE_SINGLE(CScheduler)
};

Engine_END