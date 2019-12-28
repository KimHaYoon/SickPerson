#include "EngineVector3.h"
#include "EngineMatrix.h"

GAME_USING

_tagVector3 _tagVector3::Zero = _tagVector3( 0.f, 0.f, 0.f );
_tagVector3 _tagVector3::One = _tagVector3( 1.f, 1.f, 1.f );
_tagVector3 _tagVector3::Axis[AXIS_END] =
{
	_tagVector3( 1.f, 0.f, 0.f ),
	_tagVector3( 0.f, 1.f, 0.f ),
	_tagVector3( 0.f, 0.f, 1.f )
};
_tagVector3 _tagVector3::Axis2D[AXIS_END] =
{
	_tagVector3( 1.f, 0.f, 0.f ),
	_tagVector3( 0.f, -1.f, 0.f ),
	_tagVector3( 0.f, 0.f, 1.f )
};

_tagVector3::_tagVector3()
{
	x = y = z = 0.f;
}

_tagVector3::_tagVector3( const _tagVector3& v )
{
	x = v.x;
	y = v.y;
	z = v.z;
}

_tagVector3::_tagVector3( float _x, float _y, float _z )
{
	x = _x;
	y = _y;
	z = _z;
}

_tagVector3::_tagVector3( int _x, int _y, int _z )
{
	x = _x;
	y = _y;
	z = _z;
}

_tagVector3::_tagVector3( float f[3] )
{
	x = f[0];
	y = f[1];
	z = f[2];
}

_tagVector3::_tagVector3( int i[3] )
{
	x = i[0];
	y = i[1];
	z = i[2];
}

void _tagVector3::operator =( const _tagVector3& v )
{
	x = v.x;
	y = v.y;
	z = v.z;
}

void _tagVector3::operator =( const XMFLOAT3& v )
{
	x = v.x;
	y = v.y;
	z = v.z;
}

void _tagVector3::operator =( const XMFLOAT4& v )
{
	x = v.x;
	y = v.y;
	z = v.z;
}

void _tagVector3::operator =( const XMVECTOR& v )
{
	XMStoreFloat3( this, v );
}

void _tagVector3::operator =( float f[3] )
{
	x = f[0];
	y = f[1];
	z = f[2];
}

void _tagVector3::operator =( int i[3] )
{
	x = i[0];
	y = i[1];
	z = i[2];
}

// =============== +++++++++++++++++++ =======================
_tagVector3 _tagVector3::operator +( const _tagVector3& v )	const
{
	XMVECTOR	v1 = Convert();
	XMVECTOR	v2 = v.Convert();
	_tagVector3	v3;
	XMStoreFloat3( &v3, v1 + v2 );
	return v3;
}

_tagVector3 _tagVector3::operator +( const XMVECTOR& v2 )	const
{
	_tagVector3	v1;
	_tagVector3	v;
	XMStoreFloat3( &v, v2 );
	v1.x = x + v.x;
	v1.y = y + v.y;
	v1.z = z + v.z;
	return v1;
}

_tagVector3 _tagVector3::operator +( float f )	const
{
	_tagVector3	v1;
	v1.x = x + f;
	v1.y = y + f;
	v1.z = z + f;
	return v1;
}

_tagVector3 _tagVector3::operator +( int i )	const
{
	_tagVector3	v1;
	v1.x = x + i;
	v1.y = y + i;
	v1.z = z + i;
	return v1;
}

_tagVector3 _tagVector3::operator +( float f[3] )	const
{
	_tagVector3	v1;
	v1.x = x + f[0];
	v1.y = y + f[1];
	v1.z = z + f[2];
	return v1;
}

_tagVector3 _tagVector3::operator +( int i[3] )	const
{
	_tagVector3	v1;
	v1.x = x + i[0];
	v1.y = y + i[1];
	v1.z = z + i[2];
	return v1;
}

// ============== +=    ==============================
void _tagVector3::operator +=( const _tagVector3& v )
{
	x += v.x;
	y += v.y;
	z += v.z;
}

