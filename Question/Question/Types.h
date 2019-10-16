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

// Constant Buffer
typedef struct  _tagConstantBuffer
{
	int				iRegister;
	int				iSize;
	ID3D12Resource*	pBuffer;
}CONSTANTBUFFER, *PCONSTANTBUFFER;

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

typedef struct _tagMaterial
{
	Vector4	vDif;
	Vector4	vAmb;
	Vector4	vSpc;
	Vector4	vEmv;
}MATERIAL, *PMATERIAL;

typedef struct _tagTransformCBuffer
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

// Color Vertex
typedef struct _tagVertexColor
{
	Vector3	vPos;
	Vector4	vColor;

	_tagVertexColor()
	{
	}

	_tagVertexColor( const _tagVertexColor& vtx )
	{
		vPos = vtx.vPos;
		vColor = vtx.vColor;
	}

	_tagVertexColor( float x, float y, float z, float r, float g, float b,
		float a ) :
		vPos( x, y, z ),
		vColor( r, g, b, a )
	{
	}

	_tagVertexColor( const Vector3& _vPos, const Vector4& _vColor ) :
		vPos( _vPos ),
		vColor( _vColor )
	{
	}
}VERTEXCOLOR, *PVERTEXCOLOR;

// Color Normal Vertex
typedef struct _tagVertexColorNormal
{
	Vector3	vPos;
	Vector3	vNormal;
	Vector4	vColor;

	_tagVertexColorNormal()
	{
	}

	_tagVertexColorNormal( const _tagVertexColorNormal& vtx )
	{
		vPos = vtx.vPos;
		vNormal = vtx.vNormal;
		vColor = vtx.vColor;
	}

	_tagVertexColorNormal( float x, float y, float z, float nx, float ny, float nz,
		float r, float g, float b, float a ) :
		vPos( x, y, z ),
		vNormal( nx, ny, nz ),
		vColor( r, g, b, a )
	{
	}

	_tagVertexColorNormal( const Vector3& _vPos, const Vector3& _vNormal, const Vector4& _vColor ) :
		vPos( _vPos ),
		vNormal( _vNormal ),
		vColor( _vColor )
	{
	}
}VERTEXCOLORNORMAL, *PVERTEXCOLORNORMAL;

