#pragma once
#include "resource.h"
#include "Engine.h"

#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "GameEngine64_Debug")
#else
#pragma comment(lib, "GameEngine64")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "GameEngine_Debug")
#else
#pragma comment(lib, "GameEngine")
#endif
#endif // _DEBUG
