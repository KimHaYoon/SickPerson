#include "RenderingTarget.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "../Device.h"
#include "../Core/PathManager.h"
#include "../Resources/DirectXTex.h"
#include "../Resources/Mesh.h"
#include "../Resources/ResourcesManager.h"
#include "../Component/Camera.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"

CRenderingTarget::CRenderingTarget() :
	m_pTargetTex(NULL),
	m_pTargetView(NULL),
	m_pTargetSRV(NULL),
	m_pOldDepth(NULL),
	m_pOldTargetView(NULL),
	m_pDepthTex(NULL),
	m_pDepthView(NULL),
	m_pMesh(NULL),
	m_pShader(NULL),
	m_pLayout(NULL),
	m_bDebug(false)
{
	memset(m_fClearColor, 0, sizeof(float) * 4);
}

CRenderingTarget::~CRenderingTarget()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pDepthTex);
	SAFE_RELEASE(m_pDepthView);
	SAFE_RELEASE(m_pOldTargetView);
	SAFE_RELEASE(m_pOldDepth);
	SAFE_RELEASE(m_pTargetView);
	SAFE_RELEASE(m_pTargetSRV);
	SAFE_RELEASE(m_pTargetTex);
}

ID3D12RenderTargetView * CRenderingTarget::GetRenderTargetView() const
{
	return m_pTargetView;
}

ID3D12DepthStencilView * CRenderingTarget::GetDepthStencilView() const
{
	return m_pDepthView;
}

ID3D12ShaderResourceView * CRenderingTarget::GetShaderResourceView() const
{
	return m_pTargetSRV;
}

bool CRenderingTarget::GetDebugEnable() const
{
	return m_bDebug;
}

void CRenderingTarget::SetDebugEnable(bool bDebug)
{
	m_bDebug = bDebug;

	if (bDebug)
	{
		if (!m_pMesh)
			m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("RectOrthoLTTex");

		if (!m_pShader)
			m_pShader = GET_SINGLE(CShaderManager)->FindShader(UI_SHADER);

		if (!m_pLayout)
			m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout("VertexTex");
	}
}

bool CRenderingTarget::CreateTarget(unsigned int iW, unsigned int iH,
	DXGI_FORMAT eFmt, const Vector4& vClearColor,
	DXGI_FORMAT eDepthFmt)
{
	D3D12_TEXTURE2D_DESC	tDesc = {};
	tDesc.ArraySize = 1;
	tDesc.BindFlags = D3D12_BIND_SHADER_RESOURCE | D3D12_BIND_RENDER_TARGET;
	tDesc.Usage = D3D12_USAGE_DEFAULT;
	tDesc.Format = eFmt;
	tDesc.Height = iH;
	tDesc.Width = iW;
	tDesc.MipLevels = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.SampleDesc.Count = 1;

	for (int i = 0; i < 4; ++i)
	{
		m_fClearColor[i] = vClearColor[i];
	}

	if (FAILED(DEVICE->CreateTexture2D(&tDesc, NULL, &m_pTargetTex)))
		return false;

	if (FAILED(DEVICE->CreateRenderTargetView(m_pTargetTex, NULL, &m_pTargetView)))
		return false;

	if (FAILED(DEVICE->CreateShaderResourceView(m_pTargetTex, NULL, &m_pTargetSRV)))
		return false;

	if (eDepthFmt != DXGI_FORMAT_UNKNOWN)
	{
		memset(&tDesc, 0, sizeof(tDesc));
		tDesc.ArraySize = 1;
		tDesc.BindFlags = D3D12_BIND_DEPTH_STENCIL;
		tDesc.Usage = D3D12_USAGE_DEFAULT;
		tDesc.Format = eDepthFmt;
		tDesc.Height = iH;
		tDesc.Width = iW;
		tDesc.MipLevels = 1;
		tDesc.SampleDesc.Quality = 0;
		tDesc.SampleDesc.Count = 1;

		if (FAILED(DEVICE->CreateTexture2D(&tDesc, NULL, &m_pDepthTex)))
			return false;

		if (FAILED(DEVICE->CreateDepthStencilView(m_pDepthTex, NULL, &m_pDepthView)))
			return false;
	}

	return true;
}

void CRenderingTarget::SetDebugInfo(const Vector3 & vPos, const Vector3 & vScale)
{
	m_vPos = vPos;
	m_vScale = vScale;
}

