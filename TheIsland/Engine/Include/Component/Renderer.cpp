#include "Renderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "Animation.h"
#include "../Device.h"
#include "../Rendering/RenderManager.h"
#include "../Rendering/RenderState.h"
#include "../GameObject/GameObject.h"
#include "../Resources/Mesh.h"
#include "../Resources/ResourcesManager.h"
#include "../Rendering/ShaderManager.h"
#include "../Rendering/Shader.h"
#include "../Scene/Scene.h"

GAME_USING

CRenderer::CRenderer() :
	m_pMesh( NULL ),
	m_pShader( NULL ),
	m_pForwardShader( NULL ),
	m_pInputLayout( NULL ),
	m_pShadowShader( NULL ),
	m_bAlpha( false )
{
	SetTag( "Renderer" );
	SetTypeName( "CRenderer" );
	SetTypeID<CRenderer>();
	m_eType = CT_RENDERER;
	memset( m_pRenderState, 0, sizeof( CRenderState* ) * RST_END );
	//SetShadowShader(STANDARD_SHADOWMAP_BUMPANIM_SHADER);
}

CRenderer::CRenderer( const CRenderer & renderer ) :
	CComponent( renderer )
{
	*this = renderer;

	m_vecMaterials.clear();

	for ( size_t i = 0; i < renderer.m_vecMaterials.size(); ++i )
	{
		vector<CMaterial*>	vecMtrl;

		for ( size_t j = 0; j < renderer.m_vecMaterials[i].size(); ++j )
		{
			CMaterial*	pMaterial = renderer.m_vecMaterials[i][j]->Clone();

			vecMtrl.push_back( pMaterial );
		}

		m_vecMaterials.push_back( vecMtrl );
	}

	if ( m_pMesh )
		m_pMesh->AddRef();

	if ( m_pShader )
		m_pShader->AddRef();

	if ( m_pShadowShader )
		m_pShadowShader->AddRef();

	if ( m_pForwardShader )
		m_pForwardShader->AddRef();

	m_mapCBuffer.clear();

	unordered_map<int, PRENDERERCBUFFER>::const_iterator	iter;
	unordered_map<int, PRENDERERCBUFFER>::const_iterator	iterEnd = renderer.m_mapCBuffer.end();

	for ( iter = renderer.m_mapCBuffer.begin(); iter != iterEnd; ++iter )
	{
		PRENDERERCBUFFER	pBuffer = new RENDERERCBUFFER;

		*pBuffer = *iter->second;
		pBuffer->pData = new char[pBuffer->iSize];

		m_mapCBuffer.insert( make_pair( pBuffer->iRegister, pBuffer ) );
	}

	for ( int i = 0; i < RST_END; ++i )
	{
		m_pRenderState[i] = renderer.m_pRenderState[i];
		if ( m_pRenderState[i] )
			m_pRenderState[i]->AddRef();
	}
}

CRenderer::~CRenderer()
{
	SAFE_RELEASE( m_pShadowShader );

	unordered_map<int, PRENDERERCBUFFER>::iterator	iter;
	unordered_map<int, PRENDERERCBUFFER>::iterator	iterEnd = m_mapCBuffer.end();

	for ( iter = m_mapCBuffer.begin(); iter != iterEnd; ++iter )
	{
		SAFE_DELETE_ARRAY( iter->second->pData );
		SAFE_DELETE( iter->second );
	}

	m_mapCBuffer.clear();

	for ( size_t i = 0; i < m_vecMaterials.size(); ++i )
	{
		Safe_Release_VecList( m_vecMaterials[i] );
	}

	SAFE_RELEASE( m_pMesh );
	SAFE_RELEASE( m_pShader );
	SAFE_RELEASE( m_pForwardShader );

	for ( int i = 0; i < RST_END; ++i )
	{
		SAFE_RELEASE( m_pRenderState[i] );
	}
}

CMaterial * CRenderer::GetMaterial( int iContainer, int iSubset ) const
{
	if ( m_vecMaterials.size() <= iContainer )
		return NULL;

	else if ( m_vecMaterials[iContainer].size() <= iSubset )
		return NULL;

	m_vecMaterials[iContainer][iSubset]->AddRef();

	return m_vecMaterials[iContainer][iSubset];
}

bool CRenderer::GetAlphaEnable() const
{
	return m_bAlpha;
}

