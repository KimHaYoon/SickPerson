#pragma once

#include "EngineVector4.h"

GAME_BEGIN

typedef union GAME_DLL _tagMatrix
{
	XMMATRIX	mat;
	float	m[4][4];
	Vector4	v[4];
	struct
	{
		float	_11, _12, _13, _14;
		float	_21, _22, _23, _24;
		float	_31, _32, _33, _34;
		float	_41, _42, _43, _44;
	};

	_tagMatrix();
	_tagMatrix( const XMMATRIX& _m );
	_tagMatrix( const _tagMatrix& _m );
	_tagMatrix( float _m[4][4] );
	_tagMatrix( Vector4 _m[4] );

	void operator =( const _tagMatrix& _m );
	void operator =( const XMMATRIX& _m );
	void operator =( float _m[4][4] );
	void operator =( Vector4 _m[4] );

	void* operator new( size_t size );
	void operator delete( void* pAddr );
	operator _tagMatrix ();
	_tagMatrix operator *( const _tagMatrix& _m )	const;
	_tagMatrix operator *( const XMMATRIX& _m )	const;
	void operator *=( const XMMATRIX& _m );
	void operator *=( const _tagMatrix& _m );

	_tagMatrix Transpose()	const;

}Matrix, *PMatrix;

GAME_END