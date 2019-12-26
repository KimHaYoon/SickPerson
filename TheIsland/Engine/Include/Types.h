#pragma once

#include "Flag.h"
#include "EngineVector2.h"
#include "EngineVector3.h"
#include "EngineVector4.h"
#include "EngineMatrix.h"

GAME_BEGIN

// Resolution
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

// KeyInput
typedef struct GAME_DLL _tagKeyInfo
{
	string	strName;
	bool	bDown;
	bool	bPush;
	bool	bUp;
	vector<DWORD>	vecKey;
}KEYINFO, *PKEYINFO;

typedef struct GAME_DLL _tagTransformCBuffer
{
	Matrix	matWorld;
	Matrix	matView;
	Matrix	matProj;
	Matrix	matWV;
	Matrix	matWVP;
	Matrix	matWP;
	Vector3	vPivot;
	float	fEmpty;
	Vector3	vLength;
	float	fEmpty1;
}TRANSFORMCBUFFER, *PTRANSFORMCBUFFER;

// Animation2D
typedef struct GAME_DLL _tagAnimation2DCBuffer
{
	int		iType;
	int		iFrameX;
	int		iFrameY;
	int		iLengthX;
	int		iLengthY;
	Vector3	vEmpty;
}ANIMATION2DBUFFER, *PANIMATION2DBUFFER;

GAME_END