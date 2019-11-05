#include "Scheduler.h"

Engine_USING

DEFINITION_SINGLE(CScheduler)

CScheduler::CScheduler()
{
}

CScheduler::~CScheduler()
{
	Safe_Delete_VecList(m_ScheduleList);
}

bool CScheduler::Init()
{
	return true;
}

void CScheduler::AddSchedule(const string & strName,
	SCHEDULE_OPTION eOption, void(*pFunc)(float),
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
	pInfo->func = bind(pFunc, placeholders::_1);
	pInfo->bPause = false;

	m_ScheduleList.push_back(pInfo);
}

void CScheduler::Update(float fTime)
{
	list<PSCHEDULEINFO>::iterator	iter;
	list<PSCHEDULEINFO>::iterator	iterEnd = m_ScheduleList.end();

	for (iter = m_ScheduleList.begin(); iter != iterEnd;)
	{
		(*iter)->fTime += fTime;

		switch ((*iter)->eOption)
		{
		case SO_LOOP:
			if ((*iter)->fTime >= (*iter)->fTimeLimit)
			{
				(*iter)->func(fTime);
				(*iter)->fTime -= (*iter)->fTimeLimit;
			}
			++iter;
			break;
		case SO_ONCE:
			if ((*iter)->fTime >= (*iter)->fTimeLimit)
			{
				(*iter)->func(fTime);
				SAFE_DELETE((*iter));
				iter = m_ScheduleList.erase(iter);
				iterEnd = m_ScheduleList.end();
			}

			else
				++iter;
			break;
		case SO_TIME:
			(*iter)->fDestroyTime += fTime;

			if ((*iter)->fTime >= (*iter)->fTimeLimit)
			{
				(*iter)->fTime -= (*iter)->fTimeLimit;
				(*iter)->func(fTime);
			}

			if ((*iter)->fDestroyTime >= (*iter)->fDestroyTimeLimit)
			{
				SAFE_DELETE((*iter));
				iter = m_ScheduleList.erase(iter);
				iterEnd = m_ScheduleList.end();
			}
			else
				++iter;
			break;
		case SO_COUNT:
			++iter;
			break;
		}
	}
}

void CScheduler::PauseSchedule(const string & strName)
{
	list<PSCHEDULEINFO>::iterator	iter;
	list<PSCHEDULEINFO>::iterator	iterEnd = m_ScheduleList.end();

	for (iter = m_ScheduleList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->strName == strName)
		{
			(*iter)->bPause = true;
			break;
		}
	}
}

void CScheduler::ReStartSchedule(const string & strName)
{
	list<PSCHEDULEINFO>::iterator	iter;
	list<PSCHEDULEINFO>::iterator	iterEnd = m_ScheduleList.end();

	for (iter = m_ScheduleList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->strName == strName)
		{
			(*iter)->bPause = false;
			break;
		}
	}
}

void CScheduler::DeleteSchedule(const string & strName)
{
	list<PSCHEDULEINFO>::iterator	iter;
	list<PSCHEDULEINFO>::iterator	iterEnd = m_ScheduleList.end();

	for (iter = m_ScheduleList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->strName == strName)
		{
			SAFE_DELETE((*iter));
			m_ScheduleList.erase(iter);
			break;
		}
	}
}
