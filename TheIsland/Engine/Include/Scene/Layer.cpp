#include "Layer.h"
#include "../GameObject/GameObject.h"

GAME_USING

CLayer::CLayer() :
	m_iZOrder( 0 )
{
	SetTag( "Layer" );
	SetTypeName( "CLayer" );
	SetTypeID<CLayer>();
}

CLayer::~CLayer()
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for ( iter = m_ObjList.begin(); iter != iterEnd; ++iter )
	{
		CGameObject::EraseObj( *iter );
		SAFE_RELEASE( ( *iter ) );
	}

	m_ObjList.clear();
}

void CLayer::SetZOrder( int iZOrder )
{
	m_iZOrder = iZOrder;
}

void CLayer::SetScene( CScene * pScene )
{
	m_pScene = pScene;
}

CScene * CLayer::GetScene() const
{
	return m_pScene;
}

int CLayer::GetZOrder() const
{
	return m_iZOrder;
}

bool CLayer::Init()
{
	return true;
}

void CLayer::Input( float fTime )
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for ( iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if ( !( *iter )->GetEnable() )
		{
			++iter;
			continue;
		}

		( *iter )->Input( fTime );

		if ( !( *iter )->GetAlive() )
		{
			CGameObject::EraseObj( *iter );
			SAFE_RELEASE( ( *iter ) );
			iter = m_ObjList.erase( iter );
		}

		else
			++iter;
	}
}

int CLayer::Update( float fTime )
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for ( iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if ( !( *iter )->GetEnable() )
		{
			++iter;
			continue;
		}

		( *iter )->Update( fTime );

		if ( !( *iter )->GetAlive() )
		{
			CGameObject::EraseObj( *iter );
			SAFE_RELEASE( ( *iter ) );
			iter = m_ObjList.erase( iter );
		}

		else
			++iter;
	}

	return 0;
}

int CLayer::LateUpdate( float fTime )
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for ( iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if ( !( *iter )->GetEnable() )
		{
			++iter;
			continue;
		}

		( *iter )->LateUpdate( fTime );

		if ( !( *iter )->GetAlive() )
		{
			CGameObject::EraseObj( *iter );
			SAFE_RELEASE( ( *iter ) );
			iter = m_ObjList.erase( iter );
		}

		else
			++iter;
	}

	return 0;
}

void CLayer::Collision( float fTime )
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for ( iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if ( !( *iter )->GetEnable() )
		{
			++iter;
			continue;
		}

		( *iter )->Collision( fTime );

		if ( !( *iter )->GetAlive() )
		{
			CGameObject::EraseObj( *iter );
			SAFE_RELEASE( ( *iter ) );
			iter = m_ObjList.erase( iter );
		}

		else
			++iter;
	}
}

void CLayer::Render( float fTime )
{
	list<CGameObject*>::iterator	iter;
	list<CGameObject*>::iterator	iterEnd = m_ObjList.end();

	for ( iter = m_ObjList.begin(); iter != iterEnd;)
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
			iter = m_ObjList.erase( iter );
		}

		else
			++iter;
	}
}

void CLayer::AddObject( CGameObject * pObj )
{
	pObj->SetScene( m_pScene );
	pObj->SetLayer( this );
	pObj->AddRef();

	m_ObjList.push_back( pObj );
}

void CLayer::EraseObject( CGameObject * pObj )
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

void CLayer::EraseObject( const string & strTag )
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

CGameObject * CLayer::GetObj( const string & strTag )
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

CGameObject * CLayer::GetObj( int idx )
{
	if ( m_ObjList.size() <= idx )
		return NULL;

	list<CGameObject*>::iterator	iter = m_ObjList.begin();

	for ( int i = 0; i < idx; ++i )
	{
		++iter;
	}

	( *iter )->AddRef();

	return *iter;
}
