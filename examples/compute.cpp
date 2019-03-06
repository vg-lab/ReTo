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

reto::SSBO *inABuffer, *inBBuffer, *outBuffer;

const uint32_t bufferElements = 10;
const uint32_t bufferSize = bufferElements * sizeof( uint32_t );

void initOGL( void )
{
  glEnable( GL_DEPTH_TEST );
  glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

  const auto path = std::getenv( "RETO_SHADERS_PATH" );
  std::string shadersPath;
  if ( path )
    shadersPath = std::string( path ) + std::string( "/" );
  
  prog.loadComputeShader( shadersPath + "computeSum.comp" );
  prog.compileAndLink( );
  prog.autocatching( );

  std::cout << "Allocate buffers...";
    inABuffer = new reto::SSBO( );
    inBBuffer = new reto::SSBO( );

    outBuffer = new reto::SSBO( bufferSize, GL_DYNAMIC_COPY );
    reto::SSBO::unbind( );
  std::cout << "OK" << std::endl;
}


void renderFunc( void )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  std::cout << "Upload input data...";
    std::vector<float> hostDataA( bufferElements );
      const float min = 1.0f, max = 99.0f;
      for ( float& n : hostDataA )
      {
        n = ( max - min ) * ( ( ( ( float ) rand( ) ) / ( float ) RAND_MAX ) ) + min;
      }

      inABuffer->data( bufferSize, hostDataA.data( ), GL_DYNAMIC_COPY );


      std::vector<float> hostDataB( bufferElements );
      std::vector<float> cpuResult( bufferElements );
      size_t i = 0;
      for ( float& n : hostDataB )
      {
        n = min + ( max - min ) * ( ( ( ( float ) rand( ) ) / ( float ) RAND_MAX ) ) + min;

        cpuResult[ i ] = hostDataA[ i ] + n;
        ++i;
      }

      inBBuffer->data( bufferSize, hostDataB.data( ), GL_DYNAMIC_COPY );
  std::cout << "OK" << std::endl;

  std::cout << "Input data:" << std::endl;

  for ( i = 0; i < bufferElements; ++i )
  {
    std::cout << (hostDataA[ i ] + hostDataB[ i ]) << ", ";
  }
  std::cout << "..., " << ( hostDataA[ bufferElements - 1 ] + 
    hostDataB[ bufferElements - 1 ]) << std::endl;

  std::cout << "Run computations...";

  prog.use( );

  inABuffer->bind( 0 );
  inBBuffer->bind( 1 );
  outBuffer->bind( 2 );

  glDispatchCompute( bufferElements, 1, 1 );
  std::cout << "OK" << std::endl;

  std::cout << "Read results...";

  outBuffer->bind( );
  auto ptr = (float*)outBuffer->getData( );
  std::vector<float> result = { ptr, ptr + bufferElements };

  if ( std::equal( result.begin( ), result.end( ), 
    cpuResult.begin( ), []( float r, float h ) { return r == h; } ) )
  {
    std::cout << "Ok. Same arrays as equals." << std::endl;
  }
  else
  {
    std::cout << "Fail. Invalid result." << std::endl;
  }

  std::cout << "Output data:" << std::endl;
  for ( i = 0; i < bufferElements; ++i )
  {
    std::cout << result[ i ] << ", ";
  }
  std::cout << "..., " << result[ bufferElements - 1 ] << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
}

void idleFunc( void )
{
  glutPostRedisplay( );
}

void destroy( void )
{
  delete outBuffer;
  delete inABuffer;
  delete inBBuffer;
}