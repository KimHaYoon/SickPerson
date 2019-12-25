#pragma once

#include "Flag.h"
#include "EngineVector2.h"
#include "EngineVector3.h"
#include "EngineVector4.h"
#include "EngineMatrix.h"

GAME_BEGIN

typedef struct GAME_DLL _tagResolution
{
	UINT	iWidth;
	UINT	iHeight;

	_tagResolution() :
		iWidth( 0 ),
		iHeight( 0 )
	{
	}

	_tagResolution( UINT w, UINT h ) :
		iWidth( w ),
		iHeight( h )
	{
	}
}RESOLUTION, *PRESOLUTION;

GAME_END