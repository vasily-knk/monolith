/***
*
*	Copyright (c) 1999, 2000 Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#ifndef VECTOR_H
#define VECTOR_H

typedef float vec_t;
//=========================================================
// 2DVector - used for many pathfinding and many other 
// operations that are treated as planar rather than 3d.
//=========================================================
class Vector2D
{
public:
	inline Vector2D(void)									{ }
	inline Vector2D(float X, float Y)						{ x = X; y = Y; }
	inline int operator==(const Vector2D& v) const	{ return x==v.x && y==v.y;				}
	inline int operator!=(const Vector2D& v) const	{ return !(*this==v);					}
	inline Vector2D operator+(const Vector2D& v)	const	{ return Vector2D(x+v.x, y+v.y);	}
	inline Vector2D operator-(const Vector2D& v)	const	{ return Vector2D(x-v.x, y-v.y);	}
	inline Vector2D operator*(float fl)				const	{ return Vector2D(x*fl, y*fl);	}
	inline Vector2D operator/(float fl)				const	{ return Vector2D(x/fl, y/fl);	}
	
	inline float Length(void)						const	{ return sqrt(x*x + y*y );		}

	inline Vector2D Normalize ( void ) const
	{
		Vector2D vec2;

		float flLen = Length();
		if ( flLen == 0 )
		{
			return Vector2D( 0, 0 );
		}
		else
		{
			flLen = 1 / flLen;
			return Vector2D( x * flLen, y * flLen );
		}
	}

	vec_t	x, y;
};

inline float DotProduct(const Vector2D& a, const Vector2D& b) { return( a.x*b.x + a.y*b.y ); }
inline Vector2D operator*(float fl, const Vector2D& v)	{ return v * fl; }

//=========================================================
// 3D Vector3D
//=========================================================
class Vector3D						// same data-layout as engine's vec3_t,
{								//		which is a vec_t[3]
public:
	// Construction/destruction
	inline Vector3D(void)								{ }
	inline Vector3D(float X, float Y, float Z)		{ x = X; y = Y; z = Z;						}
	//inline Vector3D(double X, double Y, double Z)		{ x = (float)X; y = (float)Y; z = (float)Z;	}
	//inline Vector3D(int X, int Y, int Z)				{ x = (float)X; y = (float)Y; z = (float)Z;	}
	inline Vector3D(const Vector3D& v)					{ x = v.x; y = v.y; z = v.z;				} 
	inline Vector3D(float rgfl[3])					{ x = rgfl[0]; y = rgfl[1]; z = rgfl[2];	}

	// Operators
	inline Vector3D operator-(void) const				{ return Vector3D(-x,-y,-z);				}
	inline int operator==(const Vector3D& v) const	{ return x==v.x && y==v.y && z==v.z;	}
	inline int operator!=(const Vector3D& v) const	{ return !(*this==v);					}
	inline Vector3D operator+(const Vector3D& v) const	{ return Vector3D(x+v.x, y+v.y, z+v.z);	}
	inline Vector3D operator-(const Vector3D& v) const	{ return Vector3D(x-v.x, y-v.y, z-v.z);	}
	inline Vector3D operator*(float fl) const			{ return Vector3D(x*fl, y*fl, z*fl);		}
	inline Vector3D operator/(float fl) const			{ return Vector3D(x/fl, y/fl, z/fl);		}
	
	// Methods
	inline void CopyToArray(float* rgfl) const		{ rgfl[0] = x, rgfl[1] = y, rgfl[2] = z; }
	inline float Length(void) const					{ return sqrt(x*x + y*y + z*z); }
	operator float *()								{ return &x; } // Vectors will now automatically convert to float * when needed
	operator const float *() const					{ return &x; } // Vectors will now automatically convert to float * when needed
	inline Vector3D Normalize(void) const
	{
		float flLen = Length();
		if (flLen == 0) return Vector3D(0,0,1); // ????
		flLen = 1 / flLen;
		return Vector3D(x * flLen, y * flLen, z * flLen);
	}

	inline Vector2D Make2D ( void ) const
	{
		Vector2D	Vec2;

		Vec2.x = x;
		Vec2.y = y;

		return Vec2;
	}
	inline float Length2D(void) const					{ return sqrt(x*x + y*y); }

	// Members
	vec_t x, y, z;
};
inline Vector3D operator*(float fl, const Vector3D& v)	{ return v * fl; }
inline float DotProduct(const Vector3D& a, const Vector3D& b) { return(a.x*b.x+a.y*b.y+a.z*b.z); }
inline Vector3D CrossProduct(const Vector3D& a, const Vector3D& b) { return Vector3D( a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x ); }

#define VEC_ZERO Vector(0,0)

#endif
