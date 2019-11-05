#pragma once

#include "Flag.h"
#include "EngineVector2.h"
#include "EngineVector3.h"
#include "EngineVector4.h"
#include "EngineMatrix.h"

Engine_BEGIN

typedef struct Engine_DLL _tagResolution
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

// Color Vertex
typedef struct Engine_DLL _tagVertexColor
{
	Vector3	vPos;
	Vector4	vColor;

	_tagVertexColor()
	{
	}

	_tagVertexColor(const _tagVertexColor& vtx)
	{
		vPos = vtx.vPos;
		vColor = vtx.vColor;
	}

	_tagVertexColor(float x, float y, float z, float r, float g, float b,
		float a) :
		vPos(x, y, z),
		vColor(r, g, b, a)
	{
	}

	_tagVertexColor(const Vector3& _vPos, const Vector4& _vColor) :
		vPos(_vPos),
		vColor(_vColor)
	{
	}
}VERTEXCOLOR, *PVERTEXCOLOR;

// Color Normal Vertex
typedef struct Engine_DLL _tagVertexColorNormal
{
	Vector3	vPos;
	Vector3	vNormal;
	Vector4	vColor;

	_tagVertexColorNormal()
	{
	}

	_tagVertexColorNormal(const _tagVertexColorNormal& vtx)
	{
		vPos = vtx.vPos;
		vNormal = vtx.vNormal;
		vColor = vtx.vColor;
	}

	_tagVertexColorNormal(float x, float y, float z, float nx, float ny, float nz,
		float r, float g, float b, float a) :
		vPos(x, y, z),
		vNormal(nx, ny, nz),
		vColor(r, g, b, a)
	{
	}

	_tagVertexColorNormal(const Vector3& _vPos, const Vector3& _vNormal, const Vector4& _vColor) :
		vPos(_vPos),
		vNormal(_vNormal),
		vColor(_vColor)
	{
	}
}VERTEXCOLORNORMAL, *PVERTEXCOLORNORMAL;

// Tex Vertex
typedef struct Engine_DLL _tagVertexTex
{
	Vector3	vPos;
	Vector2	vUV;

	_tagVertexTex()
	{
	}

	_tagVertexTex(const _tagVertexTex& vtx)
	{
		vPos = vtx.vPos;
		vUV = vtx.vUV;
	}

	_tagVertexTex(float x, float y, float z, float u, float v) :
		vPos(x, y, z),
		vUV(u, v)
	{
	}

	_tagVertexTex(const Vector3& _vPos, const Vector2& _vUV) :
		vPos(_vPos),
		vUV(_vUV)
	{
	}
}VERTEXTEX, *PVERTEXTEX;

// Tex Normal Vertex
typedef struct Engine_DLL _tagVertexTexNormal
{
	Vector3	vPos;
	Vector3	vNormal;
	Vector2	vUV;

	_tagVertexTexNormal()
	{
	}

	_tagVertexTexNormal(const _tagVertexTexNormal& vtx)
	{
		vPos = vtx.vPos;
		vUV = vtx.vUV;
		vNormal = vtx.vNormal;
	}

	_tagVertexTexNormal(float x, float y, float z, float nx, float ny, float nz,
		float u, float v) :
		vPos(x, y, z),
		vNormal(nx, ny, nz),
		vUV(u, v)
	{
	}

	_tagVertexTexNormal(const Vector3& _vPos, const Vector3& _vNormal,
		const Vector2& _vUV) :
		vPos(_vPos),
		vNormal(_vNormal),
		vUV(_vUV)
	{
	}
}VERTEXTEXNORMAL, *PVERTEXTEXNORMAL;

// Bump Vertex
typedef struct Engine_DLL _tagVertexBump
{
	Vector3	vPos;
	Vector3	vNormal;
	Vector2	vUV;
	Vector3	vTangent;
	Vector3	vBinormal;

	_tagVertexBump()
	{
	}

	_tagVertexBump(const _tagVertexBump& vtx)
	{
		vPos = vtx.vPos;
		vUV = vtx.vUV;
		vNormal = vtx.vNormal;
		vTangent = vtx.vTangent;
		vBinormal = vtx.vBinormal;
	}

	_tagVertexBump(const Vector3& _vPos, const Vector3& _vNormal,
		const Vector2& _vUV, const Vector3& _vTangent,
		const Vector3& _vBinormal) :
		vPos(_vPos),
		vNormal(_vNormal),
		vUV(_vUV),
		vTangent(_vTangent),
		vBinormal(_vBinormal)
	{
	}
}VERTEXBUMP, *PVERTEXBUMP;

// Bump Vertex
typedef struct Engine_DLL _tagVertexAnim
{
	Vector3	vPos;
	Vector3	vNormal;
	Vector2	vUV;
	Vector4	vWeights;
	Vector4	vIndices;

	_tagVertexAnim()
	{
	}

	_tagVertexAnim(const _tagVertexAnim& vtx)
	{
		vPos = vtx.vPos;
		vUV = vtx.vUV;
		vNormal = vtx.vNormal;
		vWeights = vtx.vWeights;
		vIndices = vtx.vIndices;
	}

	_tagVertexAnim(const Vector3& _vPos, const Vector3& _vNormal,
		const Vector2& _vUV, const Vector4& _vWeights,
		const Vector4& _vIndices) :
		vPos(_vPos),
		vNormal(_vNormal),
		vUV(_vUV),
		vWeights(_vWeights),
		vIndices(_vIndices)
	{
	}
}VERTEXANIM, *PVERTEXANIM;

