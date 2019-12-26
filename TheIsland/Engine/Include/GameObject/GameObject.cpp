#include "GameObject.h"
#include "../Scene/Layer.h"
#include "../Component/Transform.h"
#include "../Core/CollisionManager.h"
#include "../Core/PathManager.h"
#include "../Component/Renderer2D.h"
#include "../Component/Animation2D.h"
#include "../Component/Camera.h"
#include "../Component/ColliderPoint.h"
#include "../Component/ColliderRect.h"
#include "../Component/Terrain2D.h"
#include "../Component/Tile.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"

GAME_USING

list<CGameObject*> CGameObject::m_ObjList;
list<CGameObject*> CGameObject::m_FindObjectList;
unordered_map<string, CGameObject*> CGameObject::m_mapPrototype;

CGameObject * CGameObject::CreateObject( const string & strTag,
	CLayer * pLayer )
{
	CGameObject*	pObj = new CGameObject;

	pObj->SetTag( strTag );

	if ( pLayer )
		pLayer->AddObject( pObj );

	if ( !pObj->Init() )
	{
		if ( pLayer )
			pLayer->EraseObject( pObj );

		CGameObject::EraseObj( pObj );
		SAFE_RELEASE( pObj );
		return NULL;
	}

	CGameObject::AddObjList( pObj );

	return pObj;
}

CGameObject * CGameObject::CreatePrototype( const string & strKey,
	class CScene* pScene )
{
	CGameObject*	pProto = FindPrototype( strKey );

	if ( pProto )
		return NULL;

	pProto = new CGameObject;

	if ( !pScene )
		pScene = GET_SINGLE( CSceneManager )->GetCurrentScene();

	pProto->SetScene( pScene );
	pProto->SetTag( strKey );

	if ( !pProto->Init() )
	{
		SAFE_RELEASE( pProto );
		return NULL;
	}

	pProto->AddRef();
	m_mapPrototype.insert( make_pair( strKey, pProto ) );

	return pProto;
}

CGameObject * CGameObject::CreateClone( const string & strKey, class CLayer* pLayer )
{
	CGameObject*	pProto = FindPrototype( strKey );

	if ( !pProto )
		return NULL;

	CGameObject*	pClone = pProto->Clone();

	if ( pLayer )
		pLayer->AddObject( pClone );

	CGameObject::AddObjList( pClone );

	return pClone;
}

void CGameObject::ChangePrototypeScene( const string & strKey,
	CScene * pScene )
{
	CGameObject*	pProto = FindPrototype( strKey );

	if ( !pProto )
		return;

	pProto->SetScene( pScene );
}

void CGameObject::AddObjList( CGameObject * pObj )
{
	pObj->AddRef();

	m_ObjList.push_back( pObj );
}

void CGameObject::ChangeLayer( const string & strTag, CLayer * pLayer )
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for ( iter = m_ObjList.begin(); iter != iterEnd; ++iter )
	{
		if ( ( *iter )->GetTag() == strTag )
		{
			( *iter )->ChangeLayer( pLayer );
			break;
		}
	}
}

void CGameObject::DontDestroy( const string & strTag, bool bDontDesetroy )
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for ( iter = m_ObjList.begin(); iter != iterEnd; ++iter )
	{
		if ( ( *iter )->GetTag() == strTag )
		{
			( *iter )->DontDestroyOnLoad( bDontDesetroy );
			break;
		}
	}
}

void CGameObject::EraseObj( CGameObject * pObj )
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for ( iter = m_ObjList.begin(); iter != iterEnd; ++iter )
	{
		if ( *iter == pObj )
		{
			SAFE_RELEASE( ( *iter ) );
			m_ObjList.erase( iter );
			return;
		}
	}
}

void CGameObject::EraseObj( const string & strTag )
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for ( iter = m_ObjList.begin(); iter != iterEnd; ++iter )
	{
		if ( ( *iter )->GetTag() == strTag )
		{
			SAFE_RELEASE( ( *iter ) );
			m_ObjList.erase( iter );
			return;
		}
	}
}

