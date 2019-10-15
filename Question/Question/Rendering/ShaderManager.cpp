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
	Safe_Release_Map( m_mapLayout );
	Safe_Release_Map( m_mapShader );
}

bool CShaderManager::Init()
{
	char*	pEntry[ST_MAX] = { "StandardColorVS", "StandardColorPS", NULL };
	CShader*	pShader = LoadShader( STANDARD_COLOR_SHADER, L"Standard.fx",
		pEntry );

	SAFE_RELEASE( pShader );

	pEntry[ST_VERTEX] = "StandardColorNormalVS";
	pEntry[ST_PIXEL] = "StandardColorNormalPS";
	pEntry[ST_GEOMETRY] = NULL;
	pShader = LoadShader( STANDARD_COLOR_NORMAL_SHADER, L"Standard.fx",
		pEntry );

	SAFE_RELEASE( pShader );

	pEntry[ST_VERTEX] = "StandardTexVS";
	pEntry[ST_PIXEL] = "StandardTexPS";
	pEntry[ST_GEOMETRY] = NULL;
	pShader = LoadShader( STANDARD_TEX_SHADER, L"Standard.fx",
		pEntry );

	SAFE_RELEASE( pShader );

	pEntry[ST_VERTEX] = "StandardTexNormalVS";
	pEntry[ST_PIXEL] = "StandardTexNormalPS";
	pEntry[ST_GEOMETRY] = NULL;
	pShader = LoadShader( STANDARD_TEX_NORMAL_SHADER, L"Standard.fx",
		pEntry );

	SAFE_RELEASE( pShader );

	pEntry[ST_VERTEX] = "ColliderVS";
	pEntry[ST_PIXEL] = "ColliderPS";
	pEntry[ST_GEOMETRY] = NULL;
	pShader = LoadShader( COLLIDER_SHADER, L"Collider.fx",
		pEntry );

	SAFE_RELEASE( pShader );

	pEntry[ST_VERTEX] = "UIVS";
	pEntry[ST_PIXEL] = "UIPS";
	pEntry[ST_GEOMETRY] = NULL;
	pShader = LoadShader( UI_SHADER, L"UI.fx",
		pEntry );

	SAFE_RELEASE( pShader );

	pEntry[ST_VERTEX] = "UIVS";
	pEntry[ST_PIXEL] = "UIColorPS";
	pEntry[ST_GEOMETRY] = NULL;
	pShader = LoadShader( UI_COLOR_SHADER, L"UI.fx",
		pEntry );

	SAFE_RELEASE( pShader );

	pEntry[ST_VERTEX] = "UIVS";
	pEntry[ST_PIXEL] = "UIMultiTexturePS";
	pEntry[ST_GEOMETRY] = NULL;
	pShader = LoadShader( UI_MULTITEXTURE_SHADER, L"UI.fx",
		pEntry );

	SAFE_RELEASE( pShader );

	pEntry[ST_VERTEX] = "SkyVS";
	pEntry[ST_PIXEL] = "SkyPS";
	pEntry[ST_GEOMETRY] = NULL;
	pShader = LoadShader( SKY_SHADER, L"Sky.fx",
		pEntry );

	SAFE_RELEASE( pShader );

	pEntry[ST_VERTEX] = "EffectVS";
	pEntry[ST_PIXEL] = "EffectPS";
	pEntry[ST_GEOMETRY] = "EffectGS";
	pShader = LoadShader( EFFECT_SHADER, L"Effect.fx",
		pEntry );

	SAFE_RELEASE( pShader );

	pEntry[ST_VERTEX] = "LandScapeVS";
	pEntry[ST_PIXEL] = "LandScapePS";
	pEntry[ST_GEOMETRY] = NULL;
	pShader = LoadShader( LANDSCAPE_SHADER, L"LandScape.fx",
		pEntry );

	SAFE_RELEASE( pShader );

	pEntry[ST_VERTEX] = "StandardBumpVS";
	pEntry[ST_PIXEL] = "StandardBumpPS";
	pEntry[ST_GEOMETRY] = NULL;
	pShader = LoadShader( STANDARD_BUMP_SHADER, L"Standard.fx",
		pEntry );

	SAFE_RELEASE( pShader );

	pEntry[ST_VERTEX] = "StandardTexNormalAnimVS";
	pEntry[ST_PIXEL] = "StandardTexNormalPS";
	pEntry[ST_GEOMETRY] = NULL;
	pShader = LoadShader( STANDARD_TEX_NORMAL_ANIM_SHADER, L"Standard.fx",
		pEntry );

	SAFE_RELEASE( pShader );

	pEntry[ST_VERTEX] = "StandardBumpAnimVS";
	pEntry[ST_PIXEL] = "StandardBumpPS";
	pEntry[ST_GEOMETRY] = NULL;
	pShader = LoadShader( STANDARD_BUMP_ANIM_SHADER, L"Standard.fx",
		pEntry );

	SAFE_RELEASE( pShader );

	pEntry[ST_VERTEX] = "StandardBumpAnimVS";
	pEntry[ST_PIXEL] = "StandardBumpForwardPS";
	pEntry[ST_GEOMETRY] = NULL;
	pShader = LoadShader( STANDARD_BUMP_ANIM_FORWARD_SHADER, L"Standard.fx",
		pEntry );

	SAFE_RELEASE( pShader );

	pEntry[ST_VERTEX] = "LightVS";
	pEntry[ST_PIXEL] = "LightAccPS";
	pEntry[ST_GEOMETRY] = NULL;
	pShader = LoadShader( LIGHT_ACC_SHADER, L"Light.fx", pEntry );

	SAFE_RELEASE( pShader );

	pEntry[ST_VERTEX] = "LightVS";
	pEntry[ST_PIXEL] = "LightBlendPS";
	pEntry[ST_GEOMETRY] = NULL;
	pShader = LoadShader( LIGHT_BLEND_SHADER, L"Light.fx", pEntry );

	SAFE_RELEASE( pShader );

	pEntry[ST_VERTEX] = "LightVS";
	pEntry[ST_PIXEL] = "DeferredPS";
	pEntry[ST_GEOMETRY] = NULL;
	pShader = LoadShader( DEFERRED_SHADER, L"Light.fx", pEntry );

	SAFE_RELEASE( pShader );

	AddInputDesc( "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );

	CreateInputLayout( "VertexPos", SKY_SHADER );

	AddInputDesc( "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );
	AddInputDesc( "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
		16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );

	CreateInputLayout( "VertexColor", STANDARD_COLOR_SHADER );

	AddInputDesc( "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );
	AddInputDesc( "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );
	AddInputDesc( "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
		16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );

	CreateInputLayout( "VertexColorNormal", STANDARD_COLOR_NORMAL_SHADER );

	AddInputDesc( "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );
	AddInputDesc( "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
		8, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );

	CreateInputLayout( "VertexTex", STANDARD_TEX_SHADER );

	AddInputDesc( "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );
	AddInputDesc( "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );
	AddInputDesc( "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
		8, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );

	CreateInputLayout( "VertexTexNormal", STANDARD_TEX_NORMAL_SHADER );

	AddInputDesc( "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );
	AddInputDesc( "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );
	AddInputDesc( "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
		8, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );
	AddInputDesc( "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );
	AddInputDesc( "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );

	CreateInputLayout( "Bump", LANDSCAPE_SHADER );

	AddInputDesc( "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );
	AddInputDesc( "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );
	AddInputDesc( "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
		8, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );
	AddInputDesc( "BLENDWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
		16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );
	AddInputDesc( "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
		16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );

	CreateInputLayout( "VertexAnim", STANDARD_TEX_NORMAL_ANIM_SHADER );

	AddInputDesc( "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );
	AddInputDesc( "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );
	AddInputDesc( "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
		8, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );
	AddInputDesc( "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );
	AddInputDesc( "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );
	AddInputDesc( "BLENDWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
		16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );
	AddInputDesc( "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
		16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 );

	CreateInputLayout( "BumpAnim", STANDARD_BUMP_ANIM_SHADER );

	CreateCBuffer( "Transform", 0, sizeof( TRANSFORMCBUFFER ) );
	CreateCBuffer( "Material", 1, sizeof( MATERIAL ) );
	CreateCBuffer( "Light", 2, sizeof( LIGHT ) );

	CreateCBuffer( "Animation2D", 10, sizeof( ANIMATION2DBUFFER ) );

	CreateCBuffer( "ColliderColor", 13, sizeof( Vector4 ) );

	CreateCBuffer( "Button", 11, sizeof( BUTTONCBUFFER ) );

	CreateCBuffer( "Particle", 11, sizeof( PARTICLECBUFFER ) );

	CreateCBuffer( "LandScape", 12, sizeof( LANDSCAPECBUFFER ) );

	CreateCBuffer( "MultiTexture", 12, sizeof( MULTITEXTURECBUFFER ) );

	return true;
}

CShader * CShaderManager::LoadShader( const string & strKey,
	TCHAR * pFileName, char * pEntry[ST_MAX], const string & strPathKey )
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

	D3D12_INPUT_LAYOUT_DESC*		pInputLayout = NULL;

	pInputLayout->pInputElementDescs = &m_vecInputDesc[0];
	pInputLayout->NumElements = m_vecInputDesc.size();

	SAFE_RELEASE( pShader );

	m_vecInputDesc.clear();
	m_iInputSize = 0;

	m_mapLayout.insert( make_pair( strKey, pInputLayout ) );

	return true;
}

void CShaderManager::SetInputLayout( const string & strKey )
{
	D3D12_INPUT_LAYOUT_DESC*	pInputLayout = FindInputLayout( strKey );

	if ( !pInputLayout )
		return;

	//CONTEXT->IASetInputLayout( pInputLayout );
}

D3D12_INPUT_LAYOUT_DESC * CShaderManager::FindInputLayout( const string & strKey )
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
	pBuffer->iSize = iSize;

	D3D12_BUFFER_DESC	tDesc = {};

	tDesc.ByteWidth = iSize;
	tDesc.BindFlags = D3D12_BIND_CONSTANT_BUFFER;
	tDesc.Usage = D3D12_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D12_CPU_ACCESS_WRITE;

	if ( FAILED( DEVICE->CreateBuffer( &tDesc, NULL, &pBuffer->pBuffer ) ) )
		return false;

	m_mapCBuffer.insert( make_pair( strKey, pBuffer ) );

	return true;
}

void CShaderManager::UpdateCBuffer( const string & strKey, void * pData,
	int iShaderConstantType )
{
	PCONSTANTBUFFER	pBuffer = FindCBuffer( strKey );

	if ( !pBuffer )
		return;

	D3D12_MAPPED_SUBRESOURCE	tMap = {};

	FENCE->Map( pBuffer->pBuffer, 0, D3D12_MAP_WRITE_DISCARD,
		0, &tMap );

	memcpy( tMap.pData, pData, pBuffer->iSize );

	CMDLIST->Unmap( pBuffer->pBuffer, 0 );

	if ( iShaderConstantType & SCT_VERTEX )
		CMDLIST->VSSetConstantBuffers( pBuffer->iRegister, 1, &pBuffer->pBuffer );

	if ( iShaderConstantType & SCT_PIXEL )
		CMDLIST->PSSetConstantBuffers( pBuffer->iRegister, 1, &pBuffer->pBuffer );

	if ( iShaderConstantType & SCT_GEOMETRY )
		CMDLIST->GSSetConstantBuffers( pBuffer->iRegister, 1, &pBuffer->pBuffer );
}

PCONSTANTBUFFER CShaderManager::FindCBuffer( const string & strKey )
{
	unordered_map<string, PCONSTANTBUFFER>::iterator	iter = m_mapCBuffer.find( strKey );

	if ( iter == m_mapCBuffer.end() )
		return NULL;

	return iter->second;
}
