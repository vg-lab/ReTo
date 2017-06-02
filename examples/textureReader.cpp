/**
 * @file    textureReader.cpp
 * @brief
 * @author  Cristian Rodríguez Bernal
 * @author  Juan Guerrero Martín
 * @date    2017
 * @remarks Copyright (c) 2017 GMRV/URJC. All rights reserved.
 * Do not distribute without further notice.
 */

// std.
#include <string>
#include <glutExampleShaders.h>

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

reto::CameraController* cameraController = nullptr;

// X Y mouse position.
int previousX;
int previousY;

// States.
bool wireframe = true;
bool mouseDown = false;
bool mouseScrolling = false;
bool rotation = false;
bool traslation = false;
bool zTraslation = false;

// Constants.
const float mouseWheelFactor = 0.1f;
const float traslationScale = 0.2f;
const float traslationIncrement = 10.0f;
float rotationScale;

// Euler angles.
/**/
float currentYaw = 90.0f;
float currentPitch = 0.0f;
/**/

void renderFunc( void );
void resizeFunc( int width, int height );
void idleFunc( void );
void keyboardFunc( unsigned char key, int x, int y );
void mouseFunc( int button, int state, int x, int y );
void mouseMotionFunc( int x, int y );

void initContext( int argc, char** argv );
void initOGL( void );
void destroy( void);

MyCube* mycube;

int main( int argc, char** argv )
{
  initContext( argc, argv );
  initOGL( );

  mycube = new MyCube( 4.5f );

  cameraController = new reto::CameraController( new reto::Camera( ),
                                                 new reto::Path( ),
                                                 reto::CameraController::TProjection::PERSPECTIVE,
                                                 reto::CameraController::TCamera::STANDARD,
                                                 Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
                                                 500.0f,
                                                 5.0f, 0.01f );

  switch( cameraController->cameraType( ) )
  {
    case reto::CameraController::STANDARD :
      rotationScale = 0.1f;
      break;

    case reto::CameraController::ORBITAL :
      rotationScale = 0.3f;
      break;
  }

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

  prog.load( RETO_EXAMPLE_SHADER_TEXTURE_VERT,
              RETO_EXAMPLE_SHADER_TEXTURE_FRAG );
  prog.compileAndLink( );
  prog.autocatching( );

  glFrontFace( GL_CCW );
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );


  unsigned int texSize = 1024;
  unsigned int *data = new unsigned int[texSize * texSize * 3];
  unsigned int n = 0;
  // Generate some checker board pattern
  for (unsigned int yy = 0; yy < texSize; ++yy) {
    for (unsigned int xx = 0; xx < texSize; ++xx) {
      if ((xx + yy) / 4 % 4 == 1) {
        data[n++] = 128;
        data[n++] = 128;
        data[n++] = 128;
      }
      else {
        data[n++] = 255;
        data[n++] = 255;
        data[n++] = 255;
      }
    }
  }
  /*short texSize = 64;
  unsigned int *data = new unsigned int[texSize * texSize];
  for (short i = 0; i < texSize; ++i) {
    for (short j = 0; j < texSize; ++j) {
      data[i + j * texSize] = ((float) rand()) / (float) RAND_MAX * 255;
    }
  }*/
  reto::TextureConfig options;
  options.internalFormat = GL_RGB8;
  options.format = GL_RGB;
  options.type = GL_UNSIGNED_BYTE;
  reto::TextureManager::getInstance( ).add(
    "procedural",
    new reto::Texture2D( options, data, texSize, texSize ) );

  reto::TextureConfig opts2;
  reto::TextureManager::getInstance( ).add(
    "gmrv",
    new reto::Texture2D( opts2, RETO_EXAMPLE_TEXTURE) );
}

void destroy( void )
{
}

#define MAX 25
void renderFunc( void )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // std::cout << "DRAW" << std::endl;
  prog.use( );
  prog.sendUniform4m("proj", cameraController->camera( )->projMatrixData( ) );
  prog.sendUniform4m("view", cameraController->camera( )->viewMatrixData( ) );
  for (auto i = -MAX; i <= MAX; i+= 5) {
    for (auto j = -MAX; j <= MAX; j+= 5) {
      for (auto k = -MAX; k <= MAX; k+= 5) {
        auto modelMat_ = Eigen::Matrix4f::Identity( );
        std::vector<float> _modelVecMat;
        _modelVecMat.resize(16);

        _modelVecMat[0] = 3.5f;
        _modelVecMat[1] = modelMat_( 1, 0 );
        _modelVecMat[2] = modelMat_( 2, 0 );
        _modelVecMat[3] = modelMat_( 3, 0 );

        _modelVecMat[4] = modelMat_( 0, 1 );
        _modelVecMat[5] = 3.5f;
        _modelVecMat[6] = modelMat_( 2, 1 );
        _modelVecMat[7] = modelMat_( 3, 1 );

        _modelVecMat[8] = modelMat_( 0, 2 );
        _modelVecMat[9] = modelMat_( 1, 2 );
        _modelVecMat[10] = 3.5f;
        _modelVecMat[11] = modelMat_( 3, 2 );

        _modelVecMat[12] = i * 5;
        _modelVecMat[13] = j * 5;
        _modelVecMat[14] = k * 5;
        _modelVecMat[15] = modelMat_( 3, 3 );

        if ((i + j + k) / 4 % 4 == 1)
        {
          reto::TextureManager::getInstance( ).get("procedural")->bind( 0 );
        }
        else
        {
          reto::TextureManager::getInstance( ).get("gmrv")->bind( 0 );
        }

        prog.sendUniform4m("model", _modelVecMat.data( ));
        mycube->render( );
      }
    }
  }

  glFlush();
  glutSwapBuffers( );
}