void CRenderer::AlphaEnable( bool bAlpha )
{
	m_bAlpha = bAlpha;

	if ( bAlpha )
	{
		SAFE_RELEASE( m_pRenderState[RST_BLEND] );
		m_pRenderState[RST_BLEND] = GET_SINGLE( CRenderManager )->FindRenderState( ALPHA_BLEND );
	}
}

CMesh * CRenderer::GetMesh() const
{
	if ( m_pMesh )
		m_pMesh->AddRef();
	return m_pMesh;
}

bool CRenderer::CreateCBuffer( const string & strName, int iRegister, int iSize, int iShaderType )
{
	PRENDERERCBUFFER	pBuffer = FindCBuffer( iRegister );

	if ( pBuffer )
		return false;

	pBuffer = new RENDERERCBUFFER;

	pBuffer->iSize = iSize;
	pBuffer->iRegister = iRegister;
	pBuffer->iShaderType = iShaderType;
	pBuffer->strName = strName;
	pBuffer->pData = new char[pBuffer->iSize];

	m_mapCBuffer.insert( make_pair( pBuffer->iRegister, pBuffer ) );

	return true;
}

PRENDERERCBUFFER CRenderer::FindCBuffer( int iRegister )
{
	unordered_map<int, PRENDERERCBUFFER>::iterator	iter = m_mapCBuffer.find( iRegister );

	if ( iter == m_mapCBuffer.end() )
		return NULL;

	return iter->second;
}

void CRenderer::UpdateCBuffer( const string & strName, int iRegister, int iSize, int iShaderType, void * pData )
{
	PRENDERERCBUFFER	pBuffer = FindCBuffer( iRegister );

	if ( !pBuffer )
	{
		CreateCBuffer( strName, iRegister, iSize, iShaderType );
		pBuffer = FindCBuffer( iRegister );
	}

	memcpy( pBuffer->pData, pData, pBuffer->iSize );
}

void CRenderer::SetMesh( const string & strKey )
{
	SAFE_RELEASE( m_pMesh );
	m_pMesh = GET_SINGLE( CResourcesManager )->FindMesh( strKey );

	CAnimation*	pAnimation = m_pMesh->CloneAnimation();

	if ( pAnimation )
	{
		m_pGameObject->AddComponent( pAnimation );

		SAFE_RELEASE( pAnimation );
	}

	SAFE_RELEASE( m_pShader );

	m_strShaderKey = m_pMesh->GetShaderKey();
	m_strInputLayoutKey = m_pMesh->GetInputLayoutKey();

	SetShader( m_pMesh->GetShaderKey() );
	//m_pShader = GET_SINGLE(CShaderManager)->FindShader(m_pMesh->GetShaderKey());
	m_pInputLayout = GET_SINGLE( CShaderManager )->FindInputLayout( m_pMesh->GetInputLayoutKey() );
	//SetShadowShader(m_pMesh->GetShadowShaderKey());
}

void CRenderer::SetMesh( CMesh * pMesh )
{
	SAFE_RELEASE( m_pMesh );
	pMesh->AddRef();
	m_pMesh = pMesh;

	CAnimation*	pAnimation = m_pMesh->CloneAnimation();

	if ( pAnimation )
	{
		m_pGameObject->AddComponent( pAnimation );

		SAFE_RELEASE( pAnimation );
	}

	SAFE_RELEASE( m_pShader );

	m_strShaderKey = m_pMesh->GetShaderKey();
	m_strInputLayoutKey = m_pMesh->GetInputLayoutKey();

	SetShader( m_pMesh->GetShaderKey() );
	//m_pShader = GET_SINGLE(CShaderManager)->FindShader(m_pMesh->GetShaderKey());
	m_pInputLayout = GET_SINGLE( CShaderManager )->FindInputLayout( m_pMesh->GetInputLayoutKey() );
}

