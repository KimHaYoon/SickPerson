#include "ShaderManager.h"
#include "Shader.h"
#include "../Device.h"

DEFINITION_SINGLE( CShaderManager )

CShaderManager::CShaderManager()
{
	m_iInputSize = 0;
}

CShaderManager::~CShaderManager()
{
	unordered_map<string, PCONSTANTBUFFER>::iterator	iter;
	unordered_map<string, PCONSTANTBUFFER>::iterator	iterEnd = m_mapCBuffer.end();

	for ( iter = m_mapCBuffer.begin(); iter != iterEnd; ++iter )
	{
		SAFE_RELEASE( iter->second->pBuffer );
	}

	Safe_Delete_Map( m_mapCBuffer );
	Safe_Delete_Map( m_mapLayout );
	Safe_Release_Map( m_mapShader );
}

bool CShaderManager::Init()
{
	char*	pEntry[ST_MAX] = { "StandardColorVS", "StandardColorPS" };
	
	pEntry[ST_VERTEX] = "StandardColorNormalVS";
	pEntry[ST_PIXEL] = "StandardColorNormalPS";
	CShader* pShader = LoadShader( STANDARD_COLOR_NORMAL_SHADER, L"Standard.fx",
		pEntry );

	SAFE_RELEASE( pShader );

	AddInputDesc( "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );
	AddInputDesc( "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );
	AddInputDesc( "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
		16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );

	CreateInputLayout( "VertexColorNormal", STANDARD_COLOR_NORMAL_SHADER );

	CreateCBuffer( "Transform", 0, sizeof( TRANSFORMCBUFFER ) );
	CreateCBuffer( "Material", 1, sizeof( MATERIAL ) );
	CreateCBuffer( "Light", 2, sizeof( LIGHT ) );

	return true;
}

CShader * CShaderManager::LoadShader( const string & strKey,
	wchar_t * pFileName, char * pEntry[ST_MAX], const string & strPathKey )
{
	CShader*	pShader = FindShader( strKey );

	if ( pShader )
		return pShader;

	pShader = new CShader;

	if ( !pShader->LoadShader( strKey, pFileName, pEntry, strPathKey ) )
	{
		SAFE_RELEASE( pShader );
		return NULL;
	}

	pShader->AddRef();

	m_mapShader.insert( make_pair( strKey, pShader ) );

	return pShader;
}

CShader * CShaderManager::FindShader( const string & strKey )
{
	unordered_map<string, class CShader*>::iterator	iter = m_mapShader.find( strKey );

	if ( iter == m_mapShader.end() )
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

void CShaderManager::AddInputDesc( char * pName, int iSemanticIdx,
	DXGI_FORMAT eFmt, int iInputSlot, int iSize,
	D3D12_INPUT_CLASSIFICATION eClass, int iInstanceDataStep )
{
	D3D12_INPUT_ELEMENT_DESC	tDesc = {};
	tDesc.SemanticName = pName;
	tDesc.SemanticIndex = iSemanticIdx;
	tDesc.Format = eFmt;
	tDesc.InputSlot = iInputSlot;
	tDesc.AlignedByteOffset = m_iInputSize;
	tDesc.InputSlotClass = eClass;
	tDesc.InstanceDataStepRate = iInstanceDataStep;

	m_iInputSize += iSize;

	m_vecInputDesc.push_back( tDesc );
}

bool CShaderManager::CreateInputLayout( const string & strKey,
	const string& strShaderKey )
{
	if ( FindInputLayout( strKey ) )
		return false;

	CShader*	pShader = FindShader( strShaderKey );

	D3D12_INPUT_LAYOUT_DESC*		pInputLayout = new D3D12_INPUT_LAYOUT_DESC;

	pInputLayout->pInputElementDescs = &m_vecInputDesc[0];
	pInputLayout->NumElements = m_vecInputDesc.size();

	SAFE_RELEASE( pShader );

	m_vecInputDesc.clear();
	m_iInputSize = 0;

	m_mapLayout[strKey] = pInputLayout;

	return true;
}

void CShaderManager::SetInputLayout( const string & strKey )
{
	D3D12_INPUT_LAYOUT_DESC*	pInputLayout = FindInputLayout( strKey );

	if ( !pInputLayout )
		return;
}

D3D12_INPUT_LAYOUT_DESC* CShaderManager::FindInputLayout( const string & strKey )
{
	unordered_map<string, D3D12_INPUT_LAYOUT_DESC*>::iterator	iter = m_mapLayout.find( strKey );

	if ( iter == m_mapLayout.end() )
		return NULL;

	return iter->second;
}

bool CShaderManager::CreateCBuffer( const string & strKey, int iRegister, int iSize )
{
	if ( FindCBuffer( strKey ) )
		return false;

	PCONSTANTBUFFER	pBuffer = new CONSTANTBUFFER;

	pBuffer->iRegister = iRegister;
	pBuffer->iSize = (iSize + 255) & ~255;
	pBuffer->pBuffer = CreateBufferResource( NULL, pBuffer->iSize, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL );
	
	//[strKey] = pBuffer;
	m_mapCBuffer.insert( { strKey,pBuffer } );

	return true;
}

void CShaderManager::UpdateCBuffer( const string & strKey, void * pData,
	int iShaderConstantType )
{
	PCONSTANTBUFFER	pBuffer = FindCBuffer( strKey );

	if ( !pBuffer )
		return;

	pBuffer->pBuffer->Map( 0, NULL, &pData );

	//if ( iShaderConstantType & SCT_VERTEX )
	//	CMDLIST->VSSetConstantBuffers( pBuffer->iRegister, 1, &pBuffer->pBuffer );

	//if ( iShaderConstantType & SCT_PIXEL )
	//	CMDLIST->PSSetConstantBuffers( pBuffer->iRegister, 1, &pBuffer->pBuffer );

	//if ( iShaderConstantType & SCT_GEOMETRY )
	//	CMDLIST->GSSetConstantBuffers( pBuffer->iRegister, 1, &pBuffer->pBuffer );
}

PCONSTANTBUFFER CShaderManager::FindCBuffer( const string & strKey )
{
	unordered_map<string, PCONSTANTBUFFER>::iterator	iter = m_mapCBuffer.find( strKey );

	if ( iter == m_mapCBuffer.end() )
		return NULL;

	return iter->second;
}

ID3D12Resource * CShaderManager::CreateBufferResource( void * pData, UINT nBytes, D3D12_HEAP_TYPE d3dHeapType, D3D12_RESOURCE_STATES d3dResourceStates, ID3D12Resource ** ppd3dUploadBuffer )
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
