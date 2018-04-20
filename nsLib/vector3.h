#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <math.h>

//-----------------------------------------------------
//  struct nsVec3:  
//-----------------------------------------------------
struct nsVec3
{
	union
	{
		struct 
		{
			float x, y, z;
		};
		
		float	vec[3];
	};
	
	inline nsVec3( float _x, float _y, float _z )
	{
		x = _x;
		y = _y;
		z = _z;
	}
	
	inline nsVec3( const float* v )
	{
		x = v[0];
		y = v[1];
		z = v[2];
	}
	
	inline nsVec3()
	{
		x = y = z = 0;
	}
	
	inline float Dot( const nsVec3& v ) const
	{
		return x * v.x + y * v.y + z * v.z;
	}
	
	inline float Length() const
	{
		return sqrtf( Dot( *this ) );
	}
	
	inline void Norm()
	{
		float	len = Length();
		if ( !len ) return;
		x /= len;
		y /= len;
		z /= len;
	}

	inline void Rand()
	{
		x = (rand() % 1000) / 500.0f - 1;
		y = (rand() % 1000) / 500.0f - 1;
		z = (rand() % 1000) / 500.0f - 1;
	}
	
	inline nsVec3 Cross( const nsVec3& v )
	{
		nsVec3	c;
		c.x = y * v.z - z * v.y;
		c.y = z * v.x - x * v.z;
		c.z = x * v.y - y * v.x;
		return c;
	}
	
	inline const char* ToStr() const
	{
		static char	str[64];
		sprintf( str, "(%f %f %f)", x, y, z );
		return str;
	}
	
	inline nsVec3& operator *= ( float t )
	{
		x *= t;
		y *= t;
		z *= t;
		return *this;
	}

	inline nsVec3 operator - () const
	{
		return nsVec3( -x, -y, -z );
	}

	inline nsVec3& operator += ( const nsVec3& v )
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
};

//-----------------------------------------------------
//  nsVec3 operator *:  
//-----------------------------------------------------
inline nsVec3 operator * ( const nsVec3& v, float t ) 
{
	return nsVec3( v.x * t, v.y * t, v.z * t );
}

inline nsVec3 operator * ( float t, const nsVec3& v )
{
	return nsVec3( v.x * t, v.y * t, v.z * t );
}

inline nsVec3 operator + ( const nsVec3& v1, const nsVec3& v2 )
{
	return nsVec3( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z );
}

inline nsVec3 operator - ( const nsVec3& v1, const nsVec3& v2 )
{
	return nsVec3( v1.x - v2.x, v1.y - v2.y, v1.z - v2.z );
}


#endif