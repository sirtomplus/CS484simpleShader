
#include <cstdlib>

#ifdef __APPLE__
/* Apple's weird location of their OpenGL & GLUT implementation */
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#endif

#include <cstdio>
#include <string>

/* Thanks to Apple's Sample Code... */
#include "SurfaceGeometry.h"
#include "myGL.h"
#include "SGFaceList.h"
#include "getopt.h"

using std::cout;
using std::endl;

SGFaceList *gModel[kSurfaces];
unsigned int gCurrentModel;

VEC3 lightSource(1.25, 1.25, .5);


float gTranslation[3], gRotationAngle[3], gScaleFactor[3];

int wHeight = 800;			//window height
int wWidth = 800;			//window width

//For mouse translation
float ORIGIN[3];
int mouseInitialPos[3];
int mouseDest[3];
float DELTA[3];
int mouseMod;

bool Ortho = false;			//Turn myOrtho on/off

void printVector(float v[3]){
  printf( "(%g, %g, %g)\n", v[0], v[1], v[2] );
}

void usage( std::string msg ){
  std::cerr << msg << std::endl;
  std::cerr << "usage: psurfviewer [-v -h]" << std::endl;
  std::cerr << "                 or" << std::endl;
  std::cerr << "       psurfviewer [--verbose --help]" << std::endl;
}

/*  Initialize material property, light source, lighting model,
 *  and depth buffer.
 */
void init( void ) 
{
   glClearColor( 0.0, 0.0, 0.0, 0.0 );
   glDisable( GL_LIGHTING );
   glEnable( GL_DEPTH_TEST );
   glFrontFace( GL_CCW );
 	
   for( int i = 0; i < kSurfaces; i++ ){
     gModel[i] = BuildGeometry( i, 4, 8, 3 );     
   }
   gCurrentModel = 0;
   for( int i = 0; i < 3; i++ ){
     gRotationAngle[i] = 0.0f;
     gTranslation[i] = 0.0f;
     gScaleFactor[i] = 1.0f;
	 ORIGIN[i] = 0.0;
   }
}


void myShader( VEC3& L, VEC3& N, VEC3& V, VEC3& color )
{
	VEC3 Light = L - V;
	Normalize(Light);

	VEC3 R = (2*N) * (N.dot(Light)) - Light;
	Normalize( R );

	VEC3 eye = VEC3( 0, 0, -15 ) - V;
	Normalize( eye );

	float costheta = R.dot( eye );

	VEC3 W( 1, 1, 1 );
	VEC3 P( 1, 2, 20 );
	color[0] = W[0] * powf(((costheta+1)/2), P[0] );
	color[1] = W[1] * powf(((costheta+1)/2), P[1] );
	color[2] = W[2] * powf(((costheta+1)/2), P[2] );
}


void resetObject( )
{
	for( int i = 0; i < 3; ++i )
	{
		gScaleFactor[i] = 1.0f;
		gRotationAngle[i] = 0;
		gTranslation[i] = 0;
		DELTA[i] = 0;
		ORIGIN[i] = 0;
	}
}

void drawSurface( SGFaceList *f ){
  int i = 0;
  int j = 0;
  int index;
  VEC3 color;
  if( f->surface == kCube ){
    glEnable( GL_POLYGON_OFFSET_FILL );
    glBegin( GL_TRIANGLES );
	for( i = 0; i < f->fc; i++ ){
		for( j = 0; j < 3; j++ ){
			glColor3fv( f->colors[ f->faces[i][j] ] );
			glVertex3fv( f->vertices[ f->faces[i][j] ] );
		}
	}
    glEnd( );
    glColor3f( 0.0, 0.0, 0.0 );
    for( i = 0; i < (f->fc); i++ ){
      glBegin( GL_LINE_LOOP );
      for( j = 0; j < 3; j++ ){
        glVertex3fv( f->vertices[ f->faces[i][j] ] );
      }
      glEnd( );
    }
    glDisable( GL_POLYGON_OFFSET_LINE );
  }else{
    for( i = 0; i < (f->maxI); i++ ){
      glBegin( GL_TRIANGLE_STRIP );
		for( j = 0; j <= (f->maxJ); j++ ){
			index = (i % (f->maxI)) * (f->maxJ) + (j % (f->maxJ));
			myShader( lightSource, VEC3( f->v_normals[index] ), VEC3( f->vertices[index] ), color );
			glColor3f( color[0], color[1], color[2] );
//			glColor3fv( f->colors[index] );
//			glNormal3fv( f->v_normals[index] );
			glVertex3fv( f->vertices[index] );
        
			index = ((i + 1) % f->maxI) * f->maxJ + (j % f->maxJ);
			myShader( lightSource, VEC3( f->v_normals[index] ), VEC3( f->vertices[index] ), color );
			glColor3f( color[0], color[1], color[2] );
//        glColor3fv( f->colors[index] );
//			glNormal3fv( f->v_normals[index] );
			glVertex3fv( f->vertices[index] );
		}
      glEnd( );
    }
  }
}

