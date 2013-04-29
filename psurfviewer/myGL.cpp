#include "myGL.h"
#include <stack>

bool verboseOn = false;

/* Do not edit these variables. */
bool myMatrixModeOn;
bool myTranslateOn;
bool myRotateOn;
bool myScaleOn;
bool myLookAtOn;
bool myFrustumOn;
bool myPerspectiveOn;
bool myOrthoOn;
std::stack<MAT3> *myProjectionStack;
std::stack<MAT3> *myModelingStack;
int _myMatrixMode;

/* To toggle something on or off, change from false to true. */
void myGLInit( ){
  myMatrixModeOn = false;
  myTranslateOn = true;
  myRotateOn = true;
  myScaleOn = true;
  myLookAtOn = true;
  myFrustumOn = true;
  myPerspectiveOn = true;
  myOrthoOn = true;
  if( myMatrixModeOn ){
    myProjectionStack = new std::stack<MAT3>( );
    myModelingStack = new std::stack<MAT3>( );
    glGetIntegerv(GL_MATRIX_MODE, &_myMatrixMode);
  }
}

void myMatrixMode( GLenum mode ){
  if( myMatrixModeOn ){
    _myMatrixMode = mode;
  }else{
    glMatrixMode(mode);
  }
}

void myPushMatrix( ){
  if( myMatrixModeOn ){
    if(_myMatrixMode == GL_MODELVIEW){
		// your code here
    }else if(_myMatrixMode == GL_PROJECTION){
      // your code here
    }
  }else{
    glPushMatrix( );
  }
}

void myPopMatrix( ){
  if( myMatrixModeOn ){
    if(_myMatrixMode == GL_MODELVIEW){

    }else if(_myMatrixMode == GL_PROJECTION){

    }
  }else{
    glPopMatrix( );
  }
}

void myLoadIdentity( ){
  if( myMatrixModeOn ){
    if(_myMatrixMode == GL_MODELVIEW){
      // your code here
    }else if(_myMatrixMode == GL_PROJECTION){
      // your code here
    }
  }else{
    glLoadIdentity( );
  }
}

void myTranslatef( float x, float y, float z ){
  if( myTranslateOn ){
	  double m[16] = {1,0,0,0,0,1,0,0,0,0,1,0,x,y,z,1};
	  glMultMatrixd( m );
  }else{
    glTranslatef( x, y, z );
  }
}
		 
void myRotatef( float angle, float x, float y, float z){
  if( myRotateOn ){
    double m[16];
	MAT3 rotation = rotate( double(angle), double(x), double(y), double(z) );
	copyToFourByFourMatrix( m, rotation );
	glMultMatrixd( m );
  }else{
    glRotatef(angle, x, y, z);
  }
}
		 
void myScalef(float x, float y, float z ){
  if(myScaleOn){
    double m[16];
    MAT3 aUniformScale = uniformScale(double(x));
    // copy aUniformScale into m
    copyToFourByFourMatrix(m, aUniformScale);
    glMultMatrixd(m);
  }else{
    glScalef(x, y, z);
  }
}
		  
