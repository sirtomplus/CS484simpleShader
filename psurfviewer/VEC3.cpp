#include "VEC3.h"


VEC3::VEC3()
{
	for( int i = 0; i < 3; ++i )
		components[i] = 0;
}

VEC3::VEC3( const double& a, const double& b, const double& c )
{
	components[X] = a;
	components[Y] = b;
	components[Z] = c;
}

VEC3::VEC3( float a[3] )
{
	for( int i = 0; i < 3; ++i )
		components[i] = a[i];
}

VEC3 VEC3::add( VEC3 x )
{
	VEC3 temp;
	for( int i = 0; i < 3; i++ )
		temp.components[i] = components[i] + x.components[i];
	return temp;
}

VEC3 VEC3::subtract( VEC3 x )
{
	VEC3 temp;
	for( int i = 0; i < 3; i++ )
		temp.components[i] = components[i] - x.components[i];
	return temp;
}
VEC3 VEC3::multiply( double y )
{
	VEC3 temp;
	for( int i = 0; i < 3; i++ )
		temp.components[i] = components[i] * y;
	return temp;
}

double VEC3::dot( VEC3 x )
{
	double z = 0;
	for( int i = 0; i < 3; i++ )
		z += components[i] * x.components[i];
	return z;
}

VEC3 VEC3::negate()
{
	VEC3 temp;
	for( int i = 0; i < 3; i++ )
		temp.components[i] = -1 * components[i];
	return temp;
}

double VEC3::Length()
{
	return sqrt( (components[0]*components[0]) + (components[1]*components[1])
		+ (components[2]*components[2]) );
}

double VEC3::SquaredLength()
{
	return (components[0]*components[0]) + (components[1]*components[1])
		+ (components[2]*components[2]);
}

void VEC3::Normalize()
{
	for( int i = 0; i < 3; i++ )
		components[i] = components[i]/Length();
}

void VEC3::Display()
{
	for( int i = 0; i < 3; i++ )
		printf( "%.16f  ", components[i] );
	printf( "\n" );
}

VEC3 VEC3::operator +( VEC3 x )
{
	return add( x );
}

VEC3 VEC3::operator -( VEC3 x )
{
	return subtract( x );
}

VEC3 VEC3::operator *( double y )
{
	return multiply( y );
}

VEC3 operator *( double y, VEC3 x )
{
	return x.multiply( y );
}

VEC3 VEC3::operator -()
{
	return negate();
}

void VEC3::operator=( float *x )
{
	for( int i = 0; i < 3; i++ )
		components[i] = x[i];
}

bool VEC3::operator ==( VEC3 x )
{
	for( int i = 0; i < 3; i++ )
		if( abs(components[i]-x.components[i]) > ALMOST_ZERO )
			return false;
	return true;
}

bool VEC3::operator !=( VEC3 x )
{
	if( *this == x )
		return false;
	else 
		return true;
}


double &VEC3::operator []( const size_t index )
{
	return components[index];
}

const double &VEC3::operator []( const size_t index ) const
{
	return components[index];
}

VEC3::~VEC3(void)
{
}

VEC3 cross( VEC3 a, VEC3 b )
{
	VEC3 temp;
	temp[0] = a[1]*b[2] - a[2]*b[1];
	temp[1] = a[2]*b[0] - a[0]*b[2];
	temp[2] = a[0]*b[1] - a[1]*b[0];
	return temp;
}

VEC3 normal( VEC3& a, VEC3& b, VEC3& c )
{
	VEC3 dir = cross( b-a, c-a );
	dir.Normalize();
	return dir;
}

void Normalize( VEC3& v )
{
	for( int i = 0; i < 3; i++ )
		v[i] = v[i]/v.Length();
}



std::ostream& operator<<( std::ostream &out, const VEC3 &v )
{
	out<<std::showpoint<<std::setprecision(6);
	out<<v[X]<<"\n"<<v[Y]<<"\n"<<v[Z];
	return out;
}