void CRenderer::SetMesh( const string & strKey, const wchar_t * pFileName,
	const string & strPathKey )
{
	SAFE_RELEASE( m_pMesh );
	m_pMesh = GET_SINGLE( CResourcesManager )->LoadMesh( strKey, pFileName, strPathKey );

	CAnimation*	pAnimation = m_pMesh->CloneAnimation();

	if ( pAnimation )
	{
		m_pGameObject->AddComponent( pAnimation );

		SAFE_RELEASE( pAnimation );
	}

	SAFE_RELEASE( m_pShader );

	m_strShaderKey = m_pMesh->GetShaderKey();
	m_strInputLayoutKey = m_pMesh->GetInputLayoutKey();

	SetShader( m_pMesh->GetShaderKey() );
	//m_pShader = GET_SINGLE(CShaderManager)->FindShader(m_pMesh->GetShaderKey());
	m_pInputLayout = GET_SINGLE( CShaderManager )->FindInputLayout( m_pMesh->GetInputLayoutKey() );

	for ( size_t i = 0; i < m_vecMaterials.size(); ++i )
	{
		Safe_Release_VecList( m_vecMaterials[i] );
	}

	m_vecMaterials.clear();

	for ( int i = 0; i < m_pMesh->GetContainerCount(); ++i )
	{
		vector<class CMaterial*>	vec;

		for ( int j = 0; j < m_pMesh->GetSubsetCount( i ); ++j )
		{
			CMaterial*	pMtrl = m_pMesh->CloneMaterial( i, j );

			vec.push_back( pMtrl );
		}

		m_vecMaterials.push_back( vec );
	}
}

void CRenderer::SetMeshFromFullPath( const string & strKey,
	const wchar_t * pFullPath )
{
	SAFE_RELEASE( m_pMesh );
	m_pMesh = GET_SINGLE( CResourcesManager )->LoadMeshFromFullPath( strKey, pFullPath );

	CAnimation*	pAnimation = m_pMesh->CloneAnimation();

	if ( pAnimation )
	{
		m_pGameObject->AddComponent( pAnimation );

		SAFE_RELEASE( pAnimation );
	}

	SAFE_RELEASE( m_pShader );

	m_strShaderKey = m_pMesh->GetShaderKey();
	m_strInputLayoutKey = m_pMesh->GetInputLayoutKey();

	SetShader( m_pMesh->GetShaderKey() );
	//m_pShader = GET_SINGLE(CShaderManager)->FindShader(m_pMesh->GetShaderKey());
	m_pInputLayout = GET_SINGLE( CShaderManager )->FindInputLayout( m_pMesh->GetInputLayoutKey() );

	for ( size_t i = 0; i < m_vecMaterials.size(); ++i )
	{
		Safe_Release_VecList( m_vecMaterials[i] );
	}

	m_vecMaterials.clear();

	for ( int i = 0; i < m_pMesh->GetContainerCount(); ++i )
	{
		vector<class CMaterial*>	vec;

		for ( int j = 0; j < m_pMesh->GetSubsetCount( i ); ++j )
		{
			CMaterial*	pMtrl = m_pMesh->CloneMaterial( i, j );

			vec.push_back( pMtrl );
		}

		m_vecMaterials.push_back( vec );
	}
}

void CRenderer::SetShader( const string & strKey )
{
	SAFE_RELEASE( m_pShader );
	m_strShaderKey = strKey;
	m_pShader = GET_SINGLE( CShaderManager )->FindShader( strKey );

}

void CRenderer::SetShader( CShader * pShader )
{
	SAFE_RELEASE( m_pShader );
	pShader->AddRef();
	m_pShader = pShader;
	m_strShaderKey = pShader->GetKey();

}

void CRenderer::SetShadowShader( const string & strKey )
{
	SAFE_RELEASE( m_pShadowShader );
	m_strShaderKey = strKey;
	m_pShadowShader = GET_SINGLE( CShaderManager )->FindShader( strKey );

	if ( !m_pShadowShader )
		int a = 10;
}

void CRenderer::SetForwardShader( const string & strKey )
{
	SAFE_RELEASE( m_pForwardShader );
	m_strForwardShaderKey = strKey;
	m_pForwardShader = GET_SINGLE( CShaderManager )->FindShader( strKey );
}

void CRenderer::SetForwardShader( CShader * pShader )
{
	SAFE_RELEASE( m_pForwardShader );
	pShader->AddRef();
	m_pForwardShader = pShader;
}

void CRenderer::SetForwardShader()
{
	if ( m_strShaderKey == STANDARD_BUMP_ANIM_SHADER )
	{
		m_strForwardShaderKey = STANDARD_BUMP_ANIM_FORWARD_SHADER;
	}

	else if ( m_strShaderKey == STANDARD_TEX_NORMAL_ANIM_SHADER )
	{
		m_strForwardShaderKey = STANDARD_TEX_NORMAL_ANIM_FORWARD_SHADER;
	}

	SAFE_RELEASE( m_pForwardShader );
	m_pForwardShader = GET_SINGLE( CShaderManager )->FindShader( m_strForwardShaderKey );
}

