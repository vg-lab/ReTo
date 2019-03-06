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

void initContext( int argc, char** argv );
void initOGL( void );
void renderFunc( void );
void idleFunc( void );
void destroy( void );

int main( int argc, char** argv )
{
  initContext( argc, argv );
  initOGL( );

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

reto::ShaderProgram prog;

reto::SSBO *inIdxBuffer, *inDistanceBuffer;

std::vector<uint32_t> indicesVector;
std::vector<float> distancesVector;
uint32_t bufferElements = 20;

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
  
  prog.loadComputeShader( shadersPath + "computeSort.comp" );
  prog.compileAndLink( );
  prog.autocatching( );

  std::cout << "Allocate buffers...";
    inIdxBuffer = new reto::SSBO( );
    inDistanceBuffer = new reto::SSBO( );
    reto::SSBO::unbind( );
  std::cout << "OK" << std::endl;
}

void bubbleSort(std::vector<uint32_t>& idxs, std::vector<float>& dists) 
{ 
  for(uint32_t i = 0; i < dists.size( ); i++)
  {
    for(uint32_t j = 0; j < dists.size( ) - 1; j++)
    {   
      if (dists[j] > dists[j+1])
      {
        std::swap(dists[j], dists[j+1]);
        std::swap(idxs[j], idxs[j+1]);
      }
    }
  }
} 
template<typename T>
void printArray(const std::vector<T>& array)
{
  for(size_t i = 0; i < array.size( ); ++i)
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
  distancesVector.clear( );
  
  for( uint32_t i = 0; i < bufferElements; ++i )
  {
    indicesVector.push_back( i );
    distancesVector.push_back( randFloat( 0.0f, 5.0f ) );
  }

  inIdxBuffer->data( bufferElements * sizeof( uint32_t ), 
    indicesVector.data( ), GL_DYNAMIC_COPY );
  inDistanceBuffer->data( bufferElements * sizeof( float ), 
    distancesVector.data( ), GL_DYNAMIC_COPY );

  std::cout << "NO SORT: " << std::endl;
  printArray<uint32_t>( indicesVector );
  printArray<float>( distancesVector );
  std::cout << std::endl;

  // ----------------------------------------------- //
  // --------------- SORT ZONE (GPU) --------------- //
  // ----------------------------------------------- //

  prog.use( );
  inIdxBuffer->bind( 0 );
  inDistanceBuffer->bind( 1 );
  prog.sendUniformu( "bufferElems", bufferElements );

  glDispatchCompute( bufferElements, 1, 1 );      // Call CS kernel


  std::vector<uint32_t> indicesResult;
  std::vector<float> distancesResult;


  auto p1 = (uint32_t*)inIdxBuffer->getData( );   // Extract data from buffer
  indicesResult = { p1, p1 + bufferElements };    // Create vector from raw data

  auto p2 = (float*)inDistanceBuffer->getData( ); // Extract data from buffer
  distancesResult = { p2, p2 + bufferElements };  // Create vector from raw data

  // ----------------------------------------------- //
  // --------------- SORT ZONE (CPU) --------------- //
  // ----------------------------------------------- //
  bubbleSort( indicesVector, distancesVector );

  std::cout << "SORTED: " << std::endl;
  printArray<uint32_t>( indicesVector );
  printArray<uint32_t>( indicesResult );
  printArray<float>( distancesVector );
  printArray<float>( distancesResult );

  if ( std::equal( indicesResult.begin( ), indicesResult.end( ), 
    indicesVector.begin( ), []( float r, float h ) { return r == h; } ) )
  {
    std::cout << "Ok. Same indices arrays as equals." << std::endl;
  }
  else
  {
    std::cout << "Fail. Invalid indices result." << std::endl;
  }

  if ( std::equal( distancesResult.begin( ), distancesResult.end( ), 
    distancesVector.begin( ), []( float r, float h ) { return r == h; } ) )
  {
    std::cout << "Ok. Same distance arrays as equals." << std::endl;
  }
  else
  {
    std::cout << "Fail. Invalid distance result." << std::endl;
  }

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