#pragma once
#include "VEC3.h"
#include <iostream>
#include <iomanip>

#ifndef ALMOST_ZERO
#define ALMOST_ZERO 1e-6
#endif

class MAT3
{
	double data[9];
public:
	MAT3( void );
	MAT3( double a, double b, double c,
		  double d, double e, double f,
		  double g, double h, double i );
	MAT3( VEC3 v1, VEC3 v2, VEC3 v3 );
	MAT3( double a[9] );
	MAT3 transpose();
	void Display();
	MAT3 ScalarM( double x );	
	MAT3 multiply( MAT3 m );
	double determinate();
	MAT3 cofactor();
	MAT3 adjoint();
	MAT3 inverse();
	void identity();
	void X_ROTATE( double angle );
	void Y_ROTATE( double angle );
	void Z_ROTATE( double angle );
	double &operator []( const size_t index );
	const double &operator []( const size_t index ) const;
	MAT3 operator *( double x );
	MAT3 operator /( double x );
	MAT3 operator *( MAT3 &m );
	VEC3 operator *( VEC3 &v );
	MAT3 operator +( MAT3 &m );
	MAT3 operator -( MAT3 &m );
	~MAT3(void);
};

MAT3 operator *( double x, MAT3 m );
MAT3 rotate( const double angleInDegrees, const double x, const double y, const double z );
MAT3 rotate( const double angleInDegrees, const VEC3& v );
MAT3 scale( double a, double b, double c );
MAT3 uniformScale( double s );
void copyToFourByFourMatrix( double a[16], MAT3 &m );
void copyToThreeByThreeMatrix( MAT3 &m, float a[16] );
std::ostream& operator <<( std::ostream &out, const MAT3 &m );