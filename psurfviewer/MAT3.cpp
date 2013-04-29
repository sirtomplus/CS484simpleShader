#include "MAT3.h"


MAT3::MAT3( void )
{
	for( int i = 0; i < 9; i++ )
		data[i] = 0;
}

MAT3::MAT3( double a, double b, double c,
			double d, double e, double f, 
			double g, double h, double i )
{
	data[0] = a;
	data[1] = b;
	data[2] = c;
	data[3] = d;
	data[4] = e;
	data[5] = f;
	data[6] = g;
	data[7] = h;
	data[8] = i;
}

MAT3::MAT3( VEC3 v1, VEC3 v2, VEC3 v3 )
{
	for( int i = 0; i < 3; i++ )
		data[i] = v1[i];
	for( int i = 0; i < 3; i++ )
		data[i+3] = v2[i];
	for( int i = 0; i < 3; i++ )
		data[i+6] = v3[i];
}


MAT3::MAT3( double a[9] )
{
	for( int i = 0; i < 9; i++ )
		data[i] = a[i];
}

MAT3 MAT3::transpose()
{
	MAT3 TRANSPOSE;
	for( int i = 0; i < 3; i++ )
		for( int j = 0; j < 3; j++ )
			TRANSPOSE.data[i*3+j] = data[i+(j*3)];
	return TRANSPOSE;
}

void MAT3::Display()
{
	for( int i = 0; i < 3; i++ )
	{
		for( int j = 0; j < 3; j++ )
			printf( "%.16f ", data[ ( j * 3 ) + i ] );
		printf( "\n" );
	}
}

MAT3 MAT3::ScalarM( double x )
{
	MAT3 temp;
	for( int i = 0; i < 9; i++ )
		temp.data[i] = data[i] * x;
	return temp;
}

MAT3 MAT3::multiply( MAT3 m )
{
	MAT3 temp;
	for( int i = 0; i < 3; i++ )
		for( int j = 0; j < 3; j++ )
			for( int k = 0; k < 3; k++ )
				temp.data[ i + ( j*3 ) ] += data[ ( k*3 ) + i ] * m.data[ k + ( j*3 ) ];
	return temp;
}

double MAT3::determinate()
{
	return ((data[0]*data[4]*data[8] + data[3]*data[2]*data[7] + data[6]*data[1]*data[5]) - 
		(data[6]*data[4]*data[2] + data[3]*data[1]*data[8] + data[0]*data[7]*data[5]));
}

MAT3 MAT3::cofactor()
{
	MAT3 COFACTOR;
	COFACTOR.data[0] = data[4]*data[8] - data[5]*data[7];
	COFACTOR.data[1] = -1*(data[3]*data[8] - data[5]*data[6]);
	COFACTOR.data[2] = data[3]*data[7] - data[4]*data[6];
	COFACTOR.data[3] = -1*(data[1]*data[8] - data[2]*data[7]);
	COFACTOR.data[4] = data[0]*data[8] - data[2]*data[6];
	COFACTOR.data[5] = -1*(data[0]*data[7] - data[1]*data[6]);
	COFACTOR.data[6] = data[1]*data[5] - data[2]*data[4];
	COFACTOR.data[7] = -1*(data[0]*data[5] - data[2]*data[3]);
	COFACTOR.data[8] = data[0]*data[4] - data[1]*data[3];
	return COFACTOR;
}

MAT3 MAT3::adjoint()
{
	MAT3 COFACTOR = cofactor();
	return COFACTOR.transpose();
}

MAT3 MAT3::inverse()
{
	MAT3 INVERSE;
	MAT3 ADJOINT = adjoint();
	double DETERMINATE = determinate();
	for( int i = 0; i < 9; i++ )
		INVERSE.data[i] = ADJOINT.data[i]/DETERMINATE;
	return INVERSE;
}

void MAT3::identity()
{
	for( int i = 0; i < 9; i++ )
	{
		if( i == 0 || i == 4 || i == 8 )
			data[i] = 1;
		else 
			data[i] = 0;
	}
}

void MAT3::X_ROTATE( double angle )
{
	VEC3 v1( 1, 0, 0 );
	VEC3 v2( 0, cos(DEG2RAD(angle)), sin(DEG2RAD(angle)) );
	VEC3 v3( 0, -sin(DEG2RAD(angle)), cos(DEG2RAD(angle)) );
	MAT3 temp( v1, v2, v3 );
	*this = *this * temp;
}

void MAT3::Y_ROTATE( double angle )
{
	VEC3 v1( cos(DEG2RAD(angle)), 0, -sin(DEG2RAD(angle)) );
	VEC3 v2( 0, 1, 0 );
	VEC3 v3( sin(DEG2RAD(angle)), 0, cos(DEG2RAD(angle)) );
	MAT3 temp( v1, v2, v3 );
	*this = *this * temp;
}