void _tagVector3::operator +=( const XMVECTOR& v2 )
{
	_tagVector3	v;
	XMStoreFloat3( &v, v2 );
	x += v.x;
	y += v.y;
	z += v.z;
}

void _tagVector3::operator +=( float f )
{
	x += f;
	y += f;
	z += f;
}

void _tagVector3::operator +=( int i )
{
	x += i;
	y += i;
	z += i;
}

void _tagVector3::operator +=( float f[3] )
{
	x += f[0];
	y += f[1];
	z += f[2];
}

void _tagVector3::operator +=( int i[3] )
{
	x += i[0];
	y += i[1];
	z += i[2];
}

// =============== --------------- =======================
_tagVector3 _tagVector3::operator -( const _tagVector3& v )	const
{
	_tagVector3	v1;
	v1.x = x - v.x;
	v1.y = y - v.y;
	v1.z = z - v.z;
	return v1;
}

_tagVector3 _tagVector3::operator -( const XMVECTOR& v2 )	const
{
	_tagVector3	v1;
	_tagVector3	v;
	XMStoreFloat3( &v, v2 );
	v1.x = x - v.x;
	v1.y = y - v.y;
	v1.z = z - v.z;
	return v1;
}

_tagVector3 _tagVector3::operator -( float f )	const
{
	_tagVector3	v1;
	v1.x = x - f;
	v1.y = y - f;
	v1.z = z - f;
	return v1;
}

_tagVector3 _tagVector3::operator -( int i )	const
{
	_tagVector3	v1;
	v1.x = x - i;
	v1.y = y - i;
	v1.z = z - i;
	return v1;
}

_tagVector3 _tagVector3::operator -( float f[3] )	const
{
	_tagVector3	v1;
	v1.x = x - f[0];
	v1.y = y - f[1];
	v1.z = z - f[2];
	return v1;
}

_tagVector3 _tagVector3::operator -( int i[3] )	const
{
	_tagVector3	v1;
	v1.x = x - i[0];
	v1.y = y - i[1];
	v1.z = z - i[2];
	return v1;
}

// ============== -=    ==============================
void _tagVector3::operator -=( const _tagVector3& v )
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void _tagVector3::operator -=( const XMVECTOR& v2 )
{
	_tagVector3	v;
	XMStoreFloat3( &v, v2 );
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void _tagVector3::operator -=( float f )
{
	x -= f;
	y -= f;
	z -= f;
}

void _tagVector3::operator -=( int i )
{
	x -= i;
	y -= i;
	z -= i;
}

void _tagVector3::operator -=( float f[3] )
{
	x -= f[0];
	y -= f[1];
	z -= f[2];
}

void _tagVector3::operator -=( int i[3] )
{
	x -= i[0];
	y -= i[1];
	z -= i[2];
}

// =============== *************** =======================
_tagVector3 _tagVector3::operator *( const _tagVector3& v )	const
{
	_tagVector3	v1;
	v1.x = x * v.x;
	v1.y = y * v.y;
	v1.z = z * v.z;
	return v1;
}

_tagVector3 _tagVector3::operator *( const XMVECTOR& v2 )	const
{
	_tagVector3	v1;
	_tagVector3	v;
	XMStoreFloat3( &v, v2 );
	v1.x = x * v.x;
	v1.y = y * v.y;
	v1.z = z * v.z;
	return v1;
}

_tagVector3 _tagVector3::operator *( float f )	const
{
	_tagVector3	v1;
	v1.x = x * f;
	v1.y = y * f;
	v1.z = z * f;
	return v1;
}

_tagVector3 _tagVector3::operator *( int i )	const
{
	_tagVector3	v1;
	v1.x = x * i;
	v1.y = y * i;
	v1.z = z * i;
	return v1;
}

_tagVector3 _tagVector3::operator *( float f[3] )	const
{
	_tagVector3	v1;
	v1.x = x * f[0];
	v1.y = y * f[1];
	v1.z = z * f[2];
	return v1;
}

_tagVector3 _tagVector3::operator *( int i[3] )	const
{
	_tagVector3	v1;
	v1.x = x * i[0];
	v1.y = y * i[1];
	v1.z = z * i[2];
	return v1;
}

// ============== *=    ==============================
void _tagVector3::operator *=( const _tagVector3& v )
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
}

