#ifndef _COLOR_H_
#define _COLOR_H_

struct nsColor
{
	union
	{
		struct 
		{
			float	r, g, b, a;
		};
		float	color[4];
	};
	
	inline nsColor()
	{
		r = g = b = a = 1;
	}
	
	inline nsColor( float _r, float _g, float _b, float _a )
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
	
	inline operator float*()
	{
		return color;
	}
	
	inline void operator = ( const float* c )
	{
		color[0] = c[0];
		color[1] = c[1];
		color[2] = c[2];
		color[3] = c[3];
	}
	
	inline dword ToARGB() const
	{
		return byte(255 * color[3]) << 24 |
			byte(255 * color[0]) << 16 |
			byte(255 * color[1]) << 8 |
			byte(255 * color[2]);
	}

	inline void Rand()
	{
		r = (rand() % 1000) / 1000.0f;
		g = (rand() % 1000) / 1000.0f;
		b = (rand() % 1000) / 1000.0f;
		a = 1;
		color[ rand() % 3 ] = 1;
	}
};

#endif