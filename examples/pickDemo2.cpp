/**
 * @file    spinesStats.cpp
 * @brief
 * @author  Juan Guerrero Martín <juan.guerrero@urjc.es>
 * @date    2016
 * @remarks Copyright (c) 2016 GMRV/URJC. All rights reserved.
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

reto::Camera* camera;

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

MyCube* mycube;

int main( int argc, char** argv )
{
  initContext( argc, argv );
  initOGL( );

  mycube = new MyCube( 4.0f );

  camera = new reto::Camera( );

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
reto::PickingSystem *ps;

std::vector<MyCube*> cubes;

int MAX = 5;
void initOGL( void )
{
  glEnable( GL_DEPTH_TEST );
  glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

  prog.load( RETO_EXAMPLE_SHADERS_V0_VERT,
              RETO_EXAMPLE_SHADERS_V0_FRAG );
  prog.compileAndLink( );
  prog.autocatching( );

  progPick.loadVertexShader( RETO_EXAMPLE_SHADERS_V0_VERT );
  ps = new reto::PickingSystem( progPick );

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

        MyCube* q = new MyCube( 4.0f );
        q->setModel(_modelVecMat);
        cubes.push_back(q);

        ps->AddObject( q );
      }
    }
  }

  glFrontFace( GL_CCW );
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

  glEnable( GL_CULL_FACE );

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
    std::cout << "PICK" << std::endl;
    progPick.use( );
    progPick.sendUniform4m("proj", camera->projectionMatrix( ));
    progPick.sendUniform4m("view", camera->viewMatrix( ));

    int selected = ps->click( reto::Point( { pickX, pickY } ) );
    std::cout << selected << std::endl;

    comprobar = false;
  }
  else
  {
    /*prog.use( );
    prog.sendUniform4m("proj", camera->projectionMatrix( ));
    prog.sendUniform4m("view", camera->viewMatrix( ));
    for (auto cube: cubes)
    {
      cube->render( &prog );
    }*/
  }
  glutSwapBuffers( );
}

void resizeFunc( int w, int h )
{
  width = w;
  height = h;
  camera->ratio((( double ) width ) / height );
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
    if( button == 1 ) traslation = true;
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
                        camera->radius() / mouseWheelFactor :
                        camera->radius() * mouseWheelFactor;
      camera->radius( newRadius );
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
      camera->localRotation( deltaX * rotationScale,
                             deltaY * rotationScale );
    }
    if( traslation )
    {
      camera->localTranslation( Eigen::Vector3f ( -deltaX * traslationScale,
                                                  deltaY * traslationScale,
                                                  0.0f ) );
    }
    previousX = x;
    previousY = y;
    glutPostRedisplay();
  }
}
