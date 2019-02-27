#include "SSBO.h"

// OpenGL, GLEW, GLUT.
#include <GL/glew.h>

#ifdef Darwin
  #define __gl_h_
  #define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
#else
  #include <GL/gl.h>
#endif

namespace reto
{
  SSBO::SSBO( unsigned int size, unsigned int type )
  {
    glGenBuffers( 1, &_handler );
    if( size > 0 )
    {
      this->bind( );
      glBufferData( GL_SHADER_STORAGE_BUFFER, size, nullptr, type );
    }
  }
  SSBO::~SSBO( void )
  {
    this->unbind( );
    if( this->_handler > 0 )
    {
      glDeleteBuffers( 1, &_handler );
    }
    this->_handler = 0;
  }
  void SSBO::bind( void )
  {
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, _handler );
  }
  void SSBO::bind( unsigned int index )
  {
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, index, _handler );
  }
  void SSBO::bufferSubData( int offset, unsigned int size, void* _data )
  {
    this->bind( );
    glBufferSubData( GL_SHADER_STORAGE_BUFFER, offset, size, _data );
  }
  void SSBO::data( unsigned int size, void* _data, unsigned int usage )
  {
    this->bind( );
    glBufferData( GL_SHADER_STORAGE_BUFFER, size, _data, usage );
  }
  void* SSBO::getData( void )
  {
    this->bind( );

    void* ptr = glMapBuffer( GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY );

    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
    return ptr;
  }
  unsigned int SSBO::getBuffer( void ) const
  {
    return _handler;
  }
  void SSBO::unbind( void )
  {
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );
  }
}