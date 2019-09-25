
#include "EngineVector2.h"



_tagVector2 _tagVector2::Zero = _tagVector2(0.f, 0.f);
_tagVector2 _tagVector2::One = _tagVector2(1.f, 1.f);

_tagVector2::_tagVector2()
{
	x = y = 0.f;
}

_tagVector2::_tagVector2(const _tagVector2& v)
{
	x = v.x;
	y = v.y;
}

_tagVector2::_tagVector2(float _x, float _y)
{
	x = _x;
	y = _y;
}

_tagVector2::_tagVector2(int _x, int _y)
{
	x = _x;
	y = _y;
}

_tagVector2::_tagVector2(float f[2])
{
	x = f[0];
	y = f[1];
}

_tagVector2::_tagVector2(int i[2])
{
	x = i[0];
	y = i[1];
}

void _tagVector2::operator =(const _tagVector2& v)
{
	x = v.x;
	y = v.y;
}

void _tagVector2::operator =(const XMFLOAT2& v)
{
	x = v.x;
	y = v.y;
}

void _tagVector2::operator =(const XMFLOAT4& v)
{
	x = v.x;
	y = v.y;
}

void _tagVector2::operator =(const XMVECTOR& v)
{
	XMStoreFloat2(this, v);
}

void _tagVector2::operator =(float f[2])
{
	x = f[0];
	y = f[1];
}

void _tagVector2::operator =(int i[2])
{
	x = i[0];
	y = i[1];
}

// =============== +++++++++++++++++++ =======================
_tagVector2 _tagVector2::operator +(const _tagVector2& v)	const
{
	XMVECTOR	v1 = Convert();
	XMVECTOR	v2 = v.Convert();
	_tagVector2	v3;
	XMStoreFloat2(&v3, v1 + v2);
	return v3;
}

_tagVector2 _tagVector2::operator +(const XMVECTOR& v2)	const
{
	_tagVector2	v1;
	_tagVector2	v;
	XMStoreFloat2(&v, v2);
	v1.x = x + v.x;
	v1.y = y + v.y;
	return v1;
}

_tagVector2 _tagVector2::operator +(float f)	const
{
	_tagVector2	v1;
	v1.x = x + f;
	v1.y = y + f;
	return v1;
}

_tagVector2 _tagVector2::operator +(int i)	const
{
	_tagVector2	v1;
	v1.x = x + i;
	v1.y = y + i;
	return v1;
}

_tagVector2 _tagVector2::operator +(float f[2])	const
{
	_tagVector2	v1;
	v1.x = x + f[0];
	v1.y = y + f[1];
	return v1;
}

_tagVector2 _tagVector2::operator +(int i[2])	const
{
	_tagVector2	v1;
	v1.x = x + i[0];
	v1.y = y + i[1];
	return v1;
}

// ============== +=    ==============================
void _tagVector2::operator +=(const _tagVector2& v)
{
	x += v.x;
	y += v.y;
}

void _tagVector2::operator +=(const XMVECTOR& v2)
{
	_tagVector2	v;
	XMStoreFloat2(&v, v2);
	x += v.x;
	y += v.y;
}

void _tagVector2::operator +=(float f)
{
	x += f;
	y += f;
}

void _tagVector2::operator +=(int i)
{
	x += i;
	y += i;
}

void _tagVector2::operator +=(float f[2])
{
	x += f[0];
	y += f[1];
}

void _tagVector2::operator +=(int i[2])
{
	x += i[0];
	y += i[1];
}

// =============== --------------- =======================
_tagVector2 _tagVector2::operator -(const _tagVector2& v)	const
{
	_tagVector2	v1;
	v1.x = x - v.x;
	v1.y = y - v.y;
	return v1;
}

_tagVector2 _tagVector2::operator -(const XMVECTOR& v2)	const
{
	_tagVector2	v1;
	_tagVector2	v;
	XMStoreFloat2(&v, v2);
	v1.x = x - v.x;
	v1.y = y - v.y;
	return v1;
}

_tagVector2 _tagVector2::operator -(float f)	const
{
	_tagVector2	v1;
	v1.x = x - f;
	v1.y = y - f;
	return v1;
}

_tagVector2 _tagVector2::operator -(int i)	const
{
	_tagVector2	v1;
	v1.x = x - i;
	v1.y = y - i;
	return v1;
}

_tagVector2 _tagVector2::operator -(float f[2])	const
{
	_tagVector2	v1;
	v1.x = x - f[0];
	v1.y = y - f[1];
	return v1;
}

_tagVector2 _tagVector2::operator -(int i[2])	const
{
	_tagVector2	v1;
	v1.x = x - i[0];
	v1.y = y - i[1];
	return v1;
}

// ============== -=    ==============================
void _tagVector2::operator -=(const _tagVector2& v)
{
	x -= v.x;
	y -= v.y;
}

void _tagVector2::operator -=(const XMVECTOR& v2)
{
	_tagVector2	v;
	XMStoreFloat2(&v, v2);
	x -= v.x;
	y -= v.y;
}

void _tagVector2::operator -=(float f)
{
	x -= f;
	y -= f;
}

void _tagVector2::operator -=(int i)
{
	x -= i;
	y -= i;
}

void _tagVector2::operator -=(float f[2])
{
	x -= f[0];
	y -= f[1];
}

void _tagVector2::operator -=(int i[2])
{
	x -= i[0];
	y -= i[1];
}

