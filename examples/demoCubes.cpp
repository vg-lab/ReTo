/*
 * Copyright (c) 2014-2016 GMRV/URJC.
 *
 * Authors: Juan Guerrero Martín
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
reto::Path* path;

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
float rotationScale;

// Euler angles.
/**/
float currentYaw = 90.0f;
float currentPitch = 0.0f;
/**/

std::vector< float > matrix4fToVector16f( const Eigen::Matrix4f& inputMatrix );

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

  path = new Path( Path::TInterpolationMethod::CATMULL_ROM );
  cameraController = new reto::CameraController( reto::CameraController::TProjection::PERSPECTIVE,
                                                 reto::CameraController::TCamera::ORBITAL,
                                                 path, 5.0f, 0.01f );

  switch( cameraController->_cameraType )
  {
    case reto::CameraController::STANDARD :
      rotationScale = 0.1f;
      break;

    case reto::CameraController::ORBITAL :
      rotationScale = 0.3f;
      break;
  }

  Eigen::Vector3f defaultCameraPosition = cameraController->_camera->position( );
  Eigen::Vector3f defaultCameraLookAt = cameraController->_camera->lookAt( );
  Eigen::Vector3f defaultCameraUp = cameraController->_camera->up( );

  // Node 0.
  path->addNode( defaultCameraPosition, defaultCameraLookAt, defaultCameraUp );

  float yOffset = 200.0f;

  // Node 1.
  path->addNode( Eigen::Vector3f( defaultCameraPosition.z( )*2.0f, yOffset, 0.0f ),
                 Eigen::Vector3f( -defaultCameraPosition.z( )*2.0f, -yOffset, 0.0f ),
                 Eigen::Vector3f( 0.0f, 1.0f, 0.0f ) );
  // Node 2.
  path->addNode( Eigen::Vector3f( defaultCameraPosition.z( ), -yOffset, -defaultCameraPosition.z( ) ),
                 Eigen::Vector3f( -defaultCameraPosition.z( ), yOffset, defaultCameraPosition.z( ) ),
                 Eigen::Vector3f( 0.0f, 1.0f, 0.0f ) );
  // Node 3.
  path->addNode( Eigen::Vector3f( 0.0f, yOffset, -defaultCameraPosition.z( )*2.0f ),
                 Eigen::Vector3f( 0.0f, -yOffset, defaultCameraPosition.z( )*2.0f ),
                 Eigen::Vector3f( 0.0f, 1.0f, 0.0f ) );
  // Node 4.
  path->addNode( Eigen::Vector3f( -defaultCameraPosition.z( ), -yOffset, 0.0f ),
                 Eigen::Vector3f( defaultCameraPosition.z( ), yOffset, 0.0f ),
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

std::vector< float > matrix4fToVector16f( const Eigen::Matrix4f& inputMatrix )
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
    {
      switch( cameraController->_cameraType )
      {
        case reto::CameraController::STANDARD :
        {
          if( !path->positions( ).empty( ) &&
              !path->lookAts( ).empty( ) &&
              !path->ups( ).empty( ) )
          {
            // We make sure that camera has path first node position and orientation.
            currentYaw = 90.0f;
            currentPitch = 0.0f;
            Eigen::Vector3f initialPosition = path->positions( ).front( );
            Eigen::Vector3f initialLookAt = path->lookAts( ).front( );
            Eigen::Vector3f initialUp = path->ups( ).front( );
            cameraController->center( initialPosition,
                                      initialLookAt,
                                      initialUp );
            // Triggering camera animation.
            cameraController->triggerAnimation( );
          }
          else
          {
            std::cerr << "A path has not been built." << std::endl;
          }
          break;
        }
        case reto::CameraController::ORBITAL :
        {
          std::cerr << "Animation for orbital camera not implemented." << std::endl;
          break;
        }
      }
      glutPostRedisplay( );
      break;
    }
    case 'w':
    case 'W':
      if( cameraController->_projection == reto::CameraController::PERSPECTIVE )
        cameraController->translateInLookAtVectorDirection( 10.0f );
      glutPostRedisplay( );
      break;
    case 's':
    case 'S':
    {
      if( cameraController->_projection == reto::CameraController::PERSPECTIVE )
        cameraController->translateInLookAtVectorDirection( -10.0f );
      glutPostRedisplay( );
      break;
    }
    case 'q':
    case 'Q':
      if( cameraController->_cameraType == reto::CameraController::STANDARD )
        cameraController->translateInUpVectorDirection( 10.0f );
      glutPostRedisplay( );
      break;
    case 'e':
    case 'E':
    {
      if( cameraController->_cameraType == reto::CameraController::STANDARD )
        cameraController->translateInUpVectorDirection( -10.0f );
      glutPostRedisplay( );
      break;
    }
    case 'a':
    case 'A':
    {
      if( cameraController->_cameraType == reto::CameraController::STANDARD )
        cameraController->translateInRightVectorDirection( -10.0f );
      glutPostRedisplay( );
      break;
    }
    case 'd':
    case 'D':
    {
      if( cameraController->_cameraType == reto::CameraController::STANDARD )
        cameraController->translateInRightVectorDirection( 10.0f );
      glutPostRedisplay( );
      break;
    }
    case 'c':
    case 'C':
      switch( cameraController->_cameraType )
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

      switch( cameraController->_cameraType )
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
      std::cout << "Please, use WASD instead." << std::endl;
    }
    previousX = x;
    previousY = y;
    glutPostRedisplay();
  }
}
