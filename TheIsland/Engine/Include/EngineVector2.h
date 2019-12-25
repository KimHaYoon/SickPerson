#pragma once

#include "EngineMath.h"
#include "Flag.h"


GAME_BEGIN

typedef struct GAME_DLL _tagVector2 :
	public XMFLOAT2
{
	_tagVector2();

	_tagVector2( const _tagVector2& v );

	_tagVector2( float _x, float _y );

	_tagVector2( int _x, int _y );

	_tagVector2( float f[2] );

	_tagVector2( int i[2] );

	void operator =( const _tagVector2& v );

	void operator =( const XMFLOAT2& v );

	void operator =( const XMFLOAT4& v );

	void operator =( const XMVECTOR& v );

	void operator =( float f[2] );

	void operator =( int i[2] );

	// =============== +++++++++++++++++++ =======================
	_tagVector2 operator +( const _tagVector2& v )	const;

	_tagVector2 operator +( const XMVECTOR& v2 )	const;

	_tagVector2 operator +( float f )	const;

	_tagVector2 operator +( int i )	const;

	_tagVector2 operator +( float f[2] )	const;

	_tagVector2 operator +( int i[2] )	const;

	// ============== +=    ==============================
	void operator +=( const _tagVector2& v );

	void operator +=( const XMVECTOR& v2 );

	void operator +=( float f );

	void operator +=( int i );

	void operator +=( float f[2] );

	void operator +=( int i[2] );

	// =============== --------------- =======================
	_tagVector2 operator -( const _tagVector2& v )	const;

	_tagVector2 operator -( const XMVECTOR& v2 )	const;

	_tagVector2 operator -( float f )	const;

	_tagVector2 operator -( int i )	const;

	_tagVector2 operator -( float f[2] )	const;

	_tagVector2 operator -( int i[2] )	const;

	// ============== -=    ==============================
	void operator -=( const _tagVector2& v );

	void operator -=( const XMVECTOR& v2 );

	void operator -=( float f );

	void operator -=( int i );

	void operator -=( float f[2] );

	void operator -=( int i[2] );

	// =============== *************** =======================
	_tagVector2 operator *( const _tagVector2& v )	const;

	_tagVector2 operator *( const XMVECTOR& v2 )	const;

	_tagVector2 operator *( float f )	const;

	_tagVector2 operator *( int i )	const;

	_tagVector2 operator *( float f[2] )	const;

	_tagVector2 operator *( int i[2] )	const;

	// ============== *=    ==============================
	void operator *=( const _tagVector2& v );

	void operator *=( const XMVECTOR& v2 );

	void operator *=( float f );

	void operator *=( int i );

	void operator *=( float f[2] );

	void operator *=( int i[2] );

	// =============== /////////////// =======================
	_tagVector2 operator /( const _tagVector2& v )	const;

	_tagVector2 operator /( const XMVECTOR& v )	const;

	_tagVector2 operator /( float f )	const;

	_tagVector2 operator /( int i )	const;

	_tagVector2 operator /( float f[2] )	const;

	_tagVector2 operator /( int i[2] )	const;

	// ============== /=    ==============================
	void operator /=( const _tagVector2& v );

	void operator /=( const XMVECTOR& v2 );

	void operator /=( float f );

	void operator /=( int i );

	void operator /=( float f[2] );

	void operator /=( int i[2] );

	bool operator ==( const _tagVector2& v )	const;

	bool operator ==( const XMVECTOR& v )	const;

	bool operator ==( float f[2] )	const;

	bool operator ==( int i[2] )	const;

	bool operator !=( const _tagVector2& v )	const;

	bool operator !=( const XMVECTOR& v )	const;

	bool operator !=( float f[2] )	const;

	bool operator !=( int i[2] )	const;

	XMVECTOR Convert()	const;

	float Length()	const;

	_tagVector2 Normalize()	const;

	static _tagVector2	One;
	static _tagVector2	Zero;
}Vector2, *PVector2;

GAME_END