#pragma once
#include "../Obj.h"

typedef struct _tagVertexBuffer
{
	ID3D12Resource*	pBuffer;
	UINT			iVtxCount;
	UINT			iVtxSize;
	D3D12_PRIMITIVE_TOPOLOGY	ePrimitive;
	D3D12_VERTEX_BUFFER_VIEW	tView;
}VERTEXBUFFER, *PVERTEXBUFFER;


typedef struct _tagIndexBuffer
{
	ID3D12Resource*	pBuffer;
	UINT			iIdxCount;
	UINT			iIdxSize;
	DXGI_FORMAT		eFmt;
	D3D12_INDEX_BUFFER_VIEW	tView;
}INDEXBUFFER, *PINDEXBUFFER;

class CMesh :
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

	PVERTEXBUFFER	m_pUploadVB;
	PINDEXBUFFER	m_pUploadIB;

	string			m_strKey;
	Vector3			m_vLength;
	Vector3			m_vMin;
	Vector3			m_vMax;

public:
	Vector3 GetLength()	const;
	string GetKey()	const;
	void SetKey( const string& strKey );

public:
	bool CreateMesh( UINT iVtxCount, UINT iVtxSize,
		D3D12_PRIMITIVE_TOPOLOGY ePrimitive, void* pVtxData,
		UINT iIdxCount = 0, UINT iIdxSize = 0,
		DXGI_FORMAT eFmt = DXGI_FORMAT_R32_UINT,
		void* pIdxData = NULL );

private:
	bool CreateVertexBuffer( UINT iVtxCount, UINT iVtxSize,
		D3D12_PRIMITIVE_TOPOLOGY ePrimitive, void* pVtxData );
	bool CreateIndexBuffer( UINT iIdxCount = 0, UINT iIdxSize = 0,
		DXGI_FORMAT eFmt = DXGI_FORMAT_R32_UINT,
		void* pIdxData = NULL );

private:
	ID3D12Resource *CreateBufferResource(void *pData, UINT nBytes, D3D12_HEAP_TYPE d3dHeapType,
		D3D12_RESOURCE_STATES d3dResourceStates, ID3D12Resource **ppd3dUploadBuffer );

public:
	void Render( float fTime );
};