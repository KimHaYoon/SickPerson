#pragma once

#include "resource.h"

#include "Engine.h"

#ifdef _DEBUG	
#pragma comment(lib, "Engine_Debug")
#else
#pragma comment(lib, "Engine")
#endif	// _DEBUG