void MAT3::Z_ROTATE( double angle )
{
	VEC3 v1( cos(DEG2RAD(angle)), sin(DEG2RAD(angle)), 0 );
	VEC3 v2( -sin(DEG2RAD(angle)), cos(DEG2RAD(angle)), 0 );
	VEC3 v3( 0, 0, 1 );
	MAT3 temp( v1, v2, v3 );
	*this = *this * temp;
}

double &MAT3::operator[]( const size_t index )
{
	return data[index];
}

const double &MAT3::operator[]( const size_t index ) const
{
	return data[index];
}

MAT3 MAT3::operator *( double x )
{
	return ScalarM( x );
}

MAT3 MAT3::operator /( double x )
{
	return ScalarM( 1/x );
}

MAT3 operator *( double x, MAT3 m )
{
	return m.ScalarM( x );
}

MAT3 MAT3::operator *( MAT3 &m )
{
	return multiply( m );
}

VEC3 MAT3::operator *( VEC3 &v )
{
	double temp[3] = {0,0,0};
	for( int i = 0; i < 3; i++ )
		for( int j = 0; j < 3; j++ )
			temp[i] += data[ ( j*3 ) + i ] * v[j];
	VEC3 tempV( temp[X], temp[Y], temp[Z] );
	return tempV;
}

MAT3 MAT3::operator +( MAT3 &m )
{
	MAT3 temp;
	for( int i = 0; i < 9; i++ )
		temp.data[i] = data[i] + m[i];
	return temp;
}

MAT3 MAT3::operator -( MAT3 &m )
{
	MAT3 temp;
	for( int i = 0; i < 9; i++ )
		temp.data[i] = data[i] - m[i];
	return temp;
}

MAT3::~MAT3(void)
{
}

MAT3 rotate( const double angleInDegrees, const double x, const double y, const double z )
{
	MAT3 R;
	R[0] = cos(DEG2RAD(angleInDegrees)) + (x*x)*(1-cos(DEG2RAD(angleInDegrees)));
	R[1] = y*x*(1-cos(DEG2RAD(angleInDegrees))) + z*sin(DEG2RAD(angleInDegrees));
	R[2] = z*x*(1-cos(DEG2RAD(angleInDegrees))) - y*sin(DEG2RAD(angleInDegrees));
	R[3] = x*y*(1-cos(DEG2RAD(angleInDegrees))) - z*sin(DEG2RAD(angleInDegrees));
	R[4] = cos(DEG2RAD(angleInDegrees)) + (y*y)*(1-cos(DEG2RAD(angleInDegrees)));
	R[5] = z*y*(1-cos(DEG2RAD(angleInDegrees))) + x*sin(DEG2RAD(angleInDegrees));
	R[6] = x*z*(1-cos(DEG2RAD(angleInDegrees))) + y*sin(DEG2RAD(angleInDegrees));
	R[7] = y*z*(1-cos(DEG2RAD(angleInDegrees))) - x*sin(DEG2RAD(angleInDegrees));
	R[8] = cos(DEG2RAD(angleInDegrees)) + (z*z)*(1-cos(DEG2RAD(angleInDegrees)));
	return R;
}

MAT3 rotate( const double angleInDegrees, const VEC3& v )
{
	MAT3 R = rotate( angleInDegrees, v[X], v[Y], v[Z] );
	return R;
}

MAT3 scale( double a, double b, double c )
{
	MAT3 S(a, 0, 0, 0, b, 0, 0, 0, c);
	return S;
}

MAT3 uniformScale( double s )
{
	MAT3 S(s, 0, 0, 0, s, 0, 0, 0, s);
	return S;
}

std::ostream& operator <<( std::ostream &out, const MAT3 &m )
{
	out<<std::showpoint<<std::setprecision(6);
	for( int i = 0; i < 3; i++ )
	{
		for( int j = 0; j < 3; j++ )
			out<<m[ ( j * 3 ) + i ]<<" ";
		out<<"\n";
	}
	return out;
}

void copyToFourByFourMatrix( double a[16], MAT3 &m )
{
	a[0] = m[0];
	a[1] = m[1];
	a[2] = m[2];
	a[3] = 0;
	a[4] = m[3];
	a[5] = m[4];
	a[6] = m[5];
	a[7] = 0;
	a[8] = m[6];
	a[9] = m[7];
	a[10] = m[8];
	a[11] = 0;
	a[12] = 0;
	a[13] = 0;
	a[14] = 0;
	a[15] = 1;
}

void copyToThreeByThreeMatrix( MAT3 &m, float a[16] )
{
	m[0] = a[0];
	m[1] = a[1];
	m[2] = a[2];
	m[3] = a[4];
	m[4] = a[5];
	m[5] = a[6];
	m[6] = a[8];
	m[7] = a[9];
	m[8] = a[10];
}