#include "Obj.h"

GAME_USING

CObj::CObj() :
	m_iRefCount( 1 ),
	m_bEnable( true ),
	m_bAlive( true )
{
}

CObj::~CObj()
{
}

string CObj::GetTag() const
{
	return m_strTag;
}

string CObj::GetTypeName() const
{
	return m_strTypeName;
}

const type_info * CObj::GetTypeID() const
{
	return m_pTypeID;
}

bool CObj::GetEnable() const
{
	return m_bEnable;
}

bool CObj::GetAlive() const
{
	return m_bAlive;
}

void CObj::SetTag( const string & strTag )
{
	m_strTag = strTag;
}

void CObj::SetTypeName( const string & strTypeName )
{
	m_strTypeName = strTypeName;
}

void CObj::Enable( bool bEnable )
{
	m_bEnable = bEnable;
}

void CObj::Die()
{
	m_bAlive = false;
}

int CObj::Release()
{
	--m_iRefCount;

	if ( m_iRefCount == 0 )
	{
		delete	this;
		return 0;
	}

	return m_iRefCount;
}

void CObj::AddRef()
{
	++m_iRefCount;
}