// 장면 넘어갈때 사용하기 편함.
void CGameObject::EraseObj()
{
	Safe_Release_VecList( m_FindObjectList );
	Safe_Release_VecList( m_ObjList );
}

void CGameObject::ErasePrototype( CScene * pScene )
{
	unordered_map<string, CGameObject*>::iterator	iter;
	unordered_map<string, CGameObject*>::iterator	iterEnd = m_mapPrototype.end();

	for ( iter = m_mapPrototype.begin(); iter != iterEnd;)
	{
		if ( iter->second->m_pScene == pScene )
		{
			SAFE_RELEASE( iter->second );
			iter = m_mapPrototype.erase( iter );
			iterEnd = m_mapPrototype.end();
		}

		else
			++iter;
	}
}

void CGameObject::ErasePrototype( CGameObject * pObj )
{
	unordered_map<string, CGameObject*>::iterator	iter;
	unordered_map<string, CGameObject*>::iterator	iterEnd = m_mapPrototype.end();

	for ( iter = m_mapPrototype.begin(); iter != iterEnd; ++iter )
	{
		if ( pObj == iter->second )
		{
			SAFE_RELEASE( iter->second );
			m_mapPrototype.erase( iter );
			return;
		}
	}
}

void CGameObject::ErasePrototype( const string & strKey )
{
	unordered_map<string, CGameObject*>::iterator	iter = m_mapPrototype.find( strKey );

	if ( iter == m_mapPrototype.end() )
		return;

	SAFE_RELEASE( iter->second );

	m_mapPrototype.erase( iter );
}

void CGameObject::ErasePrototype()
{
	Safe_Release_Map( m_mapPrototype );
}

CGameObject * CGameObject::FindObject( const string & strTag )
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for ( iter = m_ObjList.begin(); iter != iterEnd; ++iter )
	{
		if ( ( *iter )->GetTag() == strTag )
		{
			( *iter )->AddRef();
			return *iter;
		}
	}

	return NULL;
}

CGameObject * CGameObject::FindObject( COMPONENT_TYPE eType )
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for ( iter = m_ObjList.begin(); iter != iterEnd; ++iter )
	{
		if ( ( *iter )->CheckComponentFromType( eType ) )
		{
			( *iter )->AddRef();
			return *iter;
		}
	}

	return NULL;
}

const list<CGameObject*>* CGameObject::FindObjects( const string & strTag )
{
	Safe_Release_VecList( m_FindObjectList );

	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for ( iter = m_ObjList.begin(); iter != iterEnd; ++iter )
	{
		if ( ( *iter )->GetTag() == strTag )
		{
			( *iter )->AddRef();
			m_FindObjectList.push_back( *iter );
		}
	}

	return &m_FindObjectList;
}

CGameObject * CGameObject::FindPrototype( const string & strKey )
{
	unordered_map<string, CGameObject*>::iterator	iter = m_mapPrototype.find( strKey );

	if ( iter == m_mapPrototype.end() )
		return NULL;

	return iter->second;
}

CGameObject::CGameObject() :
	m_pScene( NULL ),
	m_pLayer( NULL ),
	m_pTransform( NULL ),
	m_pParent( NULL ),
	m_bDontDestroy( false )
{
	SetTag( "GameObject" );
	SetTypeName( "CGameObject" );
	SetTypeID<CGameObject>();
}

CGameObject::CGameObject( const CGameObject & obj )
{
	*this = obj;
	m_iRefCount = 1;

	m_pTransform = obj.m_pTransform->Clone();
	m_pTransform->SetGameObject( this );
	m_pTransform->SetTransform( m_pTransform );

	m_ComList.clear();

	list<CComponent*>::const_iterator	iter;
	list<CComponent*>::const_iterator	iterEnd = obj.m_ComList.end();

	for ( iter = obj.m_ComList.begin(); iter != iterEnd; ++iter )
	{
		CComponent*	pClone = ( *iter )->Clone();

		pClone->SetGameObject( this );
		pClone->SetTransform( m_pTransform );

		m_ComList.push_back( pClone );
	}

	m_ChildList.clear();

	list<CGameObject*>::const_iterator	iterC;
	list<CGameObject*>::const_iterator	iterCEnd = obj.m_ChildList.end();

	for ( iterC = obj.m_ChildList.begin(); iterC != iterCEnd; ++iterC )
	{
		CGameObject*	pChild = ( *iterC )->Clone();

		AddChild( pChild );

		SAFE_RELEASE( pChild );
	}

}

