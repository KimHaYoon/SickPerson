#pragma once

#include <Windows.h>
#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include <conio.h>
#include <functional>
#include "Types.h"

using namespace std;

#define	PI	3.141592f


// Path Key
#define	BASE_PATH		"BasePath"
#define	SHADER_PATH		"ShaderPath"
#define	TEXTURE_PATH	"TexturePath"
#define	DATA_PATH		"DataPath"
#define	SOUND_PATH		"SoundPath"
#define	MESH_PATH		"MeshPath"


// Sampler Key	
#define	SAMPLER_LINEAR	"Linear"

// Shader Key
#define	STANDARD_COLOR_SHADER	"StandardColorShader"
#define	STANDARD_COLOR_NORMAL_SHADER	"StandardColorNormalShader"
#define	STANDARD_TEX_SHADER		"StandardTexShader"

#define	COLLIDER_SHADER			"ColliderShader"

#define	UI_SHADER				"UIShader"
#define	UI_COLOR_SHADER			"UIColorShader"

// RenderState Key
#define	ALPHA_BLEND	"AlphaBlend"

#define	DEPTH_DISABLE	"DepthDisable"

#define	WIRE_FRAME		"WireFrame"
#define	CULLING_NONE	"CullNone"

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
