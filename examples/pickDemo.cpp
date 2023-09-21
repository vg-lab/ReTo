/*
 * Copyright (c) 2014-2017 VG-Lab/URJC.
 *
 * Authors: Cristian Rodríguez Bernal <cristian.rodriguez@urjc.es>
 *          Juan Guerrero Martín <juan.guerrero@urjc.es>
 *
 * This file is part of ReTo <https://gitlab.vg-lab.es/nsviz/ReTo>
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

#define _USE_MATH_DEFINES
#include <math.h>

// From libraries.
#include <reto/reto.h>
using namespace reto;

#include "MyCube.h"

reto::AbstractCameraController* cameraController;
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

// Constants.

constexpr float mouseWheelFactor = 1.2f;
constexpr float rotationScale = 0.01f;
constexpr float translationScale = 0.2f;

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

  mycube = new MyCube( 4.0f );

  cameraController = new reto::OrbitalCameraController( );
  camera = cameraController->camera( );

  glutMainLoop( );
  destroy( );

  return 0;
}

int width = 500;
int height = 500;

void initContext( int argc, char** argv )
{
  glutInit( &argc,argv );
  glutInitContextVersion( 4,4 );

  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
  glutInitWindowSize( width, height );
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

reto::ShaderProgram prog, progPick;

std::vector<MyCube*> cubes;

constexpr int MAX = 25;
constexpr int STEP = 5;

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

  progPick.load( shadersPath + "pick.vert", shadersPath + "pick.frag" );
  progPick.compileAndLink( );
  progPick.autocatching( );

  glFrontFace( GL_CCW );
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

  glEnable( GL_CULL_FACE );

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

        MyCube* q = new MyCube( 4.0f );
        q->setModel(_modelVecMat);
        cubes.push_back(q);
      }
    }
  }
}

void destroy( void )
{
}

int pickX, pickY;
bool comprobar = false;

void renderFunc( void )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


  if ( comprobar )
  {
    glScissor( pickX, pickY, 1, 1 );
    glEnable(GL_SCISSOR_TEST);
  }

  if (comprobar)
  {
    progPick.use( );
    progPick.sendUniform4m("proj", camera->projectionMatrix( ));
    progPick.sendUniform4m("view", camera->viewMatrix( ));
  } else
  {
    prog.use( );
    prog.sendUniform4m("proj", camera->projectionMatrix( ));
    prog.sendUniform4m("view", camera->viewMatrix( ));
  }
  // std::cout << "DRAW" << std::endl;
  // TODO: SEND MODEL
  float id = 0.0f;

  for( auto obj: cubes )
  {
    if (comprobar)
    {
      progPick.sendUniformf("id", id);
      obj->render( &progPick );
    }
    else
    {
      prog.sendUniformf("id", id);
      obj->render( &prog );
    }
    id += 1.0f;
  }

  if ( comprobar )
  {
    int selected = -1;
    glDisable(GL_SCISSOR_TEST);

    GLubyte color[4];
    glReadPixels(pickX, pickY, 1, 1,
      GL_RGBA, GL_UNSIGNED_BYTE, color);
    int value = color[0] + color[1] * 256 + color[2] * 256 * 256;
    if (value < 3355443) {
       std::cout << value << std::endl;
    }
    std::cout << "R: " << (int)color[0] << ", G: " << (int)color[1] << ", B: " << (int)color[2] << std::endl;
    if( value < (int) id)
    {
      selected = value;
    }
    std::cout << selected << std::endl;
    comprobar = false;
  } else {
    glFlush();
    glutSwapBuffers( );
  }
}

void resizeFunc( int w, int h )
{
  width = w;
  height = h;
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
  switch( key )
  {
    // Camera control.
    case 'c':
    case 'C':
      cameraController->position( Eigen::Vector3f( 0.0f, 0.0f, 0.0f ));
      cameraController->radius( 1000.0f );
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
  // TODO: ps->click(Point{ mouseX, mouseY });
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
    if( button == 2 ) {
      printf("Click at %d, %d\n", x, height - y);
      comprobar = true;
      pickX = x;
      pickY = height - y;
    }
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
      cameraController->rotate(
        Eigen::Vector3f( deltaX * rotationScale ,
                         deltaY * rotationScale, 0.0f ));
    }
    if( translation )
    {
      cameraController->translate(
        Eigen::Vector3f ( deltaX * translationScale, -deltaY * translationScale,
                          0.0f ));
    }
    previousX = x;
    previousY = y;
    glutPostRedisplay();
  }
}