void CRenderer::SetInputLayout( const string & strKey )
{
	m_pInputLayout = GET_SINGLE( CShaderManager )->FindInputLayout( strKey );
}

void CRenderer::SetInputLayout( ID3D11InputLayout * pLayout )
{
	m_pInputLayout = pLayout;
}

CMaterial * CRenderer::CreateMaterial( int iContainer )
{
	if ( m_vecMaterials.empty() )
	{
		vector<CMaterial*>	vec;
		m_vecMaterials.push_back( vec );
	}

	CMaterial* pMaterial = new CMaterial;

	if ( !pMaterial->Init() )
	{
		SAFE_RELEASE( pMaterial );
		return NULL;
	}

	pMaterial->AddRef();

	m_vecMaterials[iContainer].push_back( pMaterial );

	return pMaterial;
}

void CRenderer::SetRenderState( const string & strKey )
{
	CRenderState*	pState = GET_SINGLE( CRenderManager )->FindRenderState( strKey );

	if ( !pState )
		return;

	SAFE_RELEASE( m_pRenderState[pState->GetType()] );
	m_pRenderState[pState->GetType()] = pState;

	if ( strKey == ALPHA_BLEND )
		m_bAlpha = true;
}

bool CRenderer::Init()
{
	return true;
}

void CRenderer::Input( float fTime )
{
}

int CRenderer::Update( float fTime )
{
	return 0;
}

int CRenderer::LateUpdate( float fTime )
{
	return 0;
}

void CRenderer::Collision( float fTime )
{
}

void CRenderer::Render( float fTime )
{
	UpdateTransform();

	unordered_map<int, PRENDERERCBUFFER>::iterator	iter;
	unordered_map<int, PRENDERERCBUFFER>::iterator	iterEnd = m_mapCBuffer.end();

	for ( iter = m_mapCBuffer.begin(); iter != iterEnd; ++iter )
	{
		GET_SINGLE( CShaderManager )->UpdateCBuffer( iter->second->strName,
			iter->second->pData, iter->second->iShaderType );
	}

	/*CLight*	pLight = m_pScene->GetGlobalLight("GlobalLight");

	if (pLight)
	{
		pLight->SetShader();

		SAFE_RELEASE(pLight);
	}*/

	// 렌더상태 적용
	for ( int i = 0; i < RST_END; ++i )
	{
		if ( m_pRenderState[i] )
			m_pRenderState[i]->SetState();
	}

	// 본 텍스쳐를 얻어온다.
	CAnimation*	pAnimation = FindComponentFromType<CAnimation>( CT_ANIMATION );

	if ( pAnimation )
	{
		ID3D11ShaderResourceView*	pBoneSRV = pAnimation->GetBoneTexture();
		CONTEXT->VSSetShaderResources( 7, 1, &pBoneSRV );
	}

	m_pShader->SetShader();

	CONTEXT->IASetInputLayout( m_pInputLayout );

	for ( size_t i = 0; i < m_vecMaterials.size(); ++i )
	{
		for ( size_t j = 0; j < m_vecMaterials[i].size(); ++j )
		{
			m_vecMaterials[i][j]->SetMaterial( SCT_VERTEX | SCT_PIXEL );
			m_pMesh->Render( fTime, i, j );
			m_vecMaterials[i][j]->ResetMaterial( SCT_VERTEX | SCT_PIXEL );
		}
	}

	if ( pAnimation )
	{
		ID3D11ShaderResourceView*	pBoneSRV = NULL;
		CONTEXT->VSSetShaderResources( 7, 1, &pBoneSRV );
		SAFE_RELEASE( pAnimation );
	}

	// 렌더상태 복원
	for ( int i = 0; i < RST_END; ++i )
	{
		if ( m_pRenderState[i] )
			m_pRenderState[i]->ResetState();
	}
}

CRenderer * CRenderer::Clone()
{
	return new CRenderer( *this );
}

void CRenderer::Save( FILE * pFile )
{
}

void CRenderer::Load( FILE * pFile )
{
}

