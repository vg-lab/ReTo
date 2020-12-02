/*
 * Copyright (c) 2014-2017 GMRV/URJC.
 *
 * Authors: Cristian Rodríguez Bernal <cristian.rodriguez@urjc.es>
 *          Juan Guerrero Martín <juan.guerrero@urjc.es>
 *
 * This file is part of ReTo <https://gitlab.gmrv.es/nsviz/ReTo>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <string>

// OpenGL, GLEW, GLUT
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

#include <chrono>

#define _USE_MATH_DEFINES
#include <math.h>

// Eigen.
#include <Eigen/Dense>
using Eigen::MatrixXf;
using Eigen::VectorXf;

// From libraries.
#include <reto/reto.h>
using namespace reto;

#include "MyCube.h"

reto::AbstractCameraController* cameraController;
reto::AbstractCameraController* cControllers[2];
unsigned int ccNumber = 0;
reto::Camera* camera;

// X Y mouse position.
int previousX;
int previousY;

// States.
bool wireframe = false;
bool mouseDown = false;
bool mouseScrolling = false;
bool rotation = false;
bool translation = false;
bool animation = false;

// Constants.

constexpr float mouseWheelFactor = 1.2f;
constexpr float rotationScale = 0.001f;
constexpr float translationScale = 0.2f;

std::chrono::time_point< std::chrono::system_clock > _previousTime;

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


  camera = new reto::Camera( );
  cControllers[1] = new reto::FreeCameraController( camera );
  cControllers[1]->position( Eigen::Vector3f( 0.0f, 0.0f, 100.0f ));
  cControllers[0] = new reto::OrbitalCameraController( camera );
  cControllers[0]->radius( 100.0f );
  cameraController = cControllers[0];

  _previousTime = std::chrono::system_clock::now( );

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

  const auto path = std::getenv( "RETO_SHADERS_PATH" );
  if ( !path )
  {
    const std::string message("Error: RETO_SHADERS_PATH environment variable not found.");
    std::cerr << message << std::endl;
    throw std::runtime_error( message );
  }

  const auto shadersPath = std::string( path ) + std::string( "/" );

  prog.load( shadersPath + "color.vert", shadersPath + "color.frag" );
  prog.compileAndLink( );
  prog.autocatching( );

  glFrontFace( GL_CCW );
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void destroy( void )
{
}

constexpr int MAX = 25;
constexpr int STEP = 25;

void renderFunc( void )
{

  auto currentTime = std::chrono::system_clock::now( );
  auto duration = std::chrono::duration_cast< std::chrono::microseconds >
    ( currentTime - _previousTime );
  float dt = (( float ) duration.count( )) * 0.000001f;
  cameraController->anim( dt );
  _previousTime = currentTime;
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  prog.use( );
  prog.sendUniform4m("proj", camera->projectionMatrix( ));
  prog.sendUniform4m("view", camera->viewMatrix( ));

  for (auto i = -MAX; i <= MAX; i+= STEP)
  {
    for (auto j = -MAX; j <= MAX; j+= STEP)
    {
      for (auto k = -MAX; k <= MAX; k+= STEP)
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
  cameraController->windowSize( width, height );
  glViewport( 0, 0, width, height );
}

void idleFunc( void )
{
  static float angle = 0.0f;
  angle = ( angle > 2.0f * float( M_PI )) ? 0 : angle + 0.01f;
  glutPostRedisplay( );
}

void keyboardFunc( unsigned char key, int, int )
{
  CameraAnimation* camAnim;
  switch( key )
  {
    // Camera control.
  case 'p':
  case 'P':
    cameraController->stopAnim( );
    break;
  case 'o':
  case 'O':
    camAnim = new CameraAnimation( CameraAnimation::LINEAR,
                                   CameraAnimation::LINEAR,
                                   CameraAnimation::LINEAR );
    camAnim->addKeyCamera(
      new KeyCamera( 5.0f, Eigen::Vector3f( 0.0f, 100.0f, 0.0f ),
                     Eigen::Vector3f( 2.0f, 0.0f, 0.0f ), 1000.0f ));
    camAnim->addKeyCamera(
      new KeyCamera( 0.0f, Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
                     Eigen::Vector3f( 0.0f, 0.0f, 0.0f ), 100.0f ));
    camAnim->addKeyCamera(
      new KeyCamera( 10.0f, Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
                     Eigen::Vector3f( 0.0f, 0.0f, 0.0f ), 100.0f ));
    cameraController->startAnim( camAnim, true );
    break;
  case 'w':
  case 'W':
    cameraController->localTranslate( Eigen::Vector3f( 0.0f, 0.0f, -10.0f ) *
                                      translationScale );
    break;
  case 'a':
  case 'A':
    cameraController->localTranslate( Eigen::Vector3f( -10.0f, 0.0f, 0.0f ) *
                                      translationScale );
    break;
  case 's':
  case 'S':
    cameraController->localTranslate( Eigen::Vector3f( 0.0f, 0.0f, 10.0f ) *
                                      translationScale );
    break;
  case 'd':
  case 'D':
    cameraController->localTranslate( Eigen::Vector3f( 10.0f, 0.0f, 0.0f ) *
                                      translationScale );
    break;
  case 'c':
  case 'C':
    ccNumber++;
    ccNumber %= 2;
    cameraController = cControllers[ccNumber];
    cameraController->update( );
    break;
  case 'r':
  case 'R':
    cameraController->position( Eigen::Vector3f( 0.0f, 0.0f, 0.0f ));
    cameraController->radius( 100.0f );
    cameraController->rotation( Eigen::Vector3f( 0.0f, 0.0f, 0.0f ));
    std::cout << "Centering." << std::endl;
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
    if( button == 1 ) translation = true;
    if ( button == 2 ) animation = true;
    if ( (button == 3) || (button == 4) )
    {
      //std::cout << "Scrolling." << std::endl;
      mouseScrolling = true;
      float newRadius = ( button == 3 ) ?
                        cameraController->radius() / mouseWheelFactor :
                        cameraController->radius() * mouseWheelFactor;
      cameraController->radius( newRadius );
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
    if( button == 1 ) translation = false;
    if ( button == 2 )
    {
      float deltaX = x - previousX;
      float deltaY = y - previousY;
      Eigen::Vector3f targetPos = cameraController->position( ) +
        Eigen::Vector3f( -deltaX * translationScale,
                         deltaY * translationScale,
                         0.0f );
      Eigen::Matrix3f targetRot =
        cameraController->rotationMatrixFromAngles(
          Eigen::Vector3f( -deltaX * rotationScale,
                           deltaY * rotationScale,
                           0.0f )) *
        cameraController->rotation( );

      auto camAnim = new CameraAnimation( CameraAnimation::NONE,
                                          CameraAnimation::LINEAR );
      camAnim->addKeyCamera(
        new KeyCamera( 0.0f, cameraController->position( ),
                       cameraController->rotation( ),
                       cameraController->radius( )));
      camAnim->addKeyCamera(
        new KeyCamera( 2.0f, targetPos, targetRot,
                       cameraController->radius( )));
      cameraController->startAnim( camAnim, false );
    }
    if ( (button == 3) || (button == 4) )
    {
      mouseScrolling = false;
    }
  }
}

void mouseMotionFunc( int x, int y )
{
  if( mouseDown & ( rotation | translation ))
  {
    float deltaX = x - previousX;
    float deltaY = y - previousY;
    if( rotation )
    {
      cameraController->rotate(
        Eigen::Vector3f( deltaX * rotationScale ,
                         deltaY * rotationScale, 0.0f ));
    }
    if( translation )
    {
      cameraController->localTranslate(
        Eigen::Vector3f ( deltaX * translationScale, -deltaY * translationScale,
                          0.0f ));
    }

    previousX = x;
    previousY = y;
    glutPostRedisplay();
  }
}