CGameObject::~CGameObject()
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ChildList.end();

	for ( iter = m_ChildList.begin(); iter != iterEnd; ++iter )
	{
		CGameObject::EraseObj( *iter );
	}

	Safe_Release_VecList( m_ChildList );
	Safe_Release_VecList( m_FindList );
	SAFE_RELEASE( m_pTransform );
	Safe_Release_VecList( m_ComList );
}

void CGameObject::DontDestroyOnLoad( bool bDontDestroy )
{
	m_bDontDestroy = bDontDestroy;
}

bool CGameObject::IsDontDestroy() const
{
	return m_bDontDestroy;
}

void CGameObject::AddChild( CGameObject * pChild )
{
	pChild->m_pParent = this;
	//pChild->SetScene(m_pScene);
	//pChild->SetLayer(m_pLayer);
	pChild->AddRef();
	pChild->m_pTransform->m_pParent = m_pTransform;

	m_ChildList.push_back( pChild );

	m_pLayer->AddObject( pChild );
}

void CGameObject::DeleteChild( const string & strTag )
{
	list<CGameObject*>::iterator	iterC;
	list<CGameObject*>::iterator	iterCEnd = m_ChildList.end();

	for ( iterC = m_ChildList.begin(); iterC != iterCEnd; ++iterC )
	{
		if ( ( *iterC )->GetTag() == strTag )
		{
			( *iterC )->m_pParent = NULL;
			SAFE_RELEASE( ( *iterC ) );
			m_ChildList.erase( iterC );
			break;
		}
	}
}

// 호적파기
void CGameObject::DeleteChild( CGameObject * pChild )
{
	list<CGameObject*>::iterator	iterC;
	list<CGameObject*>::iterator	iterCEnd = m_ChildList.end();

	for ( iterC = m_ChildList.begin(); iterC != iterCEnd; ++iterC )
	{
		if ( *iterC == pChild )
		{
			pChild->m_pParent = NULL;
			SAFE_RELEASE( ( *iterC ) );
			m_ChildList.erase( iterC );
			break;
		}
	}
}

// 부모버리기
void CGameObject::DeleteParent()
{
	if ( m_pParent )
	{
		m_pParent->DeleteChild( this );
		m_pParent = NULL;
	}
}

CGameObject * CGameObject::GetParent() const
{
	if ( m_pParent )
		m_pParent->AddRef();
	return m_pParent;
}

void CGameObject::ChangeLayer( CLayer * pLayer )
{
	pLayer->AddObject( this );
	m_pLayer->EraseObject( this );

	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ChildList.end();
	for ( iter = m_ChildList.begin(); iter != iterEnd; ++iter )
	{
		( *iter )->ChangeLayer( pLayer );
	}
}

CTransform * CGameObject::GetTransform() const
{
	m_pTransform->AddRef();
	return m_pTransform;
}

void CGameObject::SetScene( CScene * pScene )
{
	m_pScene = pScene;

	if ( m_pTransform )
		m_pTransform->SetScene( pScene );

	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for ( iter = m_ComList.begin(); iter != iterEnd; ++iter )
	{
		( *iter )->SetScene( pScene );
	}

	list<CGameObject*>::iterator	iterC;
	list<CGameObject*>::iterator	iterCEnd = m_ChildList.end();

	for ( iterC = m_ChildList.begin(); iterC != iterCEnd; ++iterC )
	{
		( *iterC )->SetScene( pScene );
	}
}

