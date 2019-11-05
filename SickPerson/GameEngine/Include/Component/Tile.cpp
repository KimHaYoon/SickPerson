#include "Tile.h"
#include "Transform.h"
#include "Camera.h"
#include "../Resources/ResourcesManager.h"
#include "../Resources/Mesh.h"
#include "../Rendering/ShaderManager.h"
#include "../Rendering/Shader.h"
#include "../Scene/Scene.h"

Engine_USING

CTile::CTile()
{
	SetTag("Tile");
	SetTypeName("CTile");
	SetTypeID<CTile>();
	m_eType = CT_TILE;
	m_eOption = TTO_NONE;
	m_iParentIndex = -1;
	m_iEdge = 0;
	m_iHuristic = 0;
	m_iTotal = 0;

//#ifdef _DEBUG
	m_pMesh = NULL;
	m_pShader = NULL;
	m_pLayout = NULL;
//#endif // _DEBUG

}

CTile::CTile(const CTile & tile)	:
	CComponent(tile)
{
	*this = tile;

	m_iParentIndex = -1;

//#ifdef _DEBUG
	if (m_pMesh)
		m_pMesh->AddRef();

	if(m_pShader)
		m_pShader->AddRef();
//#endif // _DEBUG

}

CTile::~CTile()
{
//#ifdef _DEBUG
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pMesh);
//#endif // _DEBUG
}

TERRAIN2D_TILE_OPTION CTile::GetOption() const
{
	return m_eOption;
}

void CTile::SetTileOption(TERRAIN2D_TILE_OPTION eOption)
{
//#ifdef _DEBUG
	m_eOption = eOption;

	switch (m_eOption)
	{
	case TTO_NONE:
		m_vLineColor = Vector4::Green;
		break;
	case TTO_NOMOVE:
		m_vLineColor = Vector4::Red;
		break;
	}
//#endif // _DEBUG
}

void CTile::SetLineColor(const Vector4 & vColor)
{
//#ifdef _DEBUG
	m_vLineColor = vColor;
//#endif // _DEBUG
}

Vector4 CTile::GetLineColor() const
{
//#ifdef _DEBUG
	return m_vLineColor;
//#endif // _DEBUG

	return Vector4();
}

bool CTile::Init()
{
//#ifdef _DEBUG
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("IsometricOrthoLTLine");
	m_pShader = GET_SINGLE(CShaderManager)->FindShader(COLLIDER_SHADER);
	//m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout("VertexColor");
	m_vLineColor = Vector4::Green;
//#endif // _DEBUG

	return true;
}

void CTile::Input(float fTime)
{
}

int CTile::Update(float fTime)
{
	return 0;
}

int CTile::LateUpdate(float fTime)
{
	return 0;
}

void CTile::Collision(float fTime)
{
}

void CTile::Render(float fTime)
{
//#ifdef _DEBUG

	TRANSFORMCBUFFER	m_tTransform;
	CCamera*	pCamera = m_pScene->GetMainCamera();

	m_tTransform.matWorld = m_pTransform->GetWorldMatrix();
	m_tTransform.matView = pCamera->GetViewMatrix();
	m_tTransform.matProj = pCamera->GetProjMatrix();
	m_tTransform.matWV = m_tTransform.matWorld * m_tTransform.matView;
	m_tTransform.matWVP = m_tTransform.matWV * m_tTransform.matProj;
	m_tTransform.vPivot = m_pTransform->GetPivot();
	m_tTransform.vLength = Vector3::One;
	m_tTransform.vLength.z = 0.f;

	m_tTransform.matWorld = XMMatrixTranspose(m_tTransform.matWorld.mat);
	m_tTransform.matView = XMMatrixTranspose(m_tTransform.matView.mat);
	m_tTransform.matProj = XMMatrixTranspose(m_tTransform.matProj.mat);
	m_tTransform.matWV = XMMatrixTranspose(m_tTransform.matWV.mat);
	m_tTransform.matWVP = XMMatrixTranspose(m_tTransform.matWVP.mat);

	SAFE_RELEASE(pCamera);

	switch (m_eOption)
	{
	case TTO_NONE:
		m_vLineColor = Vector4::Green;
		break;
	case TTO_NOMOVE:
		m_vLineColor = Vector4::Red;
		break;
	}

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
		&m_tTransform, SCT_VERTEX | SCT_PIXEL);
	GET_SINGLE(CShaderManager)->UpdateCBuffer("ColliderColor",
		&m_vLineColor, SCT_VERTEX);

	m_pShader->SetShader();
	GET_SINGLE(CShaderManager)->SetInputLayout("VertexColor");
	m_pMesh->Render(fTime);
//#endif // _DEBUG
}

CTile * CTile::Clone()
{
	return new CTile(*this);
}

void CTile::Save(FILE * pFile)
{
	CComponent::Save(pFile);

	fwrite(&m_eOption, 4, 1, pFile);
}

void CTile::Load(FILE * pFile)
{
	CComponent::Load(pFile);

	fread(&m_eOption, 4, 1, pFile);

//#ifdef _DEBUG
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("IsometricOrthoLTLine");
	m_pShader = GET_SINGLE(CShaderManager)->FindShader(COLLIDER_SHADER);
	//m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout("VertexColor");
	m_vLineColor = Vector4::Green;
//#endif // _DEBUG
}
