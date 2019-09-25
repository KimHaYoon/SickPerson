#pragma once

#include <Windows.h>
#include <unordered_map>
#include <list>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include <conio.h>
#include <functional>
#include "Types.h"

#define	PI	3.141592f

template <typename T>
void Safe_Release_VecList(T& p)
{
	typename T::iterator	iter = p.begin();
	typename T::iterator	iterEnd = p.end();

	while (iter != iterEnd)
	{
		SAFE_RELEASE((*iter));
		++iter;
	}

	p.clear();
}

template <typename T>
void Safe_Release_Map(T& p)
{
	typename T::iterator	iter = p.begin();
	typename T::iterator	iterEnd = p.end();

	while (iter != iterEnd)
	{
		SAFE_RELEASE(iter->second);
		++iter;
	}

	p.clear();
}

template <typename T>
void Safe_Delete_VecList(T& p)
{
	typename T::iterator	iter = p.begin();
	typename T::iterator	iterEnd = p.end();

	while (iter != iterEnd)
	{
		SAFE_DELETE((*iter));
		++iter;
	}

	p.clear();
}

template <typename T>
void Safe_Delete_Map(T& p)
{
	typename T::iterator	iter = p.begin();
	typename T::iterator	iterEnd = p.end();

	while (iter != iterEnd)
	{
		SAFE_DELETE(iter->second);
		++iter;
	}

	p.clear();
}
