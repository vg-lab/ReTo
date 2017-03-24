/**
 * @file    demoCubes.cpp
 * @brief
 * @author  Juan Guerrero Martín <juan.guerrero@urjc.es>
 * @date    2016
 * @remarks Copyright (c) 2016 GMRV/URJC. All rights reserved.
 * Do not distribute without further notice.
 */

// std.
#include <string>
#include <glutExampleShaders.h>
#include <math.h>
#include <vector>

// OpenGL, GLEW, GLUT.
#include <GL/glew.h>

#ifdef Darwin
  #define __gl_h_
  #define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <GL/freeglut.h>
#else
  #include <GL/gl.h>
  #include <GL/freeglut.h>
#endif

// Eigen.
#include <Eigen/Dense>
using Eigen::MatrixXf;
using Eigen::VectorXf;

// From libraries.
#include <reto/reto.h>
using namespace reto;

#include "MyCube.h"

reto::CameraController* cameraController;

unsigned int idleTimeCounter = 0;

// X Y mouse position.
int previousX;
int previousY;

// States.
bool wireframe = true;
bool mouseDown = false;
bool mouseScrolling = false;
bool rotation = false;
bool traslation = false;

// Constants.
const float mouseWheelFactor = 1.2f;
// lookAt.
// const float rotationScale = 0.1f;
// orientation.
const float rotationScale = 0.01f;
const float traslationScale = 0.2f;

// Euler angles.
/**
float currentYaw = -90.0f;
float currentPitch = 0.0f;
**/

std::vector< float > matrix4fToVector16f( Eigen::Matrix4f inputMatrix );

void renderFunc( void );
void resizeFunc( int width, int height );
void idleFunc( void );
void keyboardFunc( unsigned char key, int x, int y );
void mouseFunc( int button, int state, int x, int y );
void mouseMotionFunc( int x, int y );

void initContext( int argc, char** argv );
void initOGL( void );
void destroy( void );

MyCube* mycube;

int main( int argc, char** argv )
{
  initContext( argc, argv );
  initOGL( );

  mycube = new MyCube( 4.5f );

  cameraController = new reto::CameraController( reto::CameraController::TProjection::PERSPECTIVE,
                                                 reto::CameraController::TCamera::STANDARD );

  Eigen::Vector3f defaultCameraPosition = cameraController->_camera->position( );
  Eigen::Vector3f defaultCameraLookAt = cameraController->_camera->lookAt( );
  Eigen::Vector3f defaultCameraUp = cameraController->_camera->up( );

  Path* path = new Path( Path::TInterpolationMethod::CATMULL_ROM );

  path->addNode( defaultCameraPosition, defaultCameraLookAt, defaultCameraUp );

  // Node 1.
  path->addNode( Eigen::Vector3f( -defaultCameraPosition.z( ), 250.0f, 0.0f ),
                 Eigen::Vector3f( defaultCameraPosition.z( ), -250.0f, 0.0f ),
                 Eigen::Vector3f( 0.0f, 1.0f, 0.0f ) );

  // Node 2.
  path->addNode( Eigen::Vector3f( 0.0f, 0.0f, -defaultCameraPosition.z( ) ),
                 Eigen::Vector3f( 0.0f, 0.0f, defaultCameraPosition.z( ) ),
                 Eigen::Vector3f( 0.0f, 1.0f, 0.0f ) );

  // Node 3.
  path->addNode( Eigen::Vector3f( defaultCameraPosition.z( ), -250.0f, 0.0f ),
                 Eigen::Vector3f( -defaultCameraPosition.z( ), 250.0f, 0.0f ),
                 Eigen::Vector3f( 0.0f, 1.0f, 0.0f ) );

  cameraController->path( path );

  glutMainLoop( );
  destroy( );

  return 0;
}