void CRenderingTarget::SetTarget()
{
	if (m_pOldTargetView || m_pOldDepth)
		return;

	//float	fColor[4] = {};
	//ClearTarget(fColor);

	CMLIST->OMGetRenderTargets(1, &m_pOldTargetView, &m_pOldDepth);

	ID3D12DepthStencilView*	pDepthView = m_pOldDepth;

	if (m_pDepthView)
		pDepthView = m_pDepthView;

	CMLIST->OMSetRenderTargets(1, &m_pTargetView, pDepthView);
}

void CRenderingTarget::ResetTarget()
{
	if (!m_pOldTargetView)
		return;

	CMLIST->OMSetRenderTargets(1, &m_pOldTargetView, m_pOldDepth);
	SAFE_RELEASE(m_pOldDepth);
	SAFE_RELEASE(m_pOldTargetView);
}

void CRenderingTarget::SaveFile(const wchar_t * pFileName,
	const string & strPathKey)
{
	const wchar_t* pPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);

	wstring	strPath;

	if (pPath)
		strPath = pPath;

	strPath += pFileName;

	ScratchImage	img;
	CaptureTexture(DEVICE, CMLIST, m_pTargetTex, img);
	SaveToWICFile(img.GetImages(), img.GetImageCount(),
		WIC_FLAGS_NONE, GetWICCodec(WIC_CODEC_PNG), strPath.c_str());
}

void CRenderingTarget::SaveFileFromFullPath(const wchar_t * pFullPath)
{

	ScratchImage	img;
	CaptureTexture(DEVICE, CMLIST, m_pTargetTex, img);
	SaveToWICFile(img.GetImages(), img.GetImageCount(),
		WIC_FLAGS_NONE, GetWICCodec(WIC_CODEC_PNG), pFullPath);
}

void CRenderingTarget::ClearTarget()
{
	CMLIST->ClearRenderTargetView(m_pTargetView, m_fClearColor);

	if (m_pDepthView)
		CMLIST->ClearDepthStencilView(m_pDepthView, D3D12_CLEAR_DEPTH | D3D12_CLEAR_STENCIL, 1.f, 0);
}

void CRenderingTarget::DebugRender(float fTime)
{
	TRANSFORMCBUFFER	tBuffer = {};

	XMMATRIX	matView, matProj;

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CCamera*	pCamera = pScene->GetUICamera();

	SAFE_RELEASE(pScene);

	matView = pCamera->GetViewMatrix().mat;
	matProj = pCamera->GetProjMatrix().mat;

	SAFE_RELEASE(pCamera);

	Matrix	matScale, matPos;
	matScale = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	matPos = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);

	tBuffer.matWorld = matScale * matPos;
	tBuffer.matView = matView;
	tBuffer.matProj = matProj;
	tBuffer.matWV = tBuffer.matWorld * tBuffer.matView;
	tBuffer.matWVP = tBuffer.matWV * tBuffer.matProj;
	tBuffer.matWP = tBuffer.matWorld * tBuffer.matProj;
	tBuffer.matVP = matView * matProj;
	tBuffer.vPivot = Vector3::Zero;
	tBuffer.vLength = m_pMesh->GetLength();

	tBuffer.matWorld = XMMatrixTranspose(tBuffer.matWorld.mat);
	tBuffer.matView = XMMatrixTranspose(tBuffer.matView.mat);
	tBuffer.matProj = XMMatrixTranspose(tBuffer.matProj.mat);
	tBuffer.matWV = XMMatrixTranspose(tBuffer.matWV.mat);
	tBuffer.matWVP = XMMatrixTranspose(tBuffer.matWVP.mat);
	tBuffer.matWP = XMMatrixTranspose(tBuffer.matWP.mat);
	tBuffer.matVP = XMMatrixTranspose(tBuffer.matVP.mat);

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
		&tBuffer, SCT_VERTEX | SCT_PIXEL | SCT_GEOMETRY);

	ANIMATION2DBUFFER	tAniBuffer = {};
	tAniBuffer.iType = A2D_NONE;
	GET_SINGLE(CShaderManager)->UpdateCBuffer("Animation2D",
		&tAniBuffer, SCT_VERTEX | SCT_PIXEL);

	m_pShader->SetShader();
	CMLIST->IASetInputLayout(m_pLayout);
	CMLIST->PSSetShaderResources(0, 1, &m_pTargetSRV);

	m_pMesh->Render(fTime);
}

void CRenderingTarget::SetShader(int iRegister)
{
	CMLIST->PSSetShaderResources(iRegister, 1, &m_pTargetSRV);
}

void CRenderingTarget::ResetShader(int iRegister)
{
	ID3D12ShaderResourceView*	pView = NULL;
	CMLIST->PSSetShaderResources(iRegister, 1, &pView);
}