void myLookAt(double eyeX, double eyeY, double eyeZ, double centerX, double centerY, double centerZ, double upX, double upY, double upZ){
  if( myLookAtOn ){
    VEC3 eye( eyeX, eyeY, eyeZ );
	VEC3 center( centerX, centerY, centerZ );
	VEC3 up( upX, upY, upZ );
	VEC3 z = center - eye;
	Normalize(z);
	VEC3 x = cross( z, up );
	Normalize(x);
	VEC3 y = cross( x, z );
	double matrix[16] = { x[X], y[X], -z[X], 0,
						  x[Y], y[Y], -z[Y], 0,
						  x[Z], y[Z], -z[Z], 0,
						  0,0,0,1 };


	glMultMatrixd( matrix );
	myTranslatef( -eyeX, -eyeY, -eyeZ ); 
  }else{
    gluLookAt( eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
  }
}

void myFrustum(double left, double right, double bottom, double top, double zNear, double zFar){
  if( myFrustumOn ){
    double A = (right + left)/(right-left);
	double B = (top+bottom)/(top-bottom);
	double C = -(zFar+zNear)/(zFar-zNear);
	double D = -(2*zFar*zNear)/(zFar-zNear);
	double matrix[16] = { (2*zNear)/(right-left), 0, 0, 0,
						 0, (2*zNear)/(top-bottom), 0, 0,
						 A, B, C, -1,
						 0, 0, D, 0};
	glMultMatrixd( matrix );
  }else{
    glFrustum(left, right, bottom, top, zNear, zFar);
  }
}

void myPerspective(double fovy, double aspect, double zNear, double zFar){
  if( myPerspectiveOn ){
    double yMax, xMax;
	yMax = zNear * tanf( fovy*M_PI/360.0 );
	xMax = yMax * aspect;
	myFrustum( -xMax, xMax, -yMax, yMax, zNear, zFar );
  }else{
    gluPerspective(fovy, aspect, zNear, zFar);
  }
}

void myOrtho(double left, double right, double bottom, double top, double zNear, double zFar ){
  if( myOrthoOn ){
    double x = -(right+left)/(right-left);
	double y = -(top+bottom)/(top-bottom);
	double z = -(zFar+zNear)/(zFar-zNear);
	double matrix[16] = { 2/(right-left), 0, 0, 0,
						  0, 2/(top-bottom), 0, 0,
						  0, 0, -2/(zFar-zNear), 0,
						  x, y, z, 1};
	glMultMatrixd( matrix );
  }else{
    glOrtho(left, right, bottom, top, zNear, zFar);
  }
}


void toggleMyMatrixMode( ){
	if( verboseOn )
		printf("toggleMyMatrixMode not implemented\n");
}

void toggleMyTranslate( ){
  if( myTranslateOn )
  {
	  if( verboseOn )
		std::cout<<"myTranslate is off\n";
	  myTranslateOn = false;
  }
  else
  {
	  if( verboseOn )
		std::cout<<"myTranslate is on\n";
	  myTranslateOn = true;
  }
}

void toggleMyRotate( ){
  if( myRotateOn )
  {
	  if( verboseOn )
		std::cout<<"myRotate is off\n";
	  myRotateOn = false;
  }
  else
  {
	  if( verboseOn )
		 std::cout<<"myRotate is on\n";
	  myRotateOn = true;
  }
}

void toggleMyScale( ){
  if( myScaleOn )
  {
	  if( verboseOn )
		std::cout<<"myScale is off\n";
	  myScaleOn = false;
  }
  else
  {
	  if( verboseOn )
		std::cout<<"myScale is on\n";
	  myScaleOn = true;
  }
}

void toggleMyLookAt( ){
  if( myLookAtOn )
  {
	  if( verboseOn )
		std::cout<<"myLookat is off\n";
	  myLookAtOn = false;
  }
  else
  {
	  if( verboseOn )
		std::cout<<"myLookat is on\n";
	  myLookAtOn = true;
  }
}

void toggleMyFrustum( ){
	if( myFrustumOn )
	{
	  if( verboseOn )
		std::cout<<"myFrustum is off\n";
	  myFrustumOn = false;
	 }
	else
	{
	  if( verboseOn )
		std::cout<<"myFrustum is on\n";
	  myFrustumOn = true;
	}
}

void toggleMyPerspective( ){
  if( myPerspectiveOn )
  {
	  if( verboseOn )
		std::cout<<"myPerspective is off\n";
	  myPerspectiveOn = false;
  }
  else
  {
	  if( verboseOn )
		std::cout<<"myPerspective is on\n";
	  myPerspectiveOn = true;
  }
}

void toggleMyOrtho( ){
  if( myOrthoOn )
  {
	  if( verboseOn )
		std::cout<<"myOrtho is off\n";
	  myOrthoOn = false;
  }
  else
  {
	  if( verboseOn )
		std::cout<<"myOrtho is on\n";
	  myOrthoOn = true;
  }
}


void toggleVerbose( )
{
	verboseOn = true;
}