void CGameObject::SetLayer( CLayer * pLayer )
{
	m_pLayer = pLayer;

	if ( m_pTransform )
		m_pTransform->SetLayer( pLayer );

	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for ( iter = m_ComList.begin(); iter != iterEnd; ++iter )
	{
		( *iter )->SetLayer( pLayer );
	}

	list<CGameObject*>::iterator	iterC;
	list<CGameObject*>::iterator	iterCEnd = m_ChildList.end();

	for ( iterC = m_ChildList.begin(); iterC != iterCEnd; ++iterC )
	{
		( *iterC )->SetLayer( pLayer );
	}
}

bool CGameObject::Init()
{
	m_pTransform = new CTransform;

	if ( !m_pTransform->Init() )
	{
		SAFE_RELEASE( m_pTransform );
		return false;
	}

	m_pTransform->SetGameObject( this );
	m_pTransform->SetScene( m_pScene );
	m_pTransform->SetLayer( m_pLayer );
	m_pTransform->SetTransform( m_pTransform );

	return true;
}

void CGameObject::Input( float fTime )
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for ( iter = m_ComList.begin(); iter != iterEnd;)
	{
		if ( !( *iter )->GetEnable() )
		{
			++iter;
			continue;
		}

		( *iter )->Input( fTime );

		if ( !( *iter )->GetAlive() )
		{
			SAFE_RELEASE( ( *iter ) );
			iter = m_ComList.erase( iter );
		}

		else
			++iter;
	}

	m_pTransform->Input( fTime );
}

int CGameObject::Update( float fTime )
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for ( iter = m_ComList.begin(); iter != iterEnd;)
	{
		if ( !( *iter )->GetEnable() )
		{
			++iter;
			continue;
		}

		( *iter )->Update( fTime );

		if ( !( *iter )->GetAlive() )
		{
			SAFE_RELEASE( ( *iter ) );
			iter = m_ComList.erase( iter );
		}

		else
			++iter;
	}

	m_pTransform->Update( fTime );

	return 0;
}

int CGameObject::LateUpdate( float fTime )
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for ( iter = m_ComList.begin(); iter != iterEnd;)
	{
		if ( !( *iter )->GetEnable() )
		{
			++iter;
			continue;
		}

		( *iter )->LateUpdate( fTime );

		if ( !( *iter )->GetAlive() )
		{
			SAFE_RELEASE( ( *iter ) );
			iter = m_ComList.erase( iter );
		}

		else
			++iter;
	}

	m_pTransform->LateUpdate( fTime );

	return 0;
}

void CGameObject::Collision( float fTime )
{
}

void CGameObject::Render( float fTime )
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for ( iter = m_ComList.begin(); iter != iterEnd;)
	{
		if ( !( *iter )->GetEnable() )
		{
			++iter;
			continue;
		}

		( *iter )->Render( fTime );

		if ( !( *iter )->GetAlive() )
		{
			SAFE_RELEASE( ( *iter ) );
			iter = m_ComList.erase( iter );
		}

		else
			++iter;
	}

	m_pTransform->Render( fTime );
}

CGameObject * CGameObject::Clone()
{
	return new CGameObject( *this );
}

void CGameObject::Save( char * pFileName, const string & strPathKey )
{
	const char*	pPath = GET_SINGLE( CPathManager )->FindPathToMultiByte( strPathKey );

	string	strPath;

	if ( pPath )
		strPath = pPath;

	strPath += pFileName;

	SaveFromFullPath( strPath.c_str() );
}

void CGameObject::Save( FILE * pFile )
{
	// Component 수를 저장한다.
	int	iCount = m_ComList.size();

	fwrite( &iCount, 4, 1, pFile );

	// 트랜스폼 정보를 저장한다.
	m_pTransform->Save( pFile );

	list<class CComponent*>::iterator	iter;
	list<class CComponent*>::iterator	iterEnd = m_ComList.end();

	for ( iter = m_ComList.begin(); iter != iterEnd; ++iter )
	{
		COMPONENT_TYPE eType = ( *iter )->GetComponentType();

		fwrite( &eType, 4, 1, pFile );

		( *iter )->Save( pFile );
	}
}

