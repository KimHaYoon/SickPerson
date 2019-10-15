#pragma once

#include "Flag.h"
#include "EngineVector2.h"
#include "EngineVector3.h"
#include "EngineVector4.h"
#include "EngineMatrix.h"

typedef struct  _tagResolution
{
	UINT	iWidth;
	UINT	iHeight;

	_tagResolution() :
		iWidth(0),
		iHeight(0)
	{
	}

	_tagResolution(UINT w, UINT h) :
		iWidth(w),
		iHeight(h)
	{
	}
}RESOLUTION, *PRESOLUTION;


typedef struct  _tagSphere
{
	Vector3	vCenter;
	float	fRadius;
}SPHERE, *PSPHERE;


typedef struct _tagLight
{
	Vector4	vDif;
	Vector4	vAmb;
	Vector4	vSpc;
	int		iType;
	Vector3	vDir;
	Vector3	vPos;
	float	fRange;
	float	fInAngle;
	float	fOutAngle;
	float	fIntensityPercent;
	float	fEmpty;
}LIGHT, *PLIGHT;
