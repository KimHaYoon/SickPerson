#pragma once
#include "../Component/Script.h"

class CPyramid : public CScript
{
public:
	CPyramid();
	CPyramid( const CPyramid& pPyramid );
	~CPyramid();

	// CScript을(를) 통해 상속됨
	virtual bool Init();
	virtual void Input( float fTime );
	virtual int Update( float fTime );
	virtual int LateUpdate( float fTime );
	virtual void Collision( float fTime );
	virtual void Render( float fTime );
	virtual CPyramid * Clone();
};