void display( void ){
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity( );
   myLookAt( 0.0, 0.0, -15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
   myRotatef( gRotationAngle[X], 1.0, 0.0, 0.0 );
   myRotatef( gRotationAngle[Y], 0.0, 1.0, 0.0 );
   myRotatef( gRotationAngle[Z], 0.0, 0.0, 1.0 );
   myScalef( gScaleFactor[X], gScaleFactor[Y], gScaleFactor[Z] );
   myTranslatef( gTranslation[X], gTranslation[Y], gTranslation[Z] );
   //glutSolidTorus (0.5, 1.0, 32, 60);
   drawSurface( gModel[gCurrentModel] );
   glFlush ();
   glutSwapBuffers();
}

void reshape( int w, int h ){
   glViewport( 0, 0, (GLsizei) w, (GLsizei) h );
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity( );
//   myPerspective( 40.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0 );
   myFrustum( -.36397, .36397, -.36397, .36397, 1, 20 );
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity( );
}


//Turn myOrtho on/off
void ToggleOrtho()
{
	if( Ortho )
		Ortho = false;
	else
		Ortho = true;
}


void mouse( int button, int state, int x, int y ) {
  if( glutGetModifiers( ) == GLUT_ACTIVE_SHIFT ){
	   switch (button) {
       case GLUT_LEFT_BUTTON:
		   mouseMod = 2;
		   mouseInitialPos[X] = x;
		   mouseInitialPos[Z] = y;
         break;
       case GLUT_MIDDLE_BUTTON:
         // Fall through
       case GLUT_RIGHT_BUTTON:
         break;
       default:
         // You have more than three buttons on your mouse?
         break;
    }
	}else if( glutGetModifiers( ) == GLUT_ACTIVE_CTRL ){
     switch (button) {
       case GLUT_LEFT_BUTTON:
		   mouseMod = 1;
		   mouseInitialPos[Z] = y;
         break;
       case GLUT_MIDDLE_BUTTON:
         // Fall through
       case GLUT_RIGHT_BUTTON:
         break;
       default:
         // You have more than three buttons on your mouse?
         break;
    }
  }else{
    switch (button) {
      case GLUT_LEFT_BUTTON:
		  mouseMod = 0;
		  mouseInitialPos[X] = x;
		  mouseInitialPos[Y] = y;
        break;
      case GLUT_MIDDLE_BUTTON:
        // Fall through
      case GLUT_RIGHT_BUTTON:
        break;
      default:
        // You have more than three buttons on your mouse?
        break;
   }
 }
 glutPostRedisplay( );
}


void mouseMotion(int x, int y){
	if( mouseMod == 0 )
	{
		//No modifier
		mouseDest[X] = x;
		mouseDest[Y] = y;
		DELTA[Y] = mouseInitialPos[Y] - mouseDest[Y];
		DELTA[X] = mouseInitialPos[X] - mouseDest[X];
		gTranslation[X] = (ORIGIN[X] - DELTA[X])/ ((float)wWidth/4);
		gTranslation[Y] = (ORIGIN[Y] - DELTA[Y])/ ((float)wHeight/4);
		ORIGIN[X] += DELTA[X];
		ORIGIN[Y] += DELTA[Y];
		mouseInitialPos[X] = x;
		mouseInitialPos[Y] = y;
	}
	else if( mouseMod == 1 )
	{
		//Ctrl
		mouseDest[Z] = y;
		DELTA[Z] = mouseInitialPos[Z] - mouseDest[Z];
		gTranslation[Z] = (ORIGIN[Z] - DELTA[Z]) / ((float)wWidth/4);
		ORIGIN[Z] += DELTA[Z];
		mouseInitialPos[Z] = y;
	}
	else if( mouseMod == 2 )
	{
		//Shift
		mouseDest[X] = x;
		mouseDest[Z] = y;
		DELTA[Z] = mouseInitialPos[Z] - mouseDest[Z];
		DELTA[X] = mouseInitialPos[X] - mouseDest[X];
		gTranslation[X] = (ORIGIN[X] - DELTA[X])/((float)wWidth/4);
		gTranslation[Z] = (ORIGIN[Z] - DELTA[Z])/((float)wHeight/4);
		ORIGIN[X] += DELTA[X];
		ORIGIN[Z] += DELTA[Z];
		mouseInitialPos[X] = x;
		mouseInitialPos[Z] = y;
	}
  glutPostRedisplay( );
}

void special( int key, int px, int py ){
	if( glutGetModifiers( ) == GLUT_ACTIVE_SHIFT ){
		switch( key ){
		case GLUT_KEY_UP:
			gRotationAngle[Z] += .5;
			break;
		case GLUT_KEY_DOWN:
			gRotationAngle[Z] += -.5;
			break;
	  }
	}else{
	  switch( key ){
		case GLUT_KEY_UP:
			gRotationAngle[X] += .5;
			break;
		case GLUT_KEY_DOWN:
			gRotationAngle[X] += -.5;
			break;
		case GLUT_KEY_LEFT:
			gRotationAngle[Y] += -.5;
			break;
		case GLUT_KEY_RIGHT:
			gRotationAngle[Y] += .5;
			break;
	  }
	}
  glutPostRedisplay( );
}

void keyboard( unsigned char key, int x, int y ){
  switch( key ){
    case 27:
      // The esc key
      exit( 0 );
    break;
    case '+':
		gScaleFactor[X] += .25;
		gScaleFactor[Y] += .25;
		gScaleFactor[Z] += .25;
    break;
    case '-':
		gScaleFactor[X] += -.25;
		gScaleFactor[Y] += -.25;
		gScaleFactor[Z] += -.25;
    break;
    case ' ':
      gCurrentModel = (gCurrentModel + 1) % kSurfaces;
    break;
	case '1':
		toggleMyMatrixMode();
	break;
	case '2':
		toggleMyTranslate();
	break;
	case '3':
		toggleMyRotate();
	break;
	case '4':
		toggleMyScale();
	break;
	case '5':
		toggleMyLookAt();
	break;
	case '6':
		toggleMyFrustum();
	break;
	case '7':
		toggleMyPerspective();
	break;
	case '8':
		toggleMyOrtho();
	break;
	case 'r':					//Move object back to starting position
		resetObject( );
	break;
	case 'p':
		ToggleOrtho();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity( );
		if(Ortho == true)
			myOrtho(-4.73, 4.73, -4.73, 4.73, 10.5, 30);
		else
//			myPerspective(40.0, 1.0, 1.0, 20.0 );
			myFrustum( -.36397, .36397, -.36397, .36397, 1, 20 );
			
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity( );
	break;
    default:
		std::cerr << "Unknown key press \'" << key << "\'" << std::endl;
    break;
  }
  glutPostRedisplay( );
}

int main(int argc, char** argv){
  int ch;
  struct option longopts[] = {
    { "verbose", required_argument, NULL, 'v' },
    { "help", required_argument, NULL, 'h' },
    { NULL, 0, NULL, 0 }
  };
  
  // Let GLUT process the command line first
  glutInit(&argc, argv);

  // Let's figure out what the command line options are
  while( (ch = getopt_long(argc, argv, "vh", longopts, NULL)) != -1 ){
    switch( ch ){
      case 'v':
        // Turn on debugging
		  toggleVerbose( );
      break;
      case 'h':
        usage( std::string( "Hope this helps..." ) );
      break;
      default:
        /* do nothing */
        fprintf( stderr, "Ignoring unknown option: %c\n", ch );
      break;
    } 
  }

  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
  glutInitWindowSize( wWidth, wHeight ); 
  glutInitWindowPosition( 100, 100 );
  glutCreateWindow( argv[0] );

  // Initialize our OpenGL environment
  init( );
  myGLInit();

 /*
   * Set up our call back functions to handle events
   */
  // Handle window size changes
  glutReshapeFunc( reshape );
  // Handle keyboard events
  glutKeyboardFunc( keyboard );
  glutSpecialFunc( special );
  // display gets called every time we want to refresh the screen
  glutDisplayFunc( display );
  // handle user input via the mouse
  glutMouseFunc( mouse );
  glutMotionFunc( mouseMotion );

  // The program doesn't return from the next call.
  glutMainLoop( );

  return( 0 );
}