void CRenderer::RenderShadowMap( float fTime )
{
	if ( !m_pShadowShader )
		return;

	UpdateShadowMapTransform();

	// 렌더상태 적용
	for ( int i = 0; i < RST_END; ++i )
	{
		if ( m_pRenderState[i] )
			m_pRenderState[i]->SetState();
	}

	// Bone Texture 를 얻어온다.
	CAnimation* pAnimation = FindComponentFromType<CAnimation>( CT_ANIMATION );

	if ( pAnimation )
	{
		ID3D11ShaderResourceView* pBoneSRV = pAnimation->GetBoneTexture();
		CONTEXT->VSSetShaderResources( 7, 1, &pBoneSRV );
	}

	m_pShadowShader->SetShader();

	CONTEXT->IASetInputLayout( m_pInputLayout );

	for ( size_t i = 0; i < m_vecMaterials.size(); ++i )
	{
		for ( size_t j = 0; j < m_vecMaterials[i].size(); ++j )
		{
			m_pMesh->Render( fTime, i, j );
		}
	}

	if ( pAnimation )
	{
		ID3D11ShaderResourceView* pBoneSRV = NULL;
		CONTEXT->VSSetShaderResources( 7, 1, &pBoneSRV );
		SAFE_RELEASE( pAnimation );
	}

	// 렌더상태 복원
	for ( int i = 0; i < RST_END; ++i )
	{
		if ( m_pRenderState[i] )
			m_pRenderState[i]->ResetState();
	}
}

void CRenderer::UpdateTransform()
{
	TRANSFORMCBUFFER	tBuffer = {};

	XMMATRIX	matView, matProj;
	/*matView = XMMatrixLookAtLH(Vector3(0.f, 0.f, -5.f).Convert(),
	Vector3::Zero.Convert(), Vector3::Axis[AXIS_Y].Convert());
	matProj = XMMatrixPerspectiveFovLH(GAME_PI / 3.f, 1280.f / 720.f,
	0.3f, 1000.f);*/
	CCamera*	pCamera = m_pScene->GetMainCamera();

	matView = pCamera->GetViewMatrix().mat;
	matProj = pCamera->GetProjMatrix().mat;

	SAFE_RELEASE( pCamera );

	tBuffer.matWorld = m_pTransform->GetLocalMatrix() *
		m_pTransform->GetWorldMatrix();
	tBuffer.matView = matView;
	tBuffer.matProj = matProj;
	tBuffer.matWV = tBuffer.matWorld * tBuffer.matView;
	tBuffer.matWVP = tBuffer.matWV * tBuffer.matProj;
	tBuffer.matWP = tBuffer.matWorld * tBuffer.matProj;
	tBuffer.matVP = matView * matProj;
	tBuffer.matInvProj = XMMatrixInverse( &XMMatrixDeterminant( matProj ),
		matProj );
	tBuffer.matInvWVP = XMMatrixInverse( &XMMatrixDeterminant( tBuffer.matWVP.mat ),
		tBuffer.matWVP.mat );

	tBuffer.vPivot = m_pTransform->GetPivot();
	tBuffer.vLength = m_pMesh->GetLength();

	tBuffer.matWorld = XMMatrixTranspose( tBuffer.matWorld.mat );
	tBuffer.matView = XMMatrixTranspose( tBuffer.matView.mat );
	tBuffer.matProj = XMMatrixTranspose( tBuffer.matProj.mat );
	tBuffer.matWV = XMMatrixTranspose( tBuffer.matWV.mat );
	tBuffer.matWVP = XMMatrixTranspose( tBuffer.matWVP.mat );
	tBuffer.matWP = XMMatrixTranspose( tBuffer.matWP.mat );
	tBuffer.matVP = XMMatrixTranspose( tBuffer.matVP.mat );
	tBuffer.matInvProj = XMMatrixTranspose( tBuffer.matInvProj.mat );
	tBuffer.matInvWVP = XMMatrixTranspose( tBuffer.matInvWVP.mat );

	GET_SINGLE( CShaderManager )->UpdateCBuffer( "Transform",
		&tBuffer, SCT_VERTEX | SCT_PIXEL | SCT_GEOMETRY );
}

