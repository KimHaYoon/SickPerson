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

// Transform
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

// Constant Buffer
typedef struct GAME_DLL _tagConstantBuffer
{
	int				iRegister;
	int				iSize;
	ID3D11Buffer*	pBuffer;
}CONSTANTBUFFER, *PCONSTANTBUFFER;


// Animation2D
typedef struct GAME_DLL _tagAnimationClip2D
{
	ANIMATION2D_TYPE	eType;
	ANIMATION2D_OPTION	eOption;
	class CTexture*		pTexture;
	int		iFrameX;
	int		iFrameY;
	int		iFrameMaxX;
	int		iFrameMaxY;
	int		iLengthX;
	int		iLengthY;
	int		iStartY;
	float	fAnimTime;
	float	fAnimLimitTime;
	int		iCount;
	int		iCountLimit;
	float	fOptionTime;
	float	fOptionTimeLimit;

	_tagAnimationClip2D() :
		eType( A2D_ATLAS ),
		eOption( A2DO_LOOP ),
		pTexture( NULL ),
		iFrameX( 0 ),
		iFrameY( 0 ),
		iFrameMaxX( 1 ),
		iFrameMaxY( 1 ),
		fAnimTime( 0.f ),
		fAnimLimitTime( 1.f ),
		iCount( 0 ),
		iCountLimit( 0 ),
		fOptionTime( 0.f ),
		fOptionTimeLimit( 0.f ),
		iLengthX( 0 ),
		iLengthY( 0 ),
		iStartY( 0 )
	{
	}
}ANIMATIONCLIP2D, *PANIMATIONCLIP2D;

// Color Vertex
typedef struct GAME_DLL _tagVertexColor
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
typedef struct GAME_DLL _tagVertexColorNormal
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


// Tex Vertex
typedef struct GAME_DLL _tagVertexTex
{
	Vector3	vPos;
	Vector2	vUV;

	_tagVertexTex()
	{
	}

	_tagVertexTex( const _tagVertexTex& vtx )
	{
		vPos = vtx.vPos;
		vUV = vtx.vUV;
	}

	_tagVertexTex( float x, float y, float z, float u, float v ) :
		vPos( x, y, z ),
		vUV( u, v )
	{
	}

	_tagVertexTex( const Vector3& _vPos, const Vector2& _vUV ) :
		vPos( _vPos ),
		vUV( _vUV )
	{
	}

}VERTEXTEX, *PVERTEXTEX;

// Button ConstantBuffer
typedef struct GAME_DLL _tagButtonCBuffer
{
	Vector4	vColor;
	float	fLight;
	Vector3	vEmpty;
}BUTTONCBUFFER, *PBUTTONCBUFFER;

// Material
typedef struct GAME_DLL _tagMaterial
{
	Vector4	vDif;
	Vector4	vAmb;
	Vector4	vSpc;
	Vector4	vEmv;
}MATERIAL, *PMATERIAL;

// Light
typedef struct GAME_DLL _tagLight
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

GAME_END