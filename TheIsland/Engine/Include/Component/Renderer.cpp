#include "Renderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "../Device.h"
#include "../Resources/Mesh.h"
#include "../Resources/ResourcesManager.h"
#include "../Rendering/Shader.h"
#include "../Rendering/ShaderManager.h"
#include "../Rendering/RenderState.h"
#include "../Rendering/RenderManager.h"
#include "../Scene/Scene.h"

GAME_USING

CRenderer::CRenderer() :
	m_pMesh( NULL ),
	m_pShader( NULL ),
	m_pInputLayout( NULL )
{
	SetTag( "Renderer" );
	SetTypeName( "CRenderer" );
	SetTypeID<CRenderer>();
	m_eType = CT_RENDERER;

	memset( m_pRenderState, 0, sizeof( CRenderState* ) * RST_END );
}

CRenderer::CRenderer( const CRenderer & renderer ) :
	CComponent( renderer )
{
	*this = renderer;

	if ( m_pMesh )
		m_pMesh->AddRef();

	if ( m_pShader )
		m_pShader->AddRef();

	for ( int i = 0; i < RST_END; ++i )
	{
		m_pRenderState[i] = renderer.m_pRenderState[i];
		if ( m_pRenderState[i] )
			m_pRenderState[i]->AddRef();
	}
}

CRenderer::~CRenderer()
{
	SAFE_RELEASE( m_pMaterial );
	SAFE_RELEASE( m_pMesh );
	SAFE_RELEASE( m_pShader );

	for ( int i = 0; i < RST_END; ++i )
	{
		SAFE_RELEASE( m_pRenderState[i] );
	}
}

void CRenderer::SetMesh( const string & strKey )
{
	SAFE_RELEASE( m_pMesh );
	m_pMesh = GET_SINGLE( CResourcesManager )->FindMesh( strKey );
}

void CRenderer::SetMesh( CMesh * pMesh )
{
	SAFE_RELEASE( m_pMesh );
	pMesh->AddRef();
	m_pMesh = pMesh;
}

void CRenderer::SetShader( const string & strKey )
{
	SAFE_RELEASE( m_pShader );
	m_pShader = GET_SINGLE( CShaderManager )->FindShader( strKey );
}

void CRenderer::SetShader( CShader * pShader )
{
	SAFE_RELEASE( m_pShader );
	pShader->AddRef();
	m_pShader = pShader;
}

void CRenderer::SetInputLayout( const string & strKey )
{
	m_pInputLayout = GET_SINGLE( CShaderManager )->FindInputLayout( strKey );
}

void CRenderer::SetInputLayout( ID3D11InputLayout * pLayout )
{
	m_pInputLayout = pLayout;
}

CMaterial * CRenderer::CreateMaterial()
{
	m_pMaterial = new CMaterial;
	
	if ( !m_pMaterial->Init() )
	{
		SAFE_RELEASE( m_pMaterial );
		return NULL;
	}

	m_pMaterial->AddRef();

	return m_pMaterial;
}

void CRenderer::SetRenderState( const string & strKey )
{
	CRenderState* pState = GET_SINGLE( CRenderManager )->FindRenderState( strKey );

	if ( !pState )
		return;

	SAFE_RELEASE( m_pRenderState[pState->GetType()] );
	m_pRenderState[pState->GetType()] = pState;
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

	CLight* pLight = m_pScene->GetGlobalLight();

	if ( pLight )
	{
		pLight->SetShader();

		SAFE_RELEASE( pLight );
	}

	// 렌더 상태 적용
	for ( int i = 0; i < RST_END; ++i )
	{
		if ( m_pRenderState[i] )
			m_pRenderState[i]->SetState();
	}

	// Material 적용
	if ( m_pMaterial )
		m_pMaterial->SetMaterial( SCT_VERTEX | SCT_PIXEL );

	m_pShader->SetShader();

	CONTEXT->IASetInputLayout( m_pInputLayout );

	m_pMesh->Render( fTime );

	// 그린 후 모든 RenderState들을 복원한다.
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

void CRenderer::UpdateTransform()
{
	TRANSFORMCBUFFER	tBuffer = {};

	XMMATRIX	matView, matProj;

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
	tBuffer.vPivot = m_pTransform->GetPivot();
	tBuffer.vLength = m_pMesh->GetLength();

	tBuffer.matWorld = XMMatrixTranspose( tBuffer.matWorld.mat );
	tBuffer.matView = XMMatrixTranspose( tBuffer.matView.mat );
	tBuffer.matProj = XMMatrixTranspose( tBuffer.matProj.mat );
	tBuffer.matWV = XMMatrixTranspose( tBuffer.matWV.mat );
	tBuffer.matWVP = XMMatrixTranspose( tBuffer.matWVP.mat );
	tBuffer.matWP = XMMatrixTranspose( tBuffer.matWP.mat );

	GET_SINGLE( CShaderManager )->UpdateCBuffer( "Transform",
		&tBuffer, SCT_VERTEX | SCT_PIXEL );
}
