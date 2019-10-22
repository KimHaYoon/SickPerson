#pragma once
#include "../Component/Script.h"

class CPyramid : public CScript
{
public:
	CPyramid();
	CPyramid( const CPyramid& pPyramid );
	~CPyramid();

	// CScript��(��) ���� ��ӵ�
	virtual bool Init();
	virtual void Input( float fTime );
	virtual int Update( float fTime );
	virtual int LateUpdate( float fTime );
	virtual void Collision( float fTime );
	virtual void Render( float fTime );
	virtual CPyramid * Clone();
};

