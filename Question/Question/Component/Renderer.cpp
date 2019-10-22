#include "Renderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"
#include "../Resources/Mesh.h"
#include "../Resources/ResourcesManager.h"
#include "../Device.h"
#include "../Scene/Scene.h"
#include "../GameObject/GameObject.h"
#include "../Rendering/Shader.h"
#include "../Rendering/ShaderManager.h"
#include "../Rendering/PipelineState.h"
#include "../Rendering/PipelineStateManager.h"
#include "../Rendering/RenderState.h"
#include "../Rendering/RenderManager.h"

CRenderer::CRenderer()	:
	m_pMesh(NULL),
	m_pShader(NULL),
	m_pInputLayout(NULL),
	m_pPipeline(NULL),
	m_bAlpha( false ),
	m_bChange(true)
{
	SetTag("Renderer");
	SetTypeName("CRenderer");
	SetTypeID<CRenderer>();
	m_eType = CT_RENDERER;
	memset( m_pRenderState, 0, sizeof( CRenderState* ) * RST_END );
}

CRenderer::CRenderer(const CRenderer & renderer)	:
	CComponent(renderer)
{
	*this = renderer;

	if (m_pMesh)
		m_pMesh->AddRef();

	if (m_pShader)
		m_pShader->AddRef();

	if ( m_pPipeline )
		m_pPipeline->AddRef();

	for ( int i = 0; i < RST_END; ++i )
	{
		m_pRenderState[i] = renderer.m_pRenderState[i];
		if ( m_pRenderState[i] )
			m_pRenderState[i]->AddRef();
	}
}

CRenderer::~CRenderer()
{
	SAFE_RELEASE( m_pMesh );
	SAFE_RELEASE( m_pShader );
	SAFE_RELEASE( m_pPipeline );


	for ( int i = 0; i < RST_END; ++i )
	{
		SAFE_RELEASE( m_pRenderState[i] );
	}
}

void CRenderer::SetMesh(const string & strKey)
{
	SAFE_RELEASE(m_pMesh);
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh(strKey);
}

void CRenderer::SetMesh(CMesh * pMesh)
{
	SAFE_RELEASE(m_pMesh);
	pMesh->AddRef();
	m_pMesh = pMesh;
}

void CRenderer::SetShader(const string & strKey)
{
	SAFE_RELEASE(m_pShader);
	m_pShader = GET_SINGLE(CShaderManager)->FindShader(strKey);
	m_bChange = true;
}

void CRenderer::SetShader(CShader * pShader)
{
	SAFE_RELEASE(m_pShader);
	pShader->AddRef();
	m_pShader = pShader;
	m_bChange = true;
}

void CRenderer::SetInputLayout(const string & strKey)
{
	m_pInputLayout = GET_SINGLE(CShaderManager)->FindInputLayout(strKey);
	m_bChange = true;
}

void CRenderer::SetInputLayout( D3D12_INPUT_LAYOUT_DESC * pLayout)
{
	m_pInputLayout = pLayout;
	m_bChange = true;
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
	m_bChange = true;
}


bool CRenderer::Init()
{
	m_pPipeline = GET_SINGLE( CPipelineStateManager )->CreatePipelineState( m_pGameObject->GetTag() );
	m_pPipeline->SetTag( m_pGameObject->GetTag() );
	if ( !m_pPipeline )
		return false;
	
	return true;
}

void CRenderer::Input(float fTime)
{
}

int CRenderer::Update(float fTime)
{
	return 0;
}

int CRenderer::LateUpdate(float fTime)
{
	return 0;
}

void CRenderer::Collision(float fTime)
{
}

void CRenderer::Render(float fTime)
{
	UpdateTransform();

	CLight*	pLight = m_pScene->GetGlobalLight("GlobalLight");

	if (pLight)
	{
		pLight->SetShader();

		SAFE_RELEASE(pLight);
	}
	
	if( m_pRenderState[RST_RASTERIZER] && m_pRenderState[RST_DEPTH] && m_pRenderState[RST_BLEND] )
		m_pPipeline->SetRenderState( m_pRenderState );
	m_pShader->SetShader( m_pPipeline->GetPipelineStateDesc());
	m_pPipeline->SetInputLayout( m_pInputLayout );
	if ( m_bChange )
	{
		m_pPipeline->CreatePipeline();
		m_bChange = false;
	}
	CMDLIST->SetPipelineState( m_pPipeline->GetPipelineState() );

	/*ID3D12DescriptorHeap* pHeap = GET_SINGLE( CDevice )->GetRTVHeap();
	CMDLIST->SetDescriptorHeaps( 1, &pHeap );
	CMDLIST->SetGraphicsRootDescriptorTable( 0, GET_SINGLE(CDevice)->GetRTVHeap()->GetGPUDescriptorHandleForHeapStart());
*/
	m_pMesh->Render(fTime);
}

CRenderer * CRenderer::Clone()
{
	return new CRenderer(*this);
}

void CRenderer::UpdateTransform()
{
	TRANSFORMCBUFFER	tBuffer = {};

	XMMATRIX	matView, matProj;
	/*matView = XMMatrixLookAtLH(Vector3(0.f, 0.f, -5.f).Convert(),
	Vector3::Zero.Convert(), Vector3::Axis[AXIS_Y].Convert());
	matProj = XMMatrixPerspectiveFovLH(WTF_PI / 3.f, 1280.f / 720.f,
	0.3f, 1000.f);*/
	CCamera*	pCamera = m_pScene->GetMainCamera();

	matView = pCamera->GetViewMatrix().mat;
	matProj = pCamera->GetProjMatrix().mat;

	SAFE_RELEASE(pCamera);

	tBuffer.matWorld = m_pTransform->GetLocalMatrix() *
		m_pTransform->GetWorldMatrix();
	tBuffer.matView = matView;
	tBuffer.matProj = matProj;
	tBuffer.matWV = tBuffer.matWorld * tBuffer.matView;
	tBuffer.matWVP = tBuffer.matWV * tBuffer.matProj;
	tBuffer.matWP = tBuffer.matWorld * tBuffer.matProj;
	tBuffer.vPivot = m_pTransform->GetPivot();
	tBuffer.vLength = m_pMesh->GetLength();

	tBuffer.matWorld = XMMatrixTranspose(tBuffer.matWorld.mat);
	tBuffer.matView = XMMatrixTranspose(tBuffer.matView.mat);
	tBuffer.matProj = XMMatrixTranspose(tBuffer.matProj.mat);
	tBuffer.matWV = XMMatrixTranspose(tBuffer.matWV.mat);
	tBuffer.matWVP = XMMatrixTranspose(tBuffer.matWVP.mat);
	tBuffer.matWP = XMMatrixTranspose(tBuffer.matWP.mat);

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
		&tBuffer, SCT_VERTEX | SCT_PIXEL);
}