// Bump Vertex
typedef struct Engine_DLL _tagVertexBumpAnim
{
	Vector3	vPos;
	Vector3	vNormal;
	Vector2	vUV;
	Vector3	vTangent;
	Vector3	vBinormal;
	Vector4	vWeights;
	Vector4	vIndices;

	_tagVertexBumpAnim()
	{
	}

	_tagVertexBumpAnim(const _tagVertexBumpAnim& vtx)
	{
		vPos = vtx.vPos;
		vUV = vtx.vUV;
		vNormal = vtx.vNormal;
		vTangent = vtx.vTangent;
		vBinormal = vtx.vBinormal;
		vWeights = vtx.vWeights;
		vIndices = vtx.vIndices;
	}

	_tagVertexBumpAnim(const Vector3& _vPos, const Vector3& _vNormal,
		const Vector2& _vUV, const Vector3& _vTangent,
		const Vector3& _vBinormal, const Vector4& _vWeights,
		const Vector4& _vIndices) :
		vPos(_vPos),
		vNormal(_vNormal),
		vUV(_vUV),
		vTangent(_vTangent),
		vBinormal(_vBinormal),
		vWeights(_vWeights),
		vIndices(_vIndices)
	{
	}
}VERTEXBUMPANIM, *PVERTEXBUMPANIM;

// Constant Buffer
typedef struct Engine_DLL _tagConstantBuffer
{
	int				iRegister;
	int				iSize;
	ID3D11Buffer*	pBuffer;
}CONSTANTBUFFER, *PCONSTANTBUFFER;

typedef struct Engine_DLL _tagTransformCBuffer
{
	Matrix	matWorld;
	Matrix	matView;
	Matrix	matProj;
	Matrix	matWV;
	Matrix	matWVP;
	Matrix	matWP;
	Matrix	matVP;
	Matrix	matInvProj;
	Matrix	matInvWVP;
	Vector3	vPivot;
	float	fEmpty;
	Vector3	vLength;
	float	fEmpty1;
}TRANSFORMCBUFFER, *PTRANSFORMCBUFFER;

typedef struct Engine_DLL _tagAnimationClip2D
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
		eType(A2D_ATLAS),
		eOption(A2DO_LOOP),
		pTexture(NULL),
		iFrameX(0),
		iFrameY(0),
		iFrameMaxX(1),
		iFrameMaxY(1),
		fAnimTime(0.f),
		fAnimLimitTime(1.f),
		iCount(0),
		iCountLimit(0),
		fOptionTime(0.f),
		fOptionTimeLimit(0.f),
		iLengthX(0),
		iLengthY(0),
		iStartY(0)
	{
	}
}ANIMATIONCLIP2D, *PANIMATIONCLIP2D;

typedef struct Engine_DLL _tagAnimation2DCBuffer
{
	int		iType;
	int		iFrameX;
	int		iFrameY;
	int		iLengthX;
	int		iLengthY;
	Vector3	vEmpty;
}ANIMATION2DBUFFER, *PANIMATION2DBUFFER;

// Button ConstantBuffer
typedef struct Engine_DLL _tagButtonCBuffer
{
	Vector4	vColor;
	float	fLight;
	Vector3	vEmpty;
}BUTTONCBUFFER, *PBUTTONCBUFFER;

// Rect Info
typedef struct Engine_DLL _tagRectInfo
{
	float	l;
	float	t;
	float	r;
	float	b;

	_tagRectInfo() :
		l(0.f),
		t(0.f),
		r(0.f),
		b(0.f)
	{
	}

	_tagRectInfo(float _l, float _t, float _r, float _b) :
		l(_l),
		t(_t),
		r(_r),
		b(_b)
	{
	}
}RECTINFO, *PRECTINFO;

typedef struct Engine_DLL _tagObb2DInfo
{
	Vector3	vCenter;
	Vector3	vAxisX;
	Vector3	vAxisY;
	float	fLengthX;
	float	fLengthY;
}OBB2DINFO, *POBB2DINFO;

typedef struct Engine_DLL _tagAABB
{
	Vector3	vMin;
	Vector3	vMax;
}AABB, *PAABB;

typedef struct Engine_DLL _tagMaterial
{
	Vector4	vDif;
	Vector4	vAmb;
	Vector4	vSpc;
	Vector4	vEmv;
}MATERIAL, *PMATERIAL;

typedef struct Engine_DLL _tagLight
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

typedef struct Engine_DLL _tagRay
{
	Vector3	vPos;
	Vector3	vDir;
	float	fDist;
	Vector3	vIntersect;
}RAY, *PRAY;

typedef struct Engine_DLL _tagSphere
{
	Vector3	vCenter;
	float	fRadius;
}SPHERE, *PSPHERE;

typedef struct Engine_DLL _tagParticleCBuffer
{
	Vector3		vCamAxisX;
	float		fEmpty1;
	Vector3		vCamAxisY;
	float		fEmpty2;
	Vector2		vSize;
	Vector2		vEmpty3;
}PARTICLECBUFFER, *PPARTICLECBUFFER;

typedef struct Engine_DLL _tagRendererCBuffer
{
	string	strName;
	int		iRegister;
	int		iSize;
	int		iShaderType;
	void*	pData;
}RENDERERCBUFFER, *PRENDERERCBUFFER;

typedef struct Engine_DLL _tagLandScapeCBuffer
{
	int		iDetailLevel;
	int		iSplatCount;
	Vector2	vEmpty;
}LANDSCAPECBUFFER, *PLANDSCAPECBUFFER;

typedef struct Engine_DLL _tagMultiTextureCBuffer
{
	Vector2 vAreaStart;
	Vector2 vAreaEnd;
}MULTITEXTURECBUFFER, *PMULTITEXTURECBUFFER;

Engine_END