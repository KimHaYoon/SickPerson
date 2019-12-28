#pragma once

#include "EngineMath.h"

GAME_BEGIN

typedef struct GAME_DLL _tagVector4 :
	public XMFLOAT4
{
	_tagVector4();

	_tagVector4( const _tagVector4& v );
	_tagVector4( const XMVECTOR& v );

	_tagVector4( float _x, float _y, float _z, float _w );

	_tagVector4( int _x, int _y, int _z, int _w );

	_tagVector4( float f[4] );

	_tagVector4( int i[4] );

	void operator =( const _tagVector4& v );

	void operator =( const XMFLOAT3& v );

	void operator =( const XMFLOAT4& v );

	void operator =( const XMVECTOR& v );

	void operator =( float f[4] );

	void operator =( int i[4] );

	// =============== +++++++++++++++++++ =======================
	_tagVector4 operator +( const _tagVector4& v )	const;

	_tagVector4 operator +( const XMVECTOR& v2 )	const;

	_tagVector4 operator +( float f )	const;

	_tagVector4 operator +( int i )	const;

	_tagVector4 operator +( float f[4] )	const;

	_tagVector4 operator +( int i[4] )	const;

	// ============== +=    ==============================
	void operator +=( const _tagVector4& v );

	void operator +=( const XMVECTOR& v2 );

	void operator +=( float f );

	void operator +=( int i );

	void operator +=( float f[4] );

	void operator +=( int i[4] );

	// =============== --------------- =======================
	_tagVector4 operator -( const _tagVector4& v )	const;

	_tagVector4 operator -( const XMVECTOR& v2 )	const;

	_tagVector4 operator -( float f )	const;

	_tagVector4 operator -( int i )	const;

	_tagVector4 operator -( float f[4] )	const;

	_tagVector4 operator -( int i[4] )	const;

	// ============== -=    ==============================
	void operator -=( const _tagVector4& v );

	void operator -=( const XMVECTOR& v2 );

	void operator -=( float f );

	void operator -=( int i );

	void operator -=( float f[4] );

	void operator -=( int i[4] );

	// =============== *************** =======================
	_tagVector4 operator *( const _tagVector4& v )	const;

	_tagVector4 operator *( const XMVECTOR& v2 )	const;

	_tagVector4 operator *( float f )	const;

	_tagVector4 operator *( int i )	const;

	_tagVector4 operator *( float f[4] )	const;

	_tagVector4 operator *( int i[4] )	const;

	// ============== *=    ==============================
	void operator *=( const _tagVector4& v );

	void operator *=( const XMVECTOR& v2 );

	void operator *=( float f );

	void operator *=( int i );

	void operator *=( float f[4] );

	void operator *=( int i[4] );

	// =============== /////////////// =======================
	_tagVector4 operator /( const _tagVector4& v )	const;

	_tagVector4 operator /( const XMVECTOR& v )	const;

	_tagVector4 operator /( float f )	const;

	_tagVector4 operator /( int i )	const;

	_tagVector4 operator /( float f[4] )	const;

	_tagVector4 operator /( int i[4] )	const;

	// ============== /=    ==============================
	void operator /=( const _tagVector4& v );

	void operator /=( const XMVECTOR& v2 );

	void operator /=( float f );

	void operator /=( int i );

	void operator /=( float f[4] );

	void operator /=( int i[4] );

	bool operator ==( const _tagVector4& v )	const;

	bool operator ==( const XMVECTOR& v )	const;

	bool operator ==( float f[4] )	const;

	bool operator ==( int i[4] )	const;

	bool operator !=( const _tagVector4& v )	const;

	bool operator !=( const XMVECTOR& v )	const;

	bool operator !=( float f[4] )	const;

	bool operator !=( int i[4] )	const;

	float operator []( int idx )	const;

	XMVECTOR Convert()	const;
	float Length()	const;
	_tagVector4 Normalize()	const;

	static _tagVector4 Red;
	static _tagVector4 Blue;
	static _tagVector4 Green;
	static _tagVector4 Yellow;
	static _tagVector4 Magenta;
	static _tagVector4 White;
	static _tagVector4 Black;
	static _tagVector4 Zero;
}Vector4, *PVector4;

GAME_END