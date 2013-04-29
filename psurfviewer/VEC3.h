#pragma once


#include <iostream>
#include <iomanip>

#include "SGFaceList.h"

#ifndef M_PI
#define M_PI        3.14159265358979323846264338327950288
#endif

#ifndef DEG2RAD
#define DEG2RAD( theta ) ((theta) * (M_PI/180.0))
#endif

#ifndef RAD2DEG
#define RAD2DEG( theta ) ((theta) * (180.0/M_PI))
#endif

#ifndef ABS
#define ABS( x ) ((x) < (0) ? (-x) : (x))
#endif

#ifndef ALMOST_ZERO
#define ALMOST_ZERO 1e-6
#endif

/*
#ifndef XYZW
  enum{X = 0, Y, Z, W};
#endif*/


class VEC3
{
	double components[3];
public:
	VEC3 ();
	VEC3( const double& a, const double& b, const double& c );
	VEC3( float a[3] );
	VEC3 add( VEC3 x );
	VEC3 subtract( VEC3 x );
	VEC3 multiply( double y );
	double dot( VEC3 x );
	VEC3 negate();
	double Length();
	double SquaredLength();
	void Normalize();
	void Display();
	VEC3 operator +( VEC3 x );
	VEC3 operator -( VEC3 x );
	VEC3 operator *( double y );
	VEC3 operator -();
	void operator =( float *x );
	bool operator ==( VEC3 x );
	bool operator !=( VEC3 x );
	double &operator []( const size_t index );
	const double &operator []( const size_t index ) const;
	~VEC3(void);
};

VEC3 operator *( double y, VEC3 x );
VEC3 cross( VEC3 a, VEC3 b );
VEC3 normal( VEC3& a, VEC3& b, VEC3& c );
void Normalize( VEC3& v );
std::ostream& operator <<( std::ostream &out, const VEC3 &v );