void _tagVector3::operator *=( const XMVECTOR& v2 )
{
	_tagVector3	v;
	XMStoreFloat3( &v, v2 );
	x *= v.x;
	y *= v.y;
	z *= v.z;
}

void _tagVector3::operator *=( float f )
{
	x *= f;
	y *= f;
	z *= f;
}

void _tagVector3::operator *=( int i )
{
	x *= i;
	y *= i;
	z *= i;
}

void _tagVector3::operator *=( float f[3] )
{
	x *= f[0];
	y *= f[1];
	z *= f[2];
}

void _tagVector3::operator *=( int i[3] )
{
	x *= i[0];
	y *= i[1];
	z *= i[2];
}

// =============== /////////////// =======================
_tagVector3 _tagVector3::operator /( const _tagVector3& v )	const
{
	_tagVector3	v1;
	v1.x = x / v.x;
	v1.y = y / v.y;
	v1.z = z / v.z;
	return v1;
}

_tagVector3 _tagVector3::operator /( const XMVECTOR& v )	const
{
	XMVECTOR	v1, v2;

	v1 = XMLoadFloat3( this );
	v2 = v1 / v;

	_tagVector3	v3;
	XMStoreFloat3( &v3, v2 );

	return v3;
}

_tagVector3 _tagVector3::operator /( float f )	const
{
	_tagVector3	v1;
	v1.x = x / f;
	v1.y = y / f;
	v1.z = z / f;
	return v1;
}

_tagVector3 _tagVector3::operator /( int i )	const
{
	_tagVector3	v1;
	v1.x = x / i;
	v1.y = y / i;
	v1.z = z / i;
	return v1;
}

_tagVector3 _tagVector3::operator /( float f[3] )	const
{
	_tagVector3	v1;
	v1.x = x / f[0];
	v1.y = y / f[1];
	v1.z = z / f[2];
	return v1;
}

_tagVector3 _tagVector3::operator /( int i[3] )	const
{
	_tagVector3	v1;
	v1.x = x / i[0];
	v1.y = y / i[1];
	v1.z = z / i[2];
	return v1;
}

// ============== /=    ==============================
void _tagVector3::operator /=( const _tagVector3& v )
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
}

void _tagVector3::operator /=( const XMVECTOR& v2 )
{
	_tagVector3	v;
	XMStoreFloat3( &v, v2 );
	x /= v.x;
	y /= v.y;
	z /= v.z;
}

void _tagVector3::operator /=( float f )
{
	x /= f;
	y /= f;
	z /= f;
}

void _tagVector3::operator /=( int i )
{
	x /= i;
	y /= i;
	z /= i;
}

void _tagVector3::operator /=( float f[3] )
{
	x /= f[0];
	y /= f[1];
	z /= f[2];
}

void _tagVector3::operator /=( int i[3] )
{
	x /= i[0];
	y /= i[1];
	z /= i[2];
}

bool _tagVector3::operator ==( const _tagVector3& v )	const
{
	return x == v.x && y == v.y && z == v.z;
}

bool _tagVector3::operator ==( const XMVECTOR& v )	const
{
	_tagVector3	v1;
	XMStoreFloat3( &v1, v );
	return x == v1.x && y == v1.y && z == v1.z;
}

bool _tagVector3::operator ==( float f[3] )	const
{
	return x == f[0] && y == f[1] && z == f[2];
}

bool _tagVector3::operator ==( int i[3] )	const
{
	return x == i[0] && y == i[1] && z == i[2];
}