void CGameObject::SaveFromFullPath( const char * pFullPath )
{
	FILE*	pFile = NULL;

	fopen_s( &pFile, pFullPath, "wb" );

	if ( !pFile )
		return;

	// Component 수를 저장한다.
	int	iCount = m_ComList.size();

	fwrite( &iCount, 4, 1, pFile );

	// 트랜스폼 정보를 저장한다.
	m_pTransform->Save( pFile );

	list<class CComponent*>::iterator	iter;
	list<class CComponent*>::iterator	iterEnd = m_ComList.end();

	for ( iter = m_ComList.begin(); iter != iterEnd; ++iter )
	{
		COMPONENT_TYPE eType = ( *iter )->GetComponentType();

		fwrite( &eType, 4, 1, pFile );

		( *iter )->Save( pFile );
	}

	fclose( pFile );
}

void CGameObject::Load( char * pFileName, const string & strPathKey )
{
	const char*	pPath = GET_SINGLE( CPathManager )->FindPathToMultiByte( strPathKey );

	string	strPath;

	if ( pPath )
		strPath = pPath;

	strPath += pFileName;

	LoadFromFullPath( strPath.c_str() );
}

void CGameObject::Load( FILE * pFile )
{
	Safe_Release_VecList( m_ComList );

	// Component 수를 저장한다.
	int	iCount;

	fread( &iCount, 4, 1, pFile );

	// 트랜스폼 정보를 저장한다.
	m_pTransform->Load( pFile );

	for ( int i = 0; i < iCount; ++i )
	{
		COMPONENT_TYPE eType;
		fread( &eType, 4, 1, pFile );

		CComponent*	pCom = NULL;

		switch ( eType )
		{
		case CT_RENDERER2D:
			pCom = new CRenderer2D;
			break;
		case CT_RENDERER:
			break;
		case CT_SCRIPT:
			break;
		case CT_CAMERA:
			pCom = new CCamera;
			break;
		case CT_MATERIAL:
			break;
		case CT_ANIMATION2D:
			pCom = new CAnimation2D;
			break;
		case CT_COLLIDER:
			//pCom = new CRenderer2D;
			break;
		case CT_TERRAIN2D:
			pCom = new CTerrain2D;
			break;
		case CT_TILE:
			pCom = new CTile;
			break;
		}

		AddComponent( pCom );
		pCom->Load( pFile );
		SAFE_RELEASE( pCom );
	}
}

void CGameObject::LoadFromFullPath( const char * pFullPath )
{
	FILE*	pFile = NULL;

	fopen_s( &pFile, pFullPath, "rb" );

	if ( !pFile )
		return;

	Safe_Release_VecList( m_ComList );

	// Component 수를 저장한다.
	int	iCount;

	fread( &iCount, 4, 1, pFile );

	// 트랜스폼 정보를 저장한다.
	m_pTransform->Load( pFile );

	for ( int i = 0; i < iCount; ++i )
	{
		COMPONENT_TYPE eType;
		fread( &eType, 4, 1, pFile );

		CComponent*	pCom = NULL;

		switch ( eType )
		{
		case CT_RENDERER2D:
			pCom = new CRenderer2D;
			break;
		case CT_RENDERER:
			break;
		case CT_SCRIPT:
			break;
		case CT_CAMERA:
			pCom = new CCamera;
			break;
		case CT_MATERIAL:
			break;
		case CT_ANIMATION2D:
			pCom = new CAnimation2D;
			break;
		case CT_COLLIDER:
			//pCom = new CRenderer2D;
			break;
		case CT_TERRAIN2D:
			pCom = new CTerrain2D;
			break;
		case CT_TILE:
			pCom = new CTile;
			break;
		}

		pCom->SetTransform( m_pTransform );
		pCom->SetGameObject( this );
		pCom->Init();
		AddComponent( pCom );
		pCom->Load( pFile );

		//m_ComList.push_back(pCom);
		SAFE_RELEASE( pCom );
	}

	fclose( pFile );
}

