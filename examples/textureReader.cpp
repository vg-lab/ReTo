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
bool traslation = false;

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

MyCube* mycube;
std::string textureFile;

int main( int argc, char** argv )
{
  if ( argc < 2 )
  {
    std::cerr << "Error: A texture file must be provided." << std::endl;
    return -1;
  }

  textureFile = std::string( argv[1] );

  initContext( argc, argv );
  initOGL( );

  mycube = new MyCube( 4.5f );

  cameraController = new reto::OrbitalCameraController( );
  camera = cameraController->camera( );

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
    std::cerr << "Error: " << glewGetErrorString( err ) << std::endl;
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

  prog.load( shadersPath + "texture.vert", shadersPath + "texture.frag" );
  prog.compileAndLink( );
  prog.autocatching( );

  glFrontFace( GL_CCW );
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

  constexpr unsigned int texSize = 1024;
  unsigned char *data = new unsigned char[texSize * texSize * 3];
  unsigned int n = 0;
  // Generate some checker board pattern, square is 8x8, colors are 255, 128 (white, gray)
  for (unsigned int yy = 0; yy < texSize; ++yy)
  {
    const int value_y = (yy & 0x8) == 0;
    for (unsigned int xx = 0; xx < texSize; ++xx)
    {
      const int value_x = (xx & 0x8) == 0;
      const auto pix_value = static_cast<GLubyte>(255 - (value_y ^ value_x) * 128);
      data[n++] = pix_value;
      data[n++] = pix_value;
      data[n++] = pix_value;
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
    "vg-lab",
    new reto::Texture2D( opts2, textureFile ) );
}

void destroy( void )
{
}

constexpr int MAX = 25;
constexpr int STEP = 5;

void renderFunc( void )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // std::cout << "DRAW" << std::endl;
  prog.use( );
  prog.sendUniform4m("proj", camera->projectionMatrix( ));
  prog.sendUniform4m("view", camera->viewMatrix( ));
  auto &manager = reto::TextureManager::getInstance( );
  for (auto i = -MAX; i <= MAX; i+= STEP) {
    for (auto j = -MAX; j <= MAX; j+= STEP) {
      for (auto k = -MAX; k <= MAX; k+= STEP) {
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
          manager.get("procedural")->bind( 0 );
        }
        else
        {
          manager.get("vg-lab")->bind( 0 );
        }

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
      cameraController->rotate(
        Eigen::Vector3f( deltaX * rotationScale ,
                         deltaY * rotationScale, 0.0f ));
    }
    if( traslation )
    {
      cameraController->translate(
        Eigen::Vector3f ( deltaX * traslationScale, -deltaY * traslationScale,
                          0.0f ));
    }

    previousX = x;
    previousY = y;
    glutPostRedisplay();
  }
}