void initContext( int argc, char** argv )
{
  glutInit( &argc,argv );
  glutInitContextVersion( 4,4 );

  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
  glutInitWindowSize( 500, 500 );
  glutInitWindowPosition( 0, 0 );
  glutCreateWindow( "GLUT example" );

  glewExperimental = GL_TRUE;
  GLenum err = glewInit( );
  if ( GLEW_OK != err ) {
    std::cout << "Error: " << glewGetErrorString( err ) << std::endl;
    exit ( -1 );
  }
  const GLubyte *oglVersion = glGetString( GL_VERSION );
  std::cout << "This system supports OpenGL Version: "
            << oglVersion << std::endl;

  glutReshapeFunc( resizeFunc );
  glutDisplayFunc( renderFunc );
  glutIdleFunc( idleFunc );
  glutKeyboardFunc( keyboardFunc );
  glutMouseFunc( mouseFunc );
  glutMotionFunc( mouseMotionFunc );
}

reto::ShaderProgram prog;

void initOGL( void )
{
  glEnable( GL_DEPTH_TEST );
  glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

  prog.load( RETO_EXAMPLE_SHADER_COLOR_VERT,
             RETO_EXAMPLE_SHADER_COLOR_FRAG );
  prog.compileAndLink( );
  prog.autocatching( );

  glFrontFace( GL_CCW );
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void destroy( void )
{
}

std::vector< float > matrix4fToVector16f( Eigen::Matrix4f inputMatrix )
{
  std::vector< float > toReturn;

  toReturn.push_back( inputMatrix( 0, 0 ) );
  toReturn.push_back( inputMatrix( 1, 0 ) );
  toReturn.push_back( inputMatrix( 2, 0 ) );
  toReturn.push_back( inputMatrix( 3, 0 ) );

  toReturn.push_back( inputMatrix( 0, 1 ) );
  toReturn.push_back( inputMatrix( 1, 1 ) );
  toReturn.push_back( inputMatrix( 2, 1 ) );
  toReturn.push_back( inputMatrix( 3, 1 ) );

  toReturn.push_back( inputMatrix( 0, 2 ) );
  toReturn.push_back( inputMatrix( 1, 2 ) );
  toReturn.push_back( inputMatrix( 2, 2 ) );
  toReturn.push_back( inputMatrix( 3, 2 ) );

  toReturn.push_back( inputMatrix( 0, 3 ) );
  toReturn.push_back( inputMatrix( 1, 3 ) );
  toReturn.push_back( inputMatrix( 2, 3 ) );
  toReturn.push_back( inputMatrix( 3, 3 ) );

  return toReturn;
}


#define MAX 25
void renderFunc( void )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // std::cout << "DRAW" << std::endl;
  prog.use( );
  prog.sendUniform4m("proj", matrix4fToVector16f( cameraController->_camera->projMatrix( ) ));
  prog.sendUniform4m("view", matrix4fToVector16f( cameraController->_camera->viewMatrix( ) ));
  for (auto i = -MAX; i <= MAX; i+= 5)
  {
    for (auto j = -MAX; j <= MAX; j+= 5)
    {
      for (auto k = -MAX; k <= MAX; k+= 5)
      {
        auto modelMat_ = Eigen::Matrix4f::Identity( );
        std::vector<float> _modelVecMat;
        _modelVecMat.resize(16);

        _modelVecMat[0] = modelMat_( 0, 0 );
        _modelVecMat[1] = modelMat_( 1, 0 );
        _modelVecMat[2] = modelMat_( 2, 0 );
        _modelVecMat[3] = modelMat_( 3, 0 );

        _modelVecMat[4] = modelMat_( 0, 1 );
        _modelVecMat[5] = modelMat_( 1, 1 );
        _modelVecMat[6] = modelMat_( 2, 1 );
        _modelVecMat[7] = modelMat_( 3, 1 );

        _modelVecMat[8] = modelMat_( 0, 2 );
        _modelVecMat[9] = modelMat_( 1, 2 );
        _modelVecMat[10] = modelMat_( 2, 2 );
        _modelVecMat[11] = modelMat_( 3, 2 );

        _modelVecMat[12] = i;
        _modelVecMat[13] = j;
        _modelVecMat[14] = k;
        _modelVecMat[15] = modelMat_( 3, 3 );

        prog.sendUniform4m("model", _modelVecMat.data( ));
        mycube->render( );
      }
    }
  }

  glFlush();
  glutSwapBuffers( );
}

void resizeFunc( int width, int height )
{
  cameraController->resize( width, height );
  glViewport( 0, 0, width, height );
}