// =============== *************** =======================
_tagVector2 _tagVector2::operator *(const _tagVector2& v)	const
{
	_tagVector2	v1;
	v1.x = x * v.x;
	v1.y = y * v.y;
	return v1;
}

_tagVector2 _tagVector2::operator *(const XMVECTOR& v2)	const
{
	_tagVector2	v1;
	_tagVector2	v;
	XMStoreFloat2(&v, v2);
	v1.x = x * v.x;
	v1.y = y * v.y;
	return v1;
}

_tagVector2 _tagVector2::operator *(float f)	const
{
	_tagVector2	v1;
	v1.x = x * f;
	v1.y = y * f;
	return v1;
}

_tagVector2 _tagVector2::operator *(int i)	const
{
	_tagVector2	v1;
	v1.x = x * i;
	v1.y = y * i;
	return v1;
}

_tagVector2 _tagVector2::operator *(float f[2])	const
{
	_tagVector2	v1;
	v1.x = x * f[0];
	v1.y = y * f[1];
	return v1;
}

_tagVector2 _tagVector2::operator *(int i[2])	const
{
	_tagVector2	v1;
	v1.x = x * i[0];
	v1.y = y * i[1];
	return v1;
}

// ============== *=    ==============================
void _tagVector2::operator *=(const _tagVector2& v)
{
	x *= v.x;
	y *= v.y;
}

void _tagVector2::operator *=(const XMVECTOR& v2)
{
	_tagVector2	v;
	XMStoreFloat2(&v, v2);
	x *= v.x;
	y *= v.y;
}

void _tagVector2::operator *=(float f)
{
	x *= f;
	y *= f;
}

void _tagVector2::operator *=(int i)
{
	x *= i;
	y *= i;
}

void _tagVector2::operator *=(float f[2])
{
	x *= f[0];
	y *= f[1];
}

void _tagVector2::operator *=(int i[2])
{
	x *= i[0];
	y *= i[1];
}

// =============== /////////////// =======================
_tagVector2 _tagVector2::operator /(const _tagVector2& v)	const
{
	_tagVector2	v1;
	v1.x = x / v.x;
	v1.y = y / v.y;
	return v1;
}

_tagVector2 _tagVector2::operator /(const XMVECTOR& v)	const
{
	XMVECTOR	v1, v2;

	v1 = XMLoadFloat2(this);
	v2 = v1 / v;

	_tagVector2	v3;
	XMStoreFloat2(&v3, v2);

	return v3;
}

_tagVector2 _tagVector2::operator /(float f)	const
{
	_tagVector2	v1;
	v1.x = x / f;
	v1.y = y / f;
	return v1;
}

_tagVector2 _tagVector2::operator /(int i)	const
{
	_tagVector2	v1;
	v1.x = x / i;
	v1.y = y / i;
	return v1;
}

_tagVector2 _tagVector2::operator /(float f[2])	const
{
	_tagVector2	v1;
	v1.x = x / f[0];
	v1.y = y / f[1];
	return v1;
}

_tagVector2 _tagVector2::operator /(int i[2])	const
{
	_tagVector2	v1;
	v1.x = x / i[0];
	v1.y = y / i[1];
	return v1;
}

// ============== /=    ==============================
void _tagVector2::operator /=(const _tagVector2& v)
{
	x /= v.x;
	y /= v.y;
}

void _tagVector2::operator /=(const XMVECTOR& v2)
{
	_tagVector2	v;
	XMStoreFloat2(&v, v2);
	x /= v.x;
	y /= v.y;
}

void _tagVector2::operator /=(float f)
{
	x /= f;
	y /= f;
}

void _tagVector2::operator /=(int i)
{
	x /= i;
	y /= i;
}

void _tagVector2::operator /=(float f[2])
{
	x /= f[0];
	y /= f[1];
}

void _tagVector2::operator /=(int i[2])
{
	x /= i[0];
	y /= i[1];
}

bool _tagVector2::operator ==(const _tagVector2& v)	const
{
	return x == v.x && y == v.y;
}

bool _tagVector2::operator ==(const XMVECTOR& v)	const
{
	_tagVector2	v1;
	XMStoreFloat2(&v1, v);
	return x == v1.x && y == v1.y;
}

bool _tagVector2::operator ==(float f[2])	const
{
	return x == f[0] && y == f[1];
}

bool _tagVector2::operator ==(int i[2])	const
{
	return x == i[0] && y == i[1];
}

bool _tagVector2::operator !=(const _tagVector2& v)	const
{
	return x != v.x || y != v.y;
}

bool _tagVector2::operator !=(const XMVECTOR& v)	const
{
	_tagVector2	v1;
	XMStoreFloat2(&v1, v);
	return x != v1.x || y != v1.y;
}

bool _tagVector2::operator !=(float f[2])	const
{
	return x != f[0] || y != f[1];
}

bool _tagVector2::operator !=(int i[2])	const
{
	return x != i[0] || y != i[1];
}

XMVECTOR _tagVector2::Convert()	const
{
	return XMLoadFloat2(this);
}

float _tagVector2::Length()	const
{
	_tagVector2 v;
	XMStoreFloat2(&v, XMVector2Length(Convert()));
	return v.x;
}

_tagVector2 _tagVector2::Normalize()	const
{
	XMVECTOR	v = Convert();
	v = XMVector2Normalize(v);
	_tagVector2	v1;
	XMStoreFloat2(&v1, v);
	return v1;
}