void CGameObject::OnCollisionEnter( CCollider * pSrc,
	CCollider * pDest, float fTime )
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for ( iter = m_ComList.begin(); iter != iterEnd; ++iter )
	{
		( *iter )->OnCollisionEnter( pSrc, pDest, fTime );
	}
}

void CGameObject::OnCollision( CCollider * pSrc,
	CCollider * pDest, float fTime )
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for ( iter = m_ComList.begin(); iter != iterEnd; ++iter )
	{
		( *iter )->OnCollision( pSrc, pDest, fTime );
	}
}

void CGameObject::OnCollisionLeave( CCollider * pSrc,
	CCollider * pDest, float fTime )
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for ( iter = m_ComList.begin(); iter != iterEnd; ++iter )
	{
		( *iter )->OnCollisionLeave( pSrc, pDest, fTime );
	}
}

void CGameObject::UpdateTransformHierarchy()
{
	list<CGameObject*>::iterator	iterC;
	list<CGameObject*>::iterator	iterCEnd = m_ChildList.end();

	for ( iterC = m_ChildList.begin(); iterC != iterCEnd; ++iterC )
	{
		( *iterC )->m_pTransform->m_bUpdate = true;
		( *iterC )->UpdateTransformHierarchy();
	}
}

const list<CComponent*>* CGameObject::FindComponentsFromTag(
	const string & strTag )
{
	Safe_Release_VecList( m_FindList );
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for ( iter = m_ComList.begin(); iter != iterEnd; ++iter )
	{
		if ( ( *iter )->GetTag() == strTag )
		{
			( *iter )->AddRef();
			m_FindList.push_back( *iter );
		}
	}

	return &m_FindList;
}

const list<CComponent*>* CGameObject::FindComponentsFromTypeName( const string & strTypeName )
{
	Safe_Release_VecList( m_FindList );
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for ( iter = m_ComList.begin(); iter != iterEnd; ++iter )
	{
		if ( ( *iter )->GetTypeName() == strTypeName )
		{
			( *iter )->AddRef();
			m_FindList.push_back( *iter );
		}
	}

	return &m_FindList;
}

const list<CComponent*>* CGameObject::FindComponentsFromType( COMPONENT_TYPE eType )
{
	Safe_Release_VecList( m_FindList );
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for ( iter = m_ComList.begin(); iter != iterEnd; ++iter )
	{
		if ( ( *iter )->GetComponentType() == eType )
		{
			( *iter )->AddRef();
			m_FindList.push_back( *iter );
		}
	}

	return &m_FindList;
}

bool CGameObject::CheckComponentFromTag( const string & strTag )
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for ( iter = m_ComList.begin(); iter != iterEnd; ++iter )
	{
		if ( ( *iter )->GetTag() == strTag )
		{
			return true;
		}
	}

	return false;
}

bool CGameObject::CheckComponentFromTypeName( const string & strTypeName )
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for ( iter = m_ComList.begin(); iter != iterEnd; ++iter )
	{
		if ( ( *iter )->GetTypeName() == strTypeName )
		{
			return true;
		}
	}

	return false;
}

bool CGameObject::CheckComponentFromType( COMPONENT_TYPE eType )
{
	list<CComponent*>::iterator	iter;
	list<CComponent*>::iterator	iterEnd = m_ComList.end();

	for ( iter = m_ComList.begin(); iter != iterEnd; ++iter )
	{
		if ( ( *iter )->GetComponentType() == eType )
		{
			return true;
		}
	}

	return false;
}

CComponent * CGameObject::AddComponent( CComponent * pComponent )
{
	pComponent->SetScene( m_pScene );
	pComponent->SetLayer( m_pLayer );
	pComponent->SetGameObject( this );
	pComponent->SetTransform( m_pTransform );
	pComponent->AddRef();

	m_ComList.push_back( pComponent );

	return pComponent;
}
