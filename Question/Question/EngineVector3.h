#pragma once

#include "EngineMath.h"
#include "Flag.h"


typedef struct  _tagVector3 :
	public XMFLOAT3
{
	_tagVector3();

	_tagVector3(const _tagVector3& v);

	_tagVector3(float _x, float _y, float _z);

	_tagVector3(int _x, int _y, int _z);

	_tagVector3(float f[3]);

	_tagVector3(int i[3]);

	void operator =(const _tagVector3& v);

	void operator =(const XMFLOAT3& v);

	void operator =(const XMFLOAT4& v);

	void operator =(const XMVECTOR& v);

	void operator =(float f[3]);

	void operator =(int i[3]);

	// =============== +++++++++++++++++++ =======================
	_tagVector3 operator +(const _tagVector3& v)	const;

	_tagVector3 operator +(const XMVECTOR& v2)	const;

	_tagVector3 operator +(float f)	const;

	_tagVector3 operator +(int i)	const;

	_tagVector3 operator +(float f[3])	const;

	_tagVector3 operator +(int i[3])	const;

	// ============== +=    ==============================
	void operator +=(const _tagVector3& v);

	void operator +=(const XMVECTOR& v2);

	void operator +=(float f);

	void operator +=(int i);

	void operator +=(float f[3]);

	void operator +=(int i[3]);

	// =============== --------------- =======================
	_tagVector3 operator -(const _tagVector3& v)	const;

	_tagVector3 operator -(const XMVECTOR& v2)	const;

	_tagVector3 operator -(float f)	const;

	_tagVector3 operator -(int i)	const;

	_tagVector3 operator -(float f[3])	const;

	_tagVector3 operator -(int i[3])	const;

	// ============== -=    ==============================
	void operator -=(const _tagVector3& v);

	void operator -=(const XMVECTOR& v2);

	void operator -=(float f);

	void operator -=(int i);

	void operator -=(float f[3]);

	void operator -=(int i[3]);

	// =============== *************** =======================
	_tagVector3 operator *(const _tagVector3& v)	const;

	_tagVector3 operator *(const XMVECTOR& v2)	const;

	_tagVector3 operator *(float f)	const;

	_tagVector3 operator *(int i)	const;

	_tagVector3 operator *(float f[3])	const;

	_tagVector3 operator *(int i[3])	const;

	// ============== *=    ==============================
	void operator *=(const _tagVector3& v);

	void operator *=(const XMVECTOR& v2);

	void operator *=(float f);

	void operator *=(int i);

	void operator *=(float f[3]);

	void operator *=(int i[3]);

	// =============== /////////////// =======================
	_tagVector3 operator /(const _tagVector3& v)	const;

	_tagVector3 operator /(const XMVECTOR& v)	const;

	_tagVector3 operator /(float f)	const;

	_tagVector3 operator /(int i)	const;

	_tagVector3 operator /(float f[3])	const;

	_tagVector3 operator /(int i[3])	const;

	// ============== /=    ==============================
	void operator /=(const _tagVector3& v);

	void operator /=(const XMVECTOR& v2);

	void operator /=(float f);

	void operator /=(int i);

	void operator /=(float f[3]);

	void operator /=(int i[3]);

	bool operator ==(const _tagVector3& v)	const;

	bool operator ==(const XMVECTOR& v)	const;

	bool operator ==(float f[3])	const;

	bool operator ==(int i[3])	const;

	bool operator !=(const _tagVector3& v)	const;

	bool operator !=(const XMVECTOR& v)	const;

	bool operator !=(float f[3])	const;

	bool operator !=(int i[3])	const;

	bool operator < (const _tagVector3& v)	const;

	bool operator <= (const _tagVector3& v)	const;

	bool operator > (const _tagVector3& v)	const;

	bool operator >= (const _tagVector3& v)	const;

	XMVECTOR Convert()	const;

	float Length()	const;

	float Distance(const _tagVector3& v)	const;

	_tagVector3 Normalize()	const;

	float Dot(const _tagVector3& v1)	const;

	float Dot(const XMVECTOR& v1)	const;

	float Angle(const _tagVector3& v1)	const;
	float Angle(const XMVECTOR& v1)	const;

	_tagVector3 Cross(const _tagVector3& v1)	const;

	_tagVector3 Cross(const XMVECTOR& v1)	const;

	_tagVector3 TransformCoord(const XMMATRIX& m);
	_tagVector3 TransformNormal(const XMMATRIX& m);

	static _tagVector3	One;
	static _tagVector3	Zero;
	static _tagVector3	Axis[AXIS_END];
	static _tagVector3	Axis2D[AXIS_END];
}Vector3, *PVector3;

