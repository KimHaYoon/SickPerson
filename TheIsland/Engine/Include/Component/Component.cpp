#include "Component.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"
#include "../Scene/Scene.h"
#include "../Scene/Layer.h"

GAME_USING

CComponent::CComponent() :
	m_pScene( NULL ),
	m_pLayer( NULL ),
	m_pGameObject( NULL ),
	m_pTransform( NULL )
{
}

CComponent::CComponent( const CComponent & com )
{
	*this = com;
	m_iRefCount = 1;
}

CComponent::~CComponent()
{
}

COMPONENT_TYPE CComponent::GetComponentType() const
{
	return m_eType;
}

void CComponent::SetScene( CScene * pScene )
{
	m_pScene = pScene;
}

void CComponent::SetLayer( CLayer * pLayer )
{
	m_pLayer = pLayer;
}

void CComponent::SetGameObject( CGameObject * pObj )
{
	m_pGameObject = pObj;
}

void CComponent::SetTransform( CTransform * pTransform )
{
	m_pTransform = pTransform;
}

CScene * CComponent::GetScene() const
{
	if ( m_pScene )
		m_pScene->AddRef();
	return m_pScene;
}

CLayer * CComponent::GetLayer() const
{
	if ( m_pLayer )
		m_pLayer->AddRef();
	return m_pLayer;
}

CGameObject * CComponent::GetGameObject() const
{
	if ( m_pGameObject )
		m_pGameObject->AddRef();
	return m_pGameObject;
}

CTransform * CComponent::GetTransform() const
{
	m_pTransform->AddRef();
	return m_pTransform;
}

bool CComponent::Init()
{
	return true;
}

void CComponent::Input( float fTime )
{
}

int CComponent::Update( float fTime )
{
	return 0;
}

int CComponent::LateUpdate( float fTime )
{
	return 0;
}

void CComponent::Collision( float fTime )
{
}

void CComponent::Render( float fTime )
{
}

void CComponent::Save( FILE * pFile )
{
	fwrite( &m_eType, 4, 1, pFile );
}

void CComponent::Load( FILE * pFile )
{
	fread( &m_eType, 4, 1, pFile );
}

void CComponent::OnCollisionEnter( CCollider * pSrc, CCollider * pDest, float fTime )
{
}

void CComponent::OnCollision( CCollider * pSrc, CCollider * pDest, float fTime )
{
}

void CComponent::OnCollisionLeave( CCollider * pSrc, CCollider * pDest, float fTime )
{
}

const list<CComponent*>* CComponent::FindComponentsFromTag( const string & strTag )
{
	return m_pGameObject->FindComponentsFromTag( strTag );
}

const list<CComponent*>* CComponent::FindComponentsFromTypeName( const string & strTypeName )
{
	return m_pGameObject->FindComponentsFromTypeName( strTypeName );
}

const list<CComponent*>* CComponent::FindComponentsFromType( COMPONENT_TYPE eType )
{
	return m_pGameObject->FindComponentsFromType( eType );
}
