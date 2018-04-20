#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#include "vector3.h"
#include <math.h>

//-----------------------------------------------------
//  struct nsMatrix:  
//-----------------------------------------------------
struct nsMatrix
{
	union
	{
		float m[4][4];

		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
	};

	inline nsMatrix() {}

	//-----------------------------------------------------
	//  explicit nsMatrix( float scale ):  
	//-----------------------------------------------------
	inline explicit nsMatrix( float scale )
	{
		Scale( scale, scale, scale );
	}

	//-----------------------------------------------------
	//  Identity:  
	//-----------------------------------------------------
	inline void Identity()
	{
		_11 = _22 = _33 = _44 = 1;
		_12 = _13 = _14 = _21 = _23 = _24 = _31 = _32 = _34 = _41 = _42 = _43 = 0;
	}

	//-----------------------------------------------------
	//  Scale:  
	//-----------------------------------------------------
	inline void Scale( float x, float y, float z )
	{
		_11 = x;
		_22 = y;
		_33 = z;
		_44 = 1;

		_12 = _13 = _14 = _21 = _23 = _24 = _31 = _32 = _34 = _41 = _42 = _43 = 0;
	}

	//-----------------------------------------------------
	//  AxisRotate:  
	//-----------------------------------------------------
	inline void AxisRotate( nsVec3 &v, float angle )
	{
		_41 = _42 = _43 = _14 = _24 = _34 = 0;
		_44 = 1;
		
		float	c = cosf( -angle );
		float	s = sinf( -angle );

		_11 = v.x * v.x + ( 1 - v.x * v.x ) * c;
		_12 = v.x * v.y * ( 1 - c ) + v.z * s;
		_13 = v.x * v.z * ( 1 - c ) - v.y * s;
		_21 = v.x * v.y * ( 1 - c ) - v.z * s;

		_22 = v.y * v.y + ( 1 - v.y * v.y ) * c;
		_23 = v.y * v.z * ( 1 - c ) + v.x * s;
		_31 = v.x * v.z * ( 1 - c ) + v.y * s;
		_32 = v.y * v.z * ( 1 - c ) - v.x * s;
		
		_33 = v.z * v.z + ( 1 - v.z * v.z ) * c;
	}

	//-----------------------------------------------------
	//  nsMatrix& operator =:  
	//-----------------------------------------------------
	inline nsMatrix& operator = ( float _m[4][4] )
	{
		for ( register int i = 0; i < 4; i++ )
			for ( register int j = 0; j < 4; j++ )
				m[i][j] = _m[i][j];
		return *this;
	}

	//-----------------------------------------------------
	//  nsMatrix& operator =:  
	//-----------------------------------------------------
	inline nsMatrix& operator = ( float* _m )
	{
		register float	*mat = &_11;
		for ( register int i = 0; i < 16; i++, mat++, _m++ )
			*mat = *_m;
		return *this;
	}

	//-----------------------------------------------------
	//  RotateY:  
	//-----------------------------------------------------
	inline void RotateY( float angle )
	{
		Identity();
		float	c = cosf( -angle );
		float	s = sinf( -angle );

		_11 = c;	_13 = s;
		_31 = -s;	_33 = c;
	}

	//-----------------------------------------------------
	//  RotateZ:  
	//-----------------------------------------------------
	inline void RotateZ( float angle )
	{
		Identity();
		float	c = cosf( -angle );
		float	s = sinf( -angle );

		_11 = c;	_12 = s;
		_21 = -s;	_22 = c;
	}

	//-----------------------------------------------------
	//  RotateX:  
	//-----------------------------------------------------
	inline void RotateX( float angle )
	{
		Identity();
		float	c = cosf( -angle );
		float	s = sinf( -angle );

		_22 = c;	_23 = s;
		_32 = -s;	_33 = c;
	}
};

inline nsMatrix& operator* ( nsMatrix &a, nsMatrix &b )
{
	static	nsMatrix	res;
	for ( register int i = 0; i < 4; i++ )
		for ( register int j = 0; j < 4; j++ )
		{
			register float sum = 0;
			for ( register k = 0; k < 4; k++ )
				sum += a.m[i][k] * b.m[k][j];
			res.m[i][j] = sum;
		}

	return res;
}

inline void operator *= ( nsVec3 &v, nsMatrix &m )
{
	nsVec3	r;
	r.x = v.x * m._11 + v.y * m._21 + v.z * m._31;
	r.y = v.x * m._12 + v.y * m._22 + v.z * m._32;
	r.z = v.x * m._13 + v.y * m._23 + v.z * m._33;

	v = r;
}

#endif