#include "Mesh.h"
#include "../Device.h"

CMesh::CMesh() :
	m_pVB( NULL ),
	m_pIB( NULL ),
	m_pUploadVB( NULL ),
	m_pUploadIB( NULL )
{
	SetTag( "Mesh" );
	SetTypeName( "CMesh" );
	SetTypeID<CMesh>();
}

CMesh::~CMesh()
{
	if ( m_pVB )
		SAFE_RELEASE( m_pVB->pBuffer );

	if ( m_pIB )
		SAFE_RELEASE( m_pIB->pBuffer );

	if ( m_pUploadVB )
		SAFE_RELEASE( m_pUploadVB->pBuffer );

	if ( m_pUploadIB )
		SAFE_RELEASE( m_pUploadIB->pBuffer );


	SAFE_DELETE( m_pVB );
	SAFE_DELETE( m_pIB );

	SAFE_DELETE( m_pUploadVB );
	SAFE_DELETE( m_pUploadIB );
}

Vector3 CMesh::GetLength() const
{
	return m_vLength;
}

string CMesh::GetKey() const
{
	return m_strKey;
}

void CMesh::SetKey( const string & strKey )
{
	m_strKey = strKey;
}

bool CMesh::CreateMesh( UINT iVtxCount, UINT iVtxSize, 
	D3D12_PRIMITIVE_TOPOLOGY ePrimitive, void * pVtxData, UINT iIdxCount,
	UINT iIdxSize,DXGI_FORMAT eFmt, void * pIdxData )
{
	if ( !CreateVertexBuffer( iVtxCount, iVtxSize, ePrimitive,
		pVtxData ) )
		return false;

	if ( !CreateIndexBuffer( iIdxCount, iIdxSize, eFmt,
		pIdxData ) )
		return false;

	m_vLength = m_vMax - m_vMin;

	return true;
}

bool CMesh::CreateVertexBuffer( UINT iVtxCount, UINT iVtxSize,
	D3D12_PRIMITIVE_TOPOLOGY ePrimitive,	void * pVtxData )
{
	if ( m_pVB )
		SAFE_RELEASE( m_pVB->pBuffer );

	SAFE_DELETE( m_pVB );

	m_pVB = new VERTEXBUFFER;

	m_pVB->iVtxCount = iVtxCount;
	m_pVB->iVtxSize = iVtxSize;
	m_pVB->ePrimitive = ePrimitive;

	m_pVB->pBuffer = CreateBufferResource( pVtxData, iVtxCount * iVtxSize, 
		D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pUploadVB->pBuffer );
	m_pVB->tView.BufferLocation = m_pVB->pBuffer->GetGPUVirtualAddress();
	m_pVB->tView.StrideInBytes = iVtxSize;
	m_pVB->tView.SizeInBytes = iVtxSize * iVtxCount;

	char*	pVtx = ( char* )pVtxData;

	// 정점의 Min, Max 값을 구한다.
	for ( int i = 0; i < iVtxCount; ++i )
	{
		Vector3	vPos;
		memcpy( &vPos, pVtx + iVtxSize * i, sizeof( Vector3 ) );

		if ( vPos.x < m_vMin.x )
			m_vMin.x = vPos.x;

		if ( vPos.y < m_vMin.y )
			m_vMin.y = vPos.y;

		if ( vPos.z < m_vMin.z )
			m_vMin.z = vPos.z;

		if ( vPos.x > m_vMax.x )
			m_vMax.x = vPos.x;

		if ( vPos.y > m_vMax.y )
			m_vMax.y = vPos.y;

		if ( vPos.z > m_vMax.z )
			m_vMax.z = vPos.z;

	}

	return true;
}

bool CMesh::CreateIndexBuffer( UINT iIdxCount, UINT iIdxSize,
	DXGI_FORMAT eFmt, void * pIdxData )
{
	if ( !pIdxData )
		return true;

	if ( m_pIB )
		SAFE_RELEASE( m_pIB->pBuffer );

	SAFE_DELETE( m_pIB );

	m_pIB = new INDEXBUFFER;

	m_pIB->iIdxCount = iIdxCount;
	m_pIB->iIdxSize = iIdxSize;
	m_pIB->eFmt = eFmt;

	m_pIB->pBuffer = CreateBufferResource( pIdxData, iIdxCount * iIdxSize,
		D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pUploadIB->pBuffer );
	m_pIB->tView.BufferLocation = m_pIB->pBuffer->GetGPUVirtualAddress();
	m_pIB->tView.Format = eFmt;
	m_pIB->tView.SizeInBytes = iIdxCount * iIdxSize;

	return true;
}

