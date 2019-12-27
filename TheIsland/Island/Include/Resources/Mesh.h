#pragma once
#include "../Obj.h"

GAME_BEGIN


typedef struct GAME_DLL _tagVertexBuffer
{
	ID3D11Buffer*	pBuffer;
	UINT			iVtxCount;
	UINT			iVtxSize;
	D3D11_USAGE		eUsage;
	D3D11_PRIMITIVE_TOPOLOGY	ePrimitive;
}VERTEXBUFFER, *PVERTEXBUFFER;

typedef struct GAME_DLL _tagIndexBuffer
{
	ID3D11Buffer*	pBuffer;
	UINT			iIdxCount;
	UINT			iIdxSize;
	D3D11_USAGE		eUsage;
	DXGI_FORMAT		eFmt;
}INDEXBUFFER, *PINDEXBUFFER;

class GAME_DLL CMesh :
	public CObj
{
private:
	friend class CResourcesManager;

private:
	CMesh();
	~CMesh();

private:
	PVERTEXBUFFER	m_pVB;
	PINDEXBUFFER	m_pIB;
	string			m_strKey;
	Vector3			m_vLength;
	Vector3			m_vMin;
	Vector3			m_vMax;

public:
	Vector3 GetLength()	const;
	string GetKey()	const;
	void SetKey( const string& strKey );

public:
	bool CreateMesh( UINT iVtxCount, UINT iVtxSize, D3D11_USAGE eVtxUsage,
		D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void* pVtxData,
		UINT iIdxCount = 0, UINT iIdxSize = 0,
		D3D11_USAGE eIdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT eFmt = DXGI_FORMAT_R32_UINT,
		void* pIdxData = NULL );

private:
	bool CreateVertexBuffer( UINT iVtxCount, UINT iVtxSize, D3D11_USAGE eVtxUsage,
		D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void* pVtxData );
	bool CreateIndexBuffer( UINT iIdxCount = 0, UINT iIdxSize = 0,
		D3D11_USAGE eIdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT eFmt = DXGI_FORMAT_R32_UINT,
		void* pIdxData = NULL );

public:
	void Render( float fTime );
	bool CreateSphere( float radius, UINT numSubdivisions );
	void Subdivide( vector<XMFLOAT3>& vecPos, vector<UINT>& vecIndex );
	float AngleFromXY( float x, float y );
};


GAME_END