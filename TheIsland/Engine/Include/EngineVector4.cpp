#include "EngineVector4.h"


GAME_USING

_tagVector4 _tagVector4::Red = _tagVector4( 1.f, 0.f, 0.f, 1.f );
_tagVector4 _tagVector4::Blue = _tagVector4( 0.f, 0.f, 1.f, 1.f );
_tagVector4 _tagVector4::Green = _tagVector4( 0.f, 1.f, 0.f, 1.f );
_tagVector4 _tagVector4::Yellow = _tagVector4( 1.f, 1.f, 0.f, 1.f );
_tagVector4 _tagVector4::Magenta = _tagVector4( 1.f, 0.f, 1.f, 1.f );
_tagVector4 _tagVector4::White = _tagVector4( 1.f, 1.f, 1.f, 1.f );
_tagVector4 _tagVector4::Black = _tagVector4( 0.f, 0.f, 0.f, 1.f );

_tagVector4::_tagVector4()
{
	x = y = z = w = 0.f;
}

_tagVector4::_tagVector4( const _tagVector4& v )
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}

_tagVector4::_tagVector4( float _x, float _y, float _z, float _w )
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

_tagVector4::_tagVector4( int _x, int _y, int _z, int _w )
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

_tagVector4::_tagVector4( float f[4] )
{
	x = f[0];
	y = f[1];
	z = f[2];
	w = f[3];
}

_tagVector4::_tagVector4( int i[4] )
{
	x = i[0];
	y = i[1];
	z = i[2];
	w = i[3];
}

void _tagVector4::operator =( const _tagVector4& v )
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}

void _tagVector4::operator =( const XMFLOAT3& v )
{
	x = v.x;
	y = v.y;
	z = v.z;
}

void _tagVector4::operator =( const XMFLOAT4& v )
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}

void _tagVector4::operator =( const XMVECTOR& v )
{
	XMStoreFloat4( this, v );
}

void _tagVector4::operator =( float f[4] )
{
	x = f[0];
	y = f[1];
	z = f[2];
	w = f[3];
}

void _tagVector4::operator =( int i[4] )
{
	x = i[0];
	y = i[1];
	z = i[2];
	w = i[3];
}

// =============== +++++++++++++++++++ =======================
_tagVector4 _tagVector4::operator +( const _tagVector4& v )	const
{
	XMVECTOR	v1 = Convert();
	XMVECTOR	v2 = v.Convert();
	_tagVector4	v3;
	XMStoreFloat4( &v3, v1 + v2 );
	return v3;
}

_tagVector4 _tagVector4::operator +( const XMVECTOR& v2 )	const
{
	_tagVector4	v1;
	_tagVector4	v;
	XMStoreFloat4( &v, v2 );
	v1.x = x + v.x;
	v1.y = y + v.y;
	v1.z = z + v.z;
	v1.w = w + v.w;
	return v1;
}

_tagVector4 _tagVector4::operator +( float f )	const
{
	_tagVector4	v1;
	v1.x = x + f;
	v1.y = y + f;
	v1.z = z + f;
	v1.w = w + f;
	return v1;
}

_tagVector4 _tagVector4::operator +( int i )	const
{
	_tagVector4	v1;
	v1.x = x + i;
	v1.y = y + i;
	v1.z = z + i;
	v1.w = w + i;
	return v1;
}

_tagVector4 _tagVector4::operator +( float f[4] )	const
{
	_tagVector4	v1;
	v1.x = x + f[0];
	v1.y = y + f[1];
	v1.z = z + f[2];
	v1.w = w + f[3];
	return v1;
}

_tagVector4 _tagVector4::operator +( int i[4] )	const
{
	_tagVector4	v1;
	v1.x = x + i[0];
	v1.y = y + i[1];
	v1.z = z + i[2];
	v1.w = w + i[3];
	return v1;
}

// ============== +=    ==============================
void _tagVector4::operator +=( const _tagVector4& v )
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}

void _tagVector4::operator +=( const XMVECTOR& v2 )
{
	_tagVector4	v;
	XMStoreFloat4( &v, v2 );
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}

void _tagVector4::operator +=( float f )
{
	x += f;
	y += f;
	z += f;
	w += f;
}

void _tagVector4::operator +=( int i )
{
	x += i;
	y += i;
	z += i;
	w += i;
}

void _tagVector4::operator +=( float f[4] )
{
	x += f[0];
	y += f[1];
	z += f[2];
	w += f[3];
}