void CRenderer::UpdateShadowMapTransform()
{
	TRANSFORMCBUFFER	tBuffer = {};

	XMMATRIX	matView, matProj;
	/*matView = XMMatrixLookAtLH(Vector3(0.f, 0.f, -5.f).Convert(),
	Vector3::Zero.Convert(), Vector3::Axis[AXIS_Y].Convert());
	matProj = XMMatrixPerspectiveFovLH(NEW_PI / 3.f, 1280.f / 720.f,
	0.3f, 1000.f);*/
	CCamera*	pCamera = m_pScene->GetMainCamera();

	matView = pCamera->GetShadowViewMatrix().mat;
	matProj = pCamera->GetShadowProjMatrix().mat;

	SAFE_RELEASE( pCamera );

	tBuffer.matWorld = m_pTransform->GetLocalMatrix() * m_pTransform->GetWorldMatrix();
	tBuffer.matView = matView;
	tBuffer.matProj = matProj;
	tBuffer.matWV = tBuffer.matWorld * tBuffer.matView;
	tBuffer.matWVP = tBuffer.matWV * tBuffer.matProj;
	tBuffer.matWP = tBuffer.matWorld * tBuffer.matProj;
	tBuffer.matVP = matView * matProj;
	tBuffer.matInvProj = XMMatrixInverse( &XMMatrixDeterminant( matProj ), matProj ); // XMMatrixDeterminant : 행렬식 계산matInvProj
	tBuffer.matInvWVP = XMMatrixInverse( &XMMatrixDeterminant( tBuffer.matWVP.mat ),
		tBuffer.matWVP.mat );
	tBuffer.vPivot = m_pTransform->GetPivot();
	tBuffer.vLength = m_pMesh->GetLength();

	tBuffer.matWorld = XMMatrixTranspose( tBuffer.matWorld.mat );
	tBuffer.matView = XMMatrixTranspose( tBuffer.matView.mat );
	tBuffer.matProj = XMMatrixTranspose( tBuffer.matProj.mat );
	tBuffer.matWV = XMMatrixTranspose( tBuffer.matWV.mat );
	tBuffer.matWVP = XMMatrixTranspose( tBuffer.matWVP.mat );
	tBuffer.matWP = XMMatrixTranspose( tBuffer.matWP.mat );
	tBuffer.matVP = XMMatrixTranspose( tBuffer.matVP.mat );
	tBuffer.matInvProj = XMMatrixTranspose( tBuffer.matInvProj.mat );
	tBuffer.matInvWVP = XMMatrixTranspose( tBuffer.matInvWVP.mat );

	GET_SINGLE( CShaderManager )->UpdateCBuffer( "Transform",
		&tBuffer, SCT_VERTEX | SCT_PIXEL | SCT_GEOMETRY );
}

void CRenderer::RenderForward( float fTime )
{
	if ( !m_pForwardShader )
		return;

	UpdateTransform();

	unordered_map<int, PRENDERERCBUFFER>::iterator	iter;
	unordered_map<int, PRENDERERCBUFFER>::iterator	iterEnd = m_mapCBuffer.end();

	for ( iter = m_mapCBuffer.begin(); iter != iterEnd; ++iter )
	{
		GET_SINGLE( CShaderManager )->UpdateCBuffer( iter->second->strName,
			iter->second->pData, iter->second->iShaderType );
	}

	CLight*	pLight = m_pScene->GetGlobalLight( "GlobalLight" );

	if ( pLight )
	{
		pLight->SetShader();

		SAFE_RELEASE( pLight );
	}

	// 렌더상태 적용
	for ( int i = 0; i < RST_END; ++i )
	{
		if ( m_pRenderState[i] )
			m_pRenderState[i]->SetState();
	}

	// 본 텍스쳐를 얻어온다.
	CAnimation*	pAnimation = FindComponentFromType<CAnimation>( CT_ANIMATION );

	if ( pAnimation )
	{
		ID3D11ShaderResourceView*	pBoneSRV = pAnimation->GetBoneTexture();
		CONTEXT->VSSetShaderResources( 7, 1, &pBoneSRV );
	}

	m_pForwardShader->SetShader();

	CONTEXT->IASetInputLayout( m_pInputLayout );

	for ( size_t i = 0; i < m_vecMaterials.size(); ++i )
	{
		for ( size_t j = 0; j < m_vecMaterials[i].size(); ++j )
		{
			m_vecMaterials[i][j]->SetMaterial( SCT_VERTEX | SCT_PIXEL );
			m_pMesh->Render( fTime, i, j );
			m_vecMaterials[i][j]->ResetMaterial( SCT_VERTEX | SCT_PIXEL );
		}
	}

	if ( pAnimation )
	{
		ID3D11ShaderResourceView*	pBoneSRV = NULL;
		CONTEXT->VSSetShaderResources( 7, 1, &pBoneSRV );
		SAFE_RELEASE( pAnimation );
	}

	// 렌더상태 복원
	for ( int i = 0; i < RST_END; ++i )
	{
		if ( m_pRenderState[i] )
			m_pRenderState[i]->ResetState();
	}
}
