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

#define _USE_MATH_DEFINES
#include <math.h>

// From libraries.
#include <reto/reto.h>
using namespace reto;

reto::Camera* camera;

void initContext( int argc, char** argv );
void initOGL( void );
void renderFunc( void );
void idleFunc( void );
void destroy( void );

int main( int argc, char** argv )
{
  initContext( argc, argv );
  initOGL( );

  camera = new reto::Camera( );

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

  glutDisplayFunc( renderFunc );
  glutIdleFunc( idleFunc );
}

reto::ShaderProgram sortProg, cameraPosProg;

reto::SSBO *inIdxBuffer, *inDistanceBuffer, *inPositionsBuffer, *inAlivesBuffer;

struct Vector4
{
  float x;
  float y;
  float z;
  float w;
};

std::vector<uint32_t> indicesVector;
std::vector<float> distancesVector;
std::vector<uint32_t> alivesVector;
std::vector<Vector4> positionsVector; // You need to use vec4 from data aligment rules
uint32_t bufferElements = std::pow( 2, 2 );
const uint32_t bufferSize = bufferElements * sizeof( float );

float randFloat(float a, float b)
{
  return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}

void initOGL( void )
{
  glEnable( GL_DEPTH_TEST );
  glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

  const auto path = std::getenv( "RETO_SHADERS_PATH" );
  std::string shadersPath;
  if ( path )
    shadersPath = std::string( path ) + std::string( "/" );
  
  sortProg.addDefine( "WORK_GROUP", 2 );
  sortProg.addDefine( "VALUE", 2.0f );
  sortProg.loadComputeShader( shadersPath + "computeSort2.comp" );
  sortProg.compileAndLink( );
  sortProg.autocatching( );

  cameraPosProg.loadComputeShader( shadersPath + "computeCameraDistance.comp" );
  cameraPosProg.compileAndLink( );
  cameraPosProg.autocatching( );

  std::cout << "Allocate buffers...";
    inIdxBuffer = new reto::SSBO( );
    inDistanceBuffer = new reto::SSBO( bufferSize, GL_DYNAMIC_COPY );
    inPositionsBuffer = new reto::SSBO( );
    inAlivesBuffer = new reto::SSBO( );
    reto::SSBO::unbind( );
  std::cout << "OK" << std::endl;
}

template<typename T>
void printArray(const std::vector<T>& array)
{
  for( size_t i = 0; i < array.size( ); ++i )
  {
    std::cerr << array[ i ] <<" ";
  }
  std::cerr << std::endl;
}
void renderFunc( void )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  std::cout << "Upload input data...";
  std::cout << std::endl;
 
  indicesVector.clear( );
  positionsVector.clear( );
  alivesVector.clear( );
  
  for( uint32_t i = 0; i < bufferElements; ++i )
  {
    indicesVector.push_back( i );
    positionsVector.push_back( Vector4{ 
      randFloat( -5.0f, +5.0f ), 
      randFloat( -5.0f, +5.0f ), 
      randFloat( -5.0f, +5.0f ), 
      1.0f
    } );
    alivesVector.push_back( i % 2 );
  }

  inIdxBuffer->data< uint32_t >( indicesVector, GL_DYNAMIC_COPY );
  inPositionsBuffer->data< Vector4 >( positionsVector, GL_DYNAMIC_COPY );
  inAlivesBuffer->data< uint32_t >( alivesVector, GL_DYNAMIC_COPY );

  std::cout << "NO SORT: " << std::endl;
  printArray<uint32_t>( indicesVector );
  printArray<float>( distancesVector );
  std::cout << std::endl;


  cameraPosProg.use( );

  inIdxBuffer->bind( 0 );
  inDistanceBuffer->bind( 1 );
  inPositionsBuffer->bind( 2 );
  inAlivesBuffer->bind( 3 );

  //cameraPosProg.sendUniformu("bufferElems", bufferElements);
  //cameraPosProg.sendUniform3v("cameraPos", camera->position( ) );
  cameraPosProg.sendUniform4m("modelView", camera->viewMatrix( ) ); // TODO: identity model

  glDispatchCompute( bufferElements, 1, 1 );

  // ----------------------------------------------- //
  // --------------- SORT ZONE (GPU) --------------- //
  // ----------------------------------------------- //

  sortProg.use( );
  inIdxBuffer->bind( 0 );
  inDistanceBuffer->bind( 1 );
  //sortProg.sendUniformu( "bufferElems", bufferElements );
  
  uint32_t PARTICLE_COUNT = bufferElements;
  uint32_t WORK_GROUP_SIZE = 2;
  uint32_t workGroups = (PARTICLE_COUNT) / WORK_GROUP_SIZE;
  
  uint32_t logn = std::log2( bufferElements );

  for( uint32_t i = 0; i < logn; ++i )
  {
    for( uint32_t j = 0; j <= i; ++j )
    {
      sortProg.sendUniformi( "p", i );
      sortProg.sendUniformi( "q", j );
      glDispatchCompute( workGroups, 1, 1 );
      glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );
    }
  }

  //glDispatchCompute( bufferElements, 1, 1 );      // Call CS kernel


  std::vector<uint32_t> indicesResult;
  std::vector<float> distancesResult;


  auto p1 = (uint32_t*)inIdxBuffer->getData( );   // Extract data from buffer
  indicesResult = { p1, p1 + bufferElements };    // Create vector from raw data

  auto p2 = (float*)inDistanceBuffer->getData( ); // Extract data from buffer
  distancesResult = { p2, p2 + bufferElements };  // Create vector from raw data

  // ----------------------------------------------- //
  // --------------- SORT ZONE (CPU) --------------- //
  // ----------------------------------------------- //

  std::cout << "SORTED: " << std::endl;
  printArray<uint32_t>( indicesVector );
  printArray<uint32_t>( indicesResult );
  printArray<float>( distancesVector );
  printArray<float>( distancesResult );

  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
}

void idleFunc( void )
{
  //glutPostRedisplay( );
}

void destroy( void )
{
  delete inIdxBuffer;
  delete inDistanceBuffer;
}