bool _tagVector3::operator !=( const _tagVector3& v )	const
{
	return x != v.x || y != v.y || z != v.z;
}

bool _tagVector3::operator !=( const XMVECTOR& v )	const
{
	_tagVector3	v1;
	XMStoreFloat3( &v1, v );
	return x != v1.x || y != v1.y || z != v1.z;
}

bool _tagVector3::operator !=( float f[3] )	const
{
	return x != f[0] || y != f[1] || z != f[2];
}

bool _tagVector3::operator !=( int i[3] )	const
{
	return x != i[0] || y != i[1] || z != i[2];
}

bool _tagVector3::operator<( const _tagVector3 & v ) const
{
	return x < v.x && y < v.y && z < v.z;
}

bool _tagVector3::operator<=( const _tagVector3 & v ) const
{
	return x <= v.x && y <= v.y && z <= v.z;
}

bool _tagVector3::operator>( const _tagVector3 & v ) const
{
	return x > v.x && y > v.y && z > v.z;
}

bool _tagVector3::operator>=( const _tagVector3 & v ) const
{
	return x >= v.x && y >= v.y && z >= v.z;
}

XMVECTOR _tagVector3::Convert()	const
{
	return XMLoadFloat3( this );
}

float _tagVector3::Length()	const
{
	_tagVector3 v;
	XMStoreFloat3( &v, XMVector3Length( Convert() ) );
	if ( isnan( v.x ) )
		v.x = 0.f;

	return v.x;
}

float _tagVector3::Distance( const _tagVector3 & v ) const
{
	_tagVector3	v1 = v - *this;
	return v1.Length();
}

_tagVector3 _tagVector3::Normalize()	const
{
	XMVECTOR	v = Convert();
	v = XMVector3Normalize( v );
	_tagVector3	v1;
	XMStoreFloat3( &v1, v );
	return v1;
}

float _tagVector3::Dot( const _tagVector3& v1 )	const
{
	XMVECTOR	v = Convert();
	XMVECTOR	v2 = XMLoadFloat3( &v1 );

	XMVECTOR	v3 = XMVector3Dot( v, v2 );
	_tagVector3	v4;
	XMStoreFloat3( &v4, v3 );
	return v4.x;
}

float _tagVector3::Dot( const XMVECTOR& v1 )	const
{
	XMVECTOR	v = Convert();
	XMVECTOR	v2 = XMVector3Dot( v, v1 );
	_tagVector3	v4;
	XMStoreFloat3( &v4, v2 );
	return v4.x;
}

float _tagVector3::Angle( const _tagVector3 & v1 ) const
{
	float	fAngle = Dot( v1 );

	return acosf( fAngle );
}

float _tagVector3::Angle( const XMVECTOR & v1 ) const
{
	float	fAngle = Dot( v1 );

	return acosf( fAngle );
}

_tagVector3 _tagVector3::Cross( const _tagVector3& v1 )	const
{
	XMVECTOR	v = Convert();
	XMVECTOR	v2 = XMLoadFloat3( &v1 );

	XMVECTOR	v3 = XMVector3Cross( v, v2 );
	_tagVector3	v4;
	XMStoreFloat3( &v4, v3 );
	return v4;
}

_tagVector3 _tagVector3::Cross( const XMVECTOR& v1 )	const
{
	XMVECTOR	v = Convert();
	XMVECTOR	v2 = XMVector3Cross( v, v1 );
	_tagVector3	v4;
	XMStoreFloat3( &v4, v2 );
	return v4;
}

_tagVector3 _tagVector3::TransformCoord( const XMMATRIX & m )
{
	_tagVector3	v;
	XMStoreFloat3( &v, XMVector3TransformCoord( Convert(), m ) );
	return v;
}

_tagVector3 _tagVector3::TransformNormal( const XMMATRIX & m )
{
	_tagVector3	v;
	XMStoreFloat3( &v, XMVector3TransformNormal( Convert(), m ) );
	return v;
}
