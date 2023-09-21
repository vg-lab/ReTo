/*
 * Copyright (c) 2014-2019 VG-Lab/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
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
reto::OrbitalCameraController* orbitalCamera;

// X Y mouse position.
int previousX;
int previousY;

// States.
bool wireframe = false;
int mouseButton = -1;

// Constants.
constexpr float mouseWheelFactor = 1.2f;
constexpr float rotationScale = 0.01f;
constexpr float traslationScale = 0.2f;

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

std::vector< reto::ClippingPlane* > clippingPlanes;
reto::ShaderProgram* program;

unsigned int width = 500;
unsigned int height = 500;

//Cubes
std::vector<MyCube*> cubes;
constexpr int MAX = 25;
constexpr int STEP = 5;

std::string vertexCode( void )
{
  int _maxPlanes;
  glGetIntegerv( GL_MAX_CLIP_PLANES, &_maxPlanes );
  std::string maxPlanesStr = std::to_string( _maxPlanes );

  return std::string("#version 430 core\n"
    "in vec3 inPos;\n"
    "in vec3 inNormal;\n"
    "uniform mat4 proj;\n"
    "uniform mat4 view;\n"
    "uniform mat4 model;\n"
    "uniform int nPlanes;\n"
    "uniform vec4 plane[") + maxPlanesStr + ("];\n"
    "uniform bool isLocal[") + maxPlanesStr + ("];\n"
    "out float gl_ClipDistance[") + maxPlanesStr + ("];\n"
    "out vec3 norm;\n"

    "void main( void )\n"
    "{\n"
    "  for( int i = 0; i < nPlanes; i++ )\n"
    "  {\n"
    "    gl_ClipDistance[ i ] = isLocal[ i ] ?"
    "      dot( vec4( inPos, 1.0 ), plane[ i ] ) :"
    "      dot( model * vec4( inPos, 1.0 ), plane[ i ] );\n"
    "  }\n"
    "  mat3 normal = mat3( inverse( transpose( view * model ) ) );\n"
    "  norm = normal * inNormal;\n"
    "  gl_Position =  proj * view * model * vec4( inPos, 1.0 );\n"
    "}\n");
}

std::string fragmentCode( void )
{
  return std::string("#version 430 core\n"
    "out vec4 outColor;\n"
    "in vec3 norm;\n"

    "void main( void )\n"
    "{\n"
    "  outColor = vec4( norm, 1.0 );\n"
    "}\n");
}

int main( int argc, char** argv )
{
  initContext( argc, argv );
  initOGL( );

  camera = new reto::Camera( );
  orbitalCamera = new reto::OrbitalCameraController( camera );

  program = new reto::ShaderProgram( );
  program->loadVertexShaderFromText( vertexCode( ) );
  program->loadFragmentShaderFromText( fragmentCode( ) );
  program->compileAndLink( );
  program->autocatching( );

  clippingPlanes.emplace_back( new ClippingPlane( 0.0f, 0.0f, 0.0f, 5.5f ) );
  clippingPlanes.emplace_back( new ClippingPlane( 0.0f, 0.0f, 0.0f, 5.5f ) );

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
    const std::string message("Error: RETO_SHADERS_PATH environment variable not found.");
    std::cerr << message << std::endl;
    throw std::runtime_error( message );
  }

  const auto shadersPath = std::string( path ) + std::string( "/" );

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

  //Use program and send common uniforms
  program->use( );
  program->sendUniform4m( "proj", camera->projectionMatrix( ) );
  program->sendUniform4m( "view", camera->viewMatrix( ) );
  program->sendUniformi( "nPlanes",
   static_cast< GLint >( clippingPlanes.size( ) ) );

  unsigned int i = 0;

  //Activate clipping planes
  for( const auto& plane : clippingPlanes )
  {
    plane->activate( program, i );
    i++;
  }

  //Render objects
  for( auto obj: cubes )
  {
    //_program->sendUniform4v( "model", object->getModel( ) );
    obj->render( program );
  }

  //Deactivate clipping planes
  i = 0;
  for( const auto& plane : clippingPlanes )
  {
    plane->deactivate( i );
    i++;
  }

  program->unuse( );

  glFlush();
  glutSwapBuffers( );
}

void resizeFunc( int w, int h )
{
  width = w;
  height = h;
  orbitalCamera->windowSize( width, height );
  glViewport( 0, 0, width, height );
}

void idleFunc( void )
{
  static float angle = 0.0f;
  angle = ( angle > 2.0f * float( M_PI )) ? 0 : angle + 0.01f;

  clippingPlanes.at( 0 )->setEquation( sin(angle), cos(angle), 0.0f, 5.5f );
  clippingPlanes.at( 1 )->setEquation( -sin(angle), -cos(angle), 0.0f, 5.5f );

  glutPostRedisplay( );
}

void keyboardFunc( unsigned char key, int, int )
{
  switch( key )
  {
    // Camera control.
    case 'c':
    case 'C':
      orbitalCamera->position( Eigen::Vector3f( 0.0f, 0.0f, 0.0f ));
      orbitalCamera->radius( 1000.0f );
      orbitalCamera->rotation( Eigen::Vector3f( 0.0f, 0.0f, 0.0f ));
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
      case 0:
        break;
      case 3:
      case 4:
        float newRadius = ( button == 3 ) ?
        orbitalCamera->radius() / mouseWheelFactor :
        orbitalCamera->radius() * mouseWheelFactor;
        orbitalCamera->radius( newRadius );
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
      case 0:        
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
      orbitalCamera->rotate( Eigen::Vector3f( deltaX * rotationScale,
                                              deltaY * rotationScale,
                                              0.0f ));
      break;
    case 1:
      orbitalCamera->translate( Eigen::Vector3f( -deltaX * traslationScale,
                                                 deltaY * traslationScale,
                                                 0.0f ));
      break;
    case 2:
      break;
    case 3:
    case 4:
      break;
  }
  previousX = x;
  previousY = y;
  glutPostRedisplay();
}
