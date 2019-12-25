#pragma once

#include <Windows.h>
#include <unordered_map>
#include <list>
#include <vector>
#include <string>
#include <iostream>

#include "Types.h"

#define PI 3.141592f

template <typename T>
void Safe_Release_VecList( T& p )
{
	T::iterator	iter = p.begin();
	T::iterator	iterEnd = p.end();

	while ( iter != iterEnd )
	{
		SAFE_RELEASE( ( *iter ) );
		++iter;
	}

	p.clear();
}

template <typename T>
void Safe_Release_Map( T& p )
{
	T::iterator	iter = p.begin();
	T::iterator	iterEnd = p.end();

	while ( iter != iterEnd )
	{
		SAFE_RELEASE( iter->second );
		++iter;
	}

	p.clear();
}

template <typename T>
void Safe_Delete_VecList( T& p )
{
	T::iterator	iter = p.begin();
	T::iterator	iterEnd = p.end();

	while ( iter != iterEnd )
	{
		SAFE_DELETE( ( *iter ) );
		++iter;
	}

	p.clear();
}

template <typename T>
void Safe_Delete_Map( T& p )
{
	T::iterator	iter = p.begin();
	T::iterator	iterEnd = p.end();

	while ( iter != iterEnd )
	{
		SAFE_DELETE( iter->second );
		++iter;
	}

	p.clear();
}