void resizeFunc( int w, int h )
{
  cameraController->resize( w, h );
  glViewport( 0, 0, w, h );
}

void idleFunc( void )
{
  static float angle = 0.0f;
  angle = ( angle > 2.0f * float( M_PI )) ? 0 : angle + 0.01f;
  glutPostRedisplay( );
}

void keyboardFunc( unsigned char key, int, int )
{
  switch( key )
  {
    // Camera control.
    case 'c':
    case 'C':
      switch( cameraController->cameraType( ) )
      {
        case reto::CameraController::STANDARD :
          currentYaw = 90.0f;
          currentPitch = 0.0f;
          cameraController->center( Eigen::Vector3f( 0.0f, 0.0f, -500.0f ),
                                    Eigen::Vector3f( 0.0f, 1.0f, 0.0f ),
                                    Eigen::Vector3f( 0.0f, 0.0f, 1.0f ),
                                    Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
                                    500.0f, currentYaw, currentPitch );
          break;

        case reto::CameraController::ORBITAL :
          cameraController->center( Eigen::Vector3f( 0.0f, 0.0f, -500.0f ),
                                    Eigen::Vector3f( 0.0f, 1.0f, 0.0f ),
                                    Eigen::Vector3f( 0.0f, 0.0f, 1.0f ),
                                    Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
                                    500.0f, 0.0f, 0.0f );
          break;
      }
      std::cout << "Camera centered." << std::endl;
      glutPostRedisplay( );
      break;
      case 'w':
      case 'W':
        if( cameraController->projection( ) == reto::CameraController::PERSPECTIVE )
          cameraController->translateInLookAtVectorDirection( traslationIncrement );
        glutPostRedisplay( );
        break;
      case 's':
      case 'S':
      {
        if( cameraController->projection( ) == reto::CameraController::PERSPECTIVE )
          cameraController->translateInLookAtVectorDirection( -traslationIncrement );
        glutPostRedisplay( );
        break;
      }
      case 'q':
      case 'Q':
        if( cameraController->cameraType( ) == reto::CameraController::STANDARD )
          cameraController->translateInUpVectorDirection( traslationIncrement );
        glutPostRedisplay( );
        break;
      case 'e':
      case 'E':
      {
        if( cameraController->cameraType( ) == reto::CameraController::STANDARD )
          cameraController->translateInUpVectorDirection( -traslationIncrement );
        glutPostRedisplay( );
        break;
      }
      case 'a':
      case 'A':
      {
        if( cameraController->cameraType( ) == reto::CameraController::STANDARD )
          cameraController->translateInRightVectorDirection( -traslationIncrement );
        glutPostRedisplay( );
        break;
      }
      case 'd':
      case 'D':
      {
        if( cameraController->cameraType( ) == reto::CameraController::STANDARD )
          cameraController->translateInRightVectorDirection( traslationIncrement );
        glutPostRedisplay( );
        break;
      }
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
    case 't':
    case 'T':
      cameraController->toggleProjectionType( );
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
    if( button == 2 ) zTraslation = true;
    if ( (button == 3) || (button == 4) )
    {
      mouseScrolling = true;
      if( button == 3 )
        cameraController->zoom( -mouseWheelFactor );
      else
        cameraController->zoom( mouseWheelFactor );
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
    if( button == 2 ) zTraslation = false;
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

      switch( cameraController->cameraType( ) )
      {
        case reto::CameraController::STANDARD :
        {
          currentYaw += deltaX;
          currentPitch -= deltaY;

          currentYaw = fmod( currentYaw, 360.0f );
          if( currentPitch > 89.0f ) currentPitch = 89.0f;
          if( currentPitch < -89.0f ) currentPitch = -89.0f;

          cameraController->localOrientation( currentYaw, currentPitch );
        }
        break;

        case reto::CameraController::ORBITAL :
          cameraController->localRotation( deltaX, deltaY );
          break;
      }
    }
    if( traslation )
    {
      if( cameraController->cameraType( ) == reto::CameraController::STANDARD )
      {
        deltaX *= traslationScale;
        deltaY *= traslationScale;
        cameraController->translateInUpVectorDirection( deltaY );
        cameraController->translateInRightVectorDirection( -deltaX );
      }
    }
    if( zTraslation )
    {
      if( cameraController->projection( ) == reto::CameraController::PERSPECTIVE )
      {
        deltaY *= traslationScale;
        cameraController->translateInLookAtVectorDirection( deltaY );
      }
    }
    previousX = x;
    previousY = y;
    glutPostRedisplay();
  }
}
