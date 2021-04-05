#include <string>

// OpenGL, GLEW, GLUT
#include <GL/glew.h>

#include <GLFW/glfw3.h>

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

reto::ShaderProgram prog;

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width and 
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

MyCube* mycube;
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


constexpr int MAX = 25;
constexpr int STEP = 25;

void renderFunc( GLFWwindow* window )
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

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
}


void idleFunc( void )
{
  static float angle = 0.0f;
  angle = ( angle > 2.0f * float( M_PI )) ? 0 : angle + 0.01f;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    mycube = new MyCube( 4.5f );

    camera = new reto::Camera( );
    cControllers[1] = new reto::FreeCameraController( camera );
    cControllers[1]->position( Eigen::Vector3f( 0.0f, 0.0f, 100.0f ));
    cControllers[0] = new reto::OrbitalCameraController( camera );
    cControllers[0]->radius( 100.0f );
    cameraController = cControllers[0];

    _previousTime = std::chrono::system_clock::now( );

    initOGL( );

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        idleFunc( );
        renderFunc( window );
    }

    glfwTerminate();
    return 0;
}