void _tagVector4::operator +=( int i[4] )
{
	x += i[0];
	y += i[1];
	z += i[2];
	w += i[3];
}

// =============== --------------- =======================
_tagVector4 _tagVector4::operator -( const _tagVector4& v )	const
{
	_tagVector4	v1;
	v1.x = x - v.x;
	v1.y = y - v.y;
	v1.z = z - v.z;
	v1.w = w - v.w;
	return v1;
}

_tagVector4 _tagVector4::operator -( const XMVECTOR& v2 )	const
{
	_tagVector4	v1;
	_tagVector4	v;
	XMStoreFloat4( &v, v2 );
	v1.x = x - v.x;
	v1.y = y - v.y;
	v1.z = z - v.z;
	v1.w = w - v.w;
	return v1;
}

_tagVector4 _tagVector4::operator -( float f )	const
{
	_tagVector4	v1;
	v1.x = x - f;
	v1.y = y - f;
	v1.z = z - f;
	v1.w = w - f;
	return v1;
}

_tagVector4 _tagVector4::operator -( int i )	const
{
	_tagVector4	v1;
	v1.x = x - i;
	v1.y = y - i;
	v1.z = z - i;
	v1.w = w - i;
	return v1;
}

_tagVector4 _tagVector4::operator -( float f[4] )	const
{
	_tagVector4	v1;
	v1.x = x - f[0];
	v1.y = y - f[1];
	v1.z = z - f[2];
	v1.w = w - f[3];
	return v1;
}

_tagVector4 _tagVector4::operator -( int i[4] )	const
{
	_tagVector4	v1;
	v1.x = x - i[0];
	v1.y = y - i[1];
	v1.z = z - i[2];
	v1.w = w - i[3];
	return v1;
}

// ============== -=    ==============================
void _tagVector4::operator -=( const _tagVector4& v )
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}

void _tagVector4::operator -=( const XMVECTOR& v2 )
{
	_tagVector4	v;
	XMStoreFloat4( &v, v2 );
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}

void _tagVector4::operator -=( float f )
{
	x -= f;
	y -= f;
	z -= f;
	w -= f;
}

void _tagVector4::operator -=( int i )
{
	x -= i;
	y -= i;
	z -= i;
	w -= i;
}

void _tagVector4::operator -=( float f[4] )
{
	x -= f[0];
	y -= f[1];
	z -= f[2];
	w -= f[3];
}

void _tagVector4::operator -=( int i[4] )
{
	x -= i[0];
	y -= i[1];
	z -= i[2];
	w -= i[3];
}

// =============== *************** =======================
_tagVector4 _tagVector4::operator *( const _tagVector4& v )	const
{
	_tagVector4	v1;
	v1.x = x * v.x;
	v1.y = y * v.y;
	v1.z = z * v.z;
	v1.w = w * v.w;
	return v1;
}

_tagVector4 _tagVector4::operator *( const XMVECTOR& v2 )	const
{
	_tagVector4	v1;
	_tagVector4	v;
	XMStoreFloat4( &v, v2 );
	v1.x = x * v.x;
	v1.y = y * v.y;
	v1.z = z * v.z;
	v1.w = w * v.w;
	return v1;
}

_tagVector4 _tagVector4::operator *( float f )	const
{
	_tagVector4	v1;
	v1.x = x * f;
	v1.y = y * f;
	v1.z = z * f;
	v1.w = w * f;
	return v1;
}

_tagVector4 _tagVector4::operator *( int i )	const
{
	_tagVector4	v1;
	v1.x = x * i;
	v1.y = y * i;
	v1.z = z * i;
	v1.w = w * i;
	return v1;
}

_tagVector4 _tagVector4::operator *( float f[4] )	const
{
	_tagVector4	v1;
	v1.x = x * f[0];
	v1.y = y * f[1];
	v1.z = z * f[2];
	v1.w = w * f[3];
	return v1;
}

_tagVector4 _tagVector4::operator *( int i[4] )	const
{
	_tagVector4	v1;
	v1.x = x * i[0];
	v1.y = y * i[1];
	v1.z = z * i[2];
	v1.w = w * i[3];
	return v1;
}

// ============== *=    ==============================
void _tagVector4::operator *=( const _tagVector4& v )
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
}

void _tagVector4::operator *=( const XMVECTOR& v2 )
{
	_tagVector4	v;
	XMStoreFloat4( &v, v2 );
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
}

void _tagVector4::operator *=( float f )
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
}

void _tagVector4::operator *=( int i )
{
	x *= i;
	y *= i;
	z *= i;
	w *= i;
}

void _tagVector4::operator *=( float f[4] )
{
	x *= f[0];
	y *= f[1];
	z *= f[2];
	w *= f[3];
}

void _tagVector4::operator *=( int i[4] )
{
	x *= i[0];
	y *= i[1];
	z *= i[2];
	w *= i[3];
}

// =============== /////////////// =======================
_tagVector4 _tagVector4::operator /( const _tagVector4& v )	const
{
	_tagVector4	v1;
	v1.x = x / v.x;
	v1.y = y / v.y;
	v1.z = z / v.z;
	v1.w = w / v.w;
	return v1;
}

_tagVector4 _tagVector4::operator /( const XMVECTOR& v )	const
{
	XMVECTOR	v1, v2;

	v1 = XMLoadFloat4( this );
	v2 = v1 / v;

	_tagVector4	v3;
	XMStoreFloat4( &v3, v2 );

	return v3;
}

_tagVector4 _tagVector4::operator /( float f )	const
{
	_tagVector4	v1;
	v1.x = x / f;
	v1.y = y / f;
	v1.z = z / f;
	v1.w = w / f;
	return v1;
}

_tagVector4 _tagVector4::operator /( int i )	const
{
	_tagVector4	v1;
	v1.x = x / i;
	v1.y = y / i;
	v1.z = z / i;
	v1.w = w / i;
	return v1;
}

_tagVector4 _tagVector4::operator /( float f[4] )	const
{
	_tagVector4	v1;
	v1.x = x / f[0];
	v1.y = y / f[1];
	v1.z = z / f[2];
	v1.w = w / f[3];
	return v1;
}

_tagVector4 _tagVector4::operator /( int i[4] )	const
{
	_tagVector4	v1;
	v1.x = x / i[0];
	v1.y = y / i[1];
	v1.z = z / i[2];
	v1.w = w / i[3];
	return v1;
}

// ============== /=    ==============================
void _tagVector4::operator /=( const _tagVector4& v )
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
}

void _tagVector4::operator /=( const XMVECTOR& v2 )
{
	_tagVector4	v;
	XMStoreFloat4( &v, v2 );
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
}

void _tagVector4::operator /=( float f )
{
	x /= f;
	y /= f;
	z /= f;
	w /= f;
}

void _tagVector4::operator /=( int i )
{
	x /= i;
	y /= i;
	z /= i;
	w /= i;
}

void _tagVector4::operator /=( float f[4] )
{
	x /= f[0];
	y /= f[1];
	z /= f[2];
	w /= f[3];
}

void _tagVector4::operator /=( int i[4] )
{
	x /= i[0];
	y /= i[1];
	z /= i[2];
	w /= i[3];
}

bool _tagVector4::operator ==( const _tagVector4& v )	const
{
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

bool _tagVector4::operator ==( const XMVECTOR& v )	const
{
	_tagVector4	v1;
	XMStoreFloat4( &v1, v );
	return x == v1.x && y == v1.y && z == v1.z && w == v1.w;
}

bool _tagVector4::operator ==( float f[4] )	const
{
	return x == f[0] && y == f[1] && z == f[2] && w == f[3];
}

bool _tagVector4::operator ==( int i[4] )	const
{
	return x == i[0] && y == i[1] && z == i[2] && w == i[3];
}

bool _tagVector4::operator !=( const _tagVector4& v )	const
{
	return x != v.x || y != v.y || z != v.z || w != v.w;
}

bool _tagVector4::operator !=( const XMVECTOR& v )	const
{
	_tagVector4	v1;
	XMStoreFloat4( &v1, v );
	return x != v1.x || y != v1.y || z != v1.z || w != v1.w;
}

bool _tagVector4::operator !=( float f[4] )	const
{
	return x != f[0] || y != f[1] || z != f[2] || w != f[3];
}

bool _tagVector4::operator !=( int i[4] )	const
{
	return x != i[0] || y != i[1] || z != i[2] || w != i[3];
}

XMVECTOR _tagVector4::Convert()	const
{
	return XMLoadFloat4( this );
}

float _tagVector4::Length()	const
{
	_tagVector4 v;
	XMStoreFloat4( &v, XMVector4Length( Convert() ) );
	return v.x;
}

_tagVector4 _tagVector4::Normalize()	const
{
	XMVECTOR	v = Convert();
	v = XMVector4Normalize( v );
	_tagVector4	v1;
	XMStoreFloat4( &v1, v );
	return v1;
}