ID3D12Resource * CMesh::CreateBufferResource(void * pData, UINT nBytes, D3D12_HEAP_TYPE d3dHeapType, D3D12_RESOURCE_STATES d3dResourceStates, ID3D12Resource ** ppd3dUploadBuffer )
{
	ID3D12Resource *pd3dBuffer = NULL;
	D3D12_HEAP_PROPERTIES d3dHeapPropertiesDesc;
	::ZeroMemory( &d3dHeapPropertiesDesc, sizeof( D3D12_HEAP_PROPERTIES ) );
	d3dHeapPropertiesDesc.Type = d3dHeapType;
	d3dHeapPropertiesDesc.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	d3dHeapPropertiesDesc.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	d3dHeapPropertiesDesc.CreationNodeMask = 1;
	d3dHeapPropertiesDesc.VisibleNodeMask = 1;
	D3D12_RESOURCE_DESC d3dResourceDesc;
	::ZeroMemory( &d3dResourceDesc, sizeof( D3D12_RESOURCE_DESC ) );
	d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	d3dResourceDesc.Alignment = 0;
	d3dResourceDesc.Width = nBytes;
	d3dResourceDesc.Height = 1;
	d3dResourceDesc.DepthOrArraySize = 1;
	d3dResourceDesc.MipLevels = 1;
	d3dResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	d3dResourceDesc.SampleDesc.Count = 1;
	d3dResourceDesc.SampleDesc.Quality = 0;
	d3dResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	d3dResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	D3D12_RESOURCE_STATES d3dResourceInitialStates = D3D12_RESOURCE_STATE_COPY_DEST;
	if ( d3dHeapType == D3D12_HEAP_TYPE_UPLOAD ) d3dResourceInitialStates =
		D3D12_RESOURCE_STATE_GENERIC_READ;
	else if ( d3dHeapType == D3D12_HEAP_TYPE_READBACK ) d3dResourceInitialStates =
		D3D12_RESOURCE_STATE_COPY_DEST;
	HRESULT hResult = DEVICE->CreateCommittedResource( &d3dHeapPropertiesDesc,
		D3D12_HEAP_FLAG_NONE, &d3dResourceDesc, d3dResourceInitialStates, NULL,
		__uuidof( ID3D12Resource ), ( void ** )&pd3dBuffer );


	if ( pData )
	{
		switch ( d3dHeapType )
		{
		case D3D12_HEAP_TYPE_DEFAULT:
		{
			if ( ppd3dUploadBuffer )
			{
				//업로드 버퍼를 생성한다.
				d3dHeapPropertiesDesc.Type = D3D12_HEAP_TYPE_UPLOAD;
				DEVICE->CreateCommittedResource( &d3dHeapPropertiesDesc,
					D3D12_HEAP_FLAG_NONE, &d3dResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL,
					__uuidof( ID3D12Resource ), ( void ** )ppd3dUploadBuffer );
				//업로드 버퍼를 매핑하여 초기화 데이터를 업로드 버퍼에 복사한다.
				D3D12_RANGE d3dReadRange = { 0, 0 };
				UINT8 *pBufferDataBegin = NULL;
				( *ppd3dUploadBuffer )->Map( 0, &d3dReadRange, ( void ** )&pBufferDataBegin );
				memcpy( pBufferDataBegin, pData, nBytes );
				( *ppd3dUploadBuffer )->Unmap( 0, NULL );
				//업로드 버퍼의 내용을 디폴트 버퍼에 복사한다.

					//return;
				D3D12_RESOURCE_BARRIER d3dResourceBarrier;
				::ZeroMemory( &d3dResourceBarrier, sizeof( D3D12_RESOURCE_BARRIER ) );
				d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
				d3dResourceBarrier.Transition.pResource = pd3dBuffer;
				d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
				d3dResourceBarrier.Transition.StateAfter = d3dResourceStates;
				d3dResourceBarrier.Transition.Subresource =
					D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

				CMDLIST->CopyResource( pd3dBuffer, *ppd3dUploadBuffer );
				CMDLIST->ResourceBarrier( 1, &d3dResourceBarrier );
			}
			break;
		}
		case D3D12_HEAP_TYPE_UPLOAD:
		{
			D3D12_RANGE d3dReadRange = { 0, 0 };
			UINT8 *pBufferDataBegin = NULL;
			pd3dBuffer->Map( 0, &d3dReadRange, ( void ** )&pBufferDataBegin );
			memcpy( pBufferDataBegin, pData, nBytes );
			pd3dBuffer->Unmap( 0, NULL );
			break;
		}
		case D3D12_HEAP_TYPE_READBACK:
			break;
		}
	}
	return( pd3dBuffer );
}

void CMesh::Render( float fTime )
{
	// 버텍스버퍼를 지정한다.
	CMDLIST->IASetVertexBuffers( 0, 1, &m_pVB->tView);
	// 도형 위상구조를 설정한다.
	CMDLIST->IASetPrimitiveTopology( m_pVB->ePrimitive );

	//CMDLIST->DrawInstanced( m_pVB->iVtxCount, 1, 0, 0 );
	if ( m_pIB )
	{
		// 인덱스버퍼를 지정한다.
		CMDLIST->IASetIndexBuffer( &m_pIB->tView);
		CMDLIST->DrawIndexedInstanced( m_pIB->iIdxCount, 1, 0, 0, 0 );
	}

	else
	{
		CMDLIST->DrawInstanced( m_pVB->iVtxCount, 1, 0, 0 );
	}
}
