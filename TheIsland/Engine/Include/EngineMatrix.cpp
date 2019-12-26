#include "EngineMatrix.h"

GAME_USING

_tagMatrix::_tagMatrix()
{
	// XMMatrixIdentity : 단위행렬로 만들어주는 함수
	mat = XMMatrixIdentity();
}

_tagMatrix::_tagMatrix( const XMMATRIX & _m )
{
	mat = _m;
}

_tagMatrix::_tagMatrix( const _tagMatrix & _m )
{
	mat = _m.mat;
}

_tagMatrix::_tagMatrix( float _m[4][4] )
{
	memcpy( m, _m, sizeof( float ) * 4 * 4 );
}

_tagMatrix::_tagMatrix( Vector4 _m[4] )
{
	memcpy( v, _m, sizeof( Vector4 ) * 4 );
}

void _tagMatrix::operator=( const _tagMatrix & _m )
{
	mat = _m.mat;
}

void _tagMatrix::operator=( const XMMATRIX & _m )
{
	mat = _m;
}

void _tagMatrix::operator=( float _m[4][4] )
{
	memcpy( m, _m, sizeof( float ) * 4 * 4 );
}

void _tagMatrix::operator=( Vector4 _m[4] )
{
	memcpy( v, _m, sizeof( Vector4 ) * 4 );
}

void * _tagMatrix::operator new( size_t size )
{
	return _aligned_malloc( size, 16 );
}

void _tagMatrix::operator delete( void * pAddr )
{
	_aligned_free( pAddr );
}

_tagMatrix::operator _tagMatrix()
{
	return mat;
}

_tagMatrix _tagMatrix::operator*( const _tagMatrix & _m )
{
	_tagMatrix	matrix;
	matrix.mat = mat * _m.mat;
	return matrix;
}

_tagMatrix _tagMatrix::operator*( const XMMATRIX & _m )
{
	_tagMatrix	matrix;
	matrix.mat = mat * _m;
	return matrix;
}

void _tagMatrix::operator*=( const XMMATRIX & _m )
{
	mat *= _m;
}

void _tagMatrix::operator*=( const _tagMatrix & _m )
{
	mat *= _m.mat;
}