void idleFunc( void )
{
  //static float angle = 0.0f;
  //angle = ( angle > 2.0f * float( M_PI )) ? 0 : angle + 0.01f;
  if( idleTimeCounter > 5 && cameraController->animate( ) )
  {
    idleTimeCounter = 0;
  }
  idleTimeCounter++;
  glutPostRedisplay( );
}

void keyboardFunc( unsigned char key, int, int )
{
  switch( key )
  {
    // Camera control.
    case 'i':
    case 'I':
      cameraController->triggerAnimation( );
      glutPostRedisplay( );
      break;

    case 'w':
    case 'W':
      cameraController->moveUsingLookAtVector( 10.0f );
      //cameraController->localTranslation( Eigen::Vector3f( 0.0f, 0.0f, 10.0f ) );
      glutPostRedisplay( );
      break;
    case 's':
    case 'S':
    {
      cameraController->moveUsingLookAtVector( -10.0f );
      //cameraController->localTranslation( Eigen::Vector3f( 0.0f, 0.0f, -10.0f ) );
      glutPostRedisplay( );
      break;
    }
    case 'a':
    case 'A':
    {
      cameraController->moveUsingTangentVector( -10.0f );
      //cameraController->localTranslation( Eigen::Vector3f( 10.0f, 0.0f, 0.0f ) );
      glutPostRedisplay( );
      break;
    }
    case 'd':
    case 'D':
    {
      cameraController->moveUsingTangentVector( 10.0f );
      //cameraController->localTranslation( Eigen::Vector3f( -10.0f, 0.0f, 0.0f ) );
      glutPostRedisplay( );
      break;
    }
    case 'c':
    case 'C':
      cameraController->center( Eigen::Vector3f( 0.0f, 0.0f, -500.0f ) );
      std::cout << "Camera centered." << std::endl;
      glutPostRedisplay( );
      break;
    case 'm':
    case 'M':
      wireframe = !wireframe;
      if ( wireframe )
      {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        std::cout << "Wireframe ON." << std::endl;
      }
      else
      {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        std::cout << "Wireframe OFF." << std::endl;
      }
      glutPostRedisplay( );
      break;
  }
}

void mouseFunc( int button, int state, int x, int y )
{
  /**
   * GLUT
   * button: 0 (left), 1 (central), 2 (right), 3 (wheel up), 4 (wheel down).
   * state: GLUT_DOWN (button down), GLUT_UP (button released).
   */
  if ( state == GLUT_DOWN )
  {
    mouseDown = true;
    if( button == 0 ) rotation = true;
    if( button == 1 ) traslation = true;
    if ( (button == 3) || (button == 4) )
    {
      //std::cout << "Scrolling." << std::endl;
      mouseScrolling = true;
      if( button == 3 )
        cameraController->zoom( -0.1f );
      else
        cameraController->zoom( 0.1f );
      glutPostRedisplay();
    }
    // We save X and Y previous positions.
    previousX = x;
    previousY = y;
  }
  else
  {
    mouseDown = false;
    if( button == 0 ) rotation = false;
    if( button == 1 ) traslation = false;
    if ( (button == 3) || (button == 4) )
    {
      mouseScrolling = false;
    }
  }
}

void mouseMotionFunc( int x, int y )
{
  if( mouseDown )
  {
    float deltaX = x - previousX;
    float deltaY = y - previousY;
    if( rotation )
    {
      deltaX *= rotationScale;
      deltaY *= rotationScale;

      /**/
      cameraController->localRotation( deltaX, deltaY );
      /**/

      /**
      currentYaw -= deltaX;
      currentPitch += deltaY;

      if( currentPitch > 89.0f ) currentPitch = 89.0f;
      if( currentPitch < -89.0f ) currentPitch = -89.0f;

      // To radians.
      currentYaw *= (M_PI/180.0f);
      currentPitch *= (M_PI/180.0f);

      cameraController->localRotation( currentYaw, currentPitch );
      **/
    }
    if( traslation )
    {
      std::cout << "Please use WASD instead." << std::endl;
      //cameraController->localTranslation( Eigen::Vector3f ( -deltaX * traslationScale,
      //                                                      0.0f,
      //                                                      deltaY * traslationScale ) );
    }
    previousX = x;
    previousY = y;
    glutPostRedisplay();
  }
}
