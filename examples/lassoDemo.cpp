/*
 * Copyright (c) 2014-2019 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
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
#define _USE_MATH_DEFINES
#include <math.h>

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

reto::Camera* camera;

// X Y mouse position.
int previousX;
int previousY;

// States.
bool wireframe = true;
int mouseButton = -1;

// Constants.
const float mouseWheelFactor = 1.2f;
const float rotationScale = 0.01f;
const float traslationScale = 0.2f;

void renderFunc( void );
void resizeFunc( int width, int height );
void idleFunc( void );
void keyboardFunc( unsigned char key, int x, int y );
void mouseFunc( int button, int state, int x, int y );
void mouseMotionFunc( int x, int y );

void initContext( int argc, char** argv );
void initOGL( void );
void destroy( void);

void createCubes( void );

reto::ShaderProgram prog;
reto::SelectionSystem::Lasso* lasso;

unsigned int width = 500;
unsigned int height = 500;

//Cubes
std::vector<MyCube*> cubes;
int MAX = 25;
int STEP = 5;

int main( int argc, char** argv )
{
  initContext( argc, argv );
  initOGL( );

  camera = new reto::Camera( );

  lasso = new reto::SelectionSystem::Lasso( width, height );
  lasso->setColor( Eigen::Vector4f( 0.0f, 0.5f, 1.0f, 0.5f ) );
  lasso->setLineWidth( 2.0f );

  createCubes( );

  glutMainLoop( );
  destroy( );

  return 0;
}

void initContext( int argc, char** argv )
{
  glutInit( &argc, argv );
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

void createCubes( void )
{
  int idc = 0;
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
        q->setId(idc);
        idc++;
        cubes.push_back(q);

        lasso->addObject( q );
      }
    }
  }
}

void initOGL( void )
{
  glEnable( GL_DEPTH_TEST );
  glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

  const auto path = std::getenv( "RETO_SHADERS_PATH" );
  if ( !path )
  {
    throw std::runtime_error(
      "Error: RETO_SHADERS_PATH environment variable not found." );
  }

  std::string shadersPath;
  if ( path )
    shadersPath = std::string( path ) + std::string( "/" );

  prog.load( shadersPath + "colorUniform.vert",
    shadersPath + "colorUniform.frag" );
  prog.compileAndLink( );
  prog.autocatching( );

  glFrontFace( GL_CCW );
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  glEnable( GL_CULL_FACE );
}

void destroy( void )
{
}

void renderFunc( void )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  prog.use( );
  prog.sendUniform4m( "proj", camera->projectionMatrix( ) );
  prog.sendUniform4m( "view", camera->viewMatrix( ) );

  for( auto obj: cubes )
  {
    obj->render( &prog );
  }

  // Render after objects due alpha blending (if needed)
  lasso->draw( );

  glFlush();
  glutSwapBuffers( );
}

void resizeFunc( int w, int h )
{
  width = w;
  height = h;
  camera->ratio((( double ) width ) / height );
  glViewport( 0, 0, width, height );
  lasso->resize( w, h );
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
      camera->pivot( Eigen::Vector3f( 0.0f, 0.0f, 0.0f ));
      camera->radius( 1000.0f );
      camera->rotation( 0.0f, 0.0f );
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
  if ( state == GLUT_DOWN )
  {
    mouseButton = button;
    switch( mouseButton )
    {
      case 2:
        lasso->mouseDown( { x, y } );
        break;
      case 3:
      case 4:
        float newRadius = ( button == 3 ) ?
          camera->radius() / mouseWheelFactor :
          camera->radius() * mouseWheelFactor;
        camera->radius( newRadius );
        glutPostRedisplay();
        break;
    }

    previousX = x;
    previousY = y;
  }
  else
  {
    //mouse up
    switch( mouseButton )
    {
      case 2:
        lasso->mouseUp( { x, y }, camera->viewProjectionMatrix( ) );
        break;
    }
    mouseButton = -1;
  }
}

void mouseMotionFunc( int x, int y )
{
  float deltaX = x - previousX;
  float deltaY = y - previousY;
  switch( mouseButton )
  {
    case 0:
      camera->localRotation( deltaX * rotationScale, deltaY * rotationScale );
      break;
    case 1:
      camera->localTranslation( Eigen::Vector3f ( -deltaX * traslationScale,
        deltaY * traslationScale, 0.0f ) );
      break;
    case 2:
      lasso->mouseMove( { x, y } );
      break;
    case 3:
    case 4:
      break;
  }
  previousX = x;
  previousY = y;
  glutPostRedisplay();
}
