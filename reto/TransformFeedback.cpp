#include "TransformFeedback.h"

//std
#include <iostream>
#include <sstream>
#include <algorithm>

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

  TransformFeedback::TransformFeedback( const std::string& vertexCode,
    std::vector< const char* > varyings, int mode )
  {
    _program = new reto::ShaderProgram( );
    _program->loadVertexShaderFromText( vertexCode );
    _program->create( );
    _program->feedbackVarying( varyings.data( ) ,
      static_cast< GLsizei >( varyings.size( ) ), mode );
    _program->link( );
    _program->autocatching( );
  }

  TransformFeedback::~TransformFeedback( void )
  {
    clear( );
  }

  void TransformFeedback::draw( void )
  {
    std::stringstream idsMsg;
    int selected = 0;
    for ( auto object : _objects )
    {
      const std::vector< float > positions = object.first->getPositions( );

      Buffers* buffers = object.second;

      // Disable rasterizer, use Program and bind Vertex Array
      glEnable( GL_RASTERIZER_DISCARD );
      program( )->use( );
      program( )->sendUniform4m( "model", object.first->getModel( ) );
      glBindVertexArray( buffers->_vao );

      // Begin Transform Feedback
      glBindTransformFeedback( GL_TRANSFORM_FEEDBACK, buffers->_tfo );
      glBeginTransformFeedback( GL_POINTS );

      // Draw
      glDrawArrays( GL_POINTS, 0, static_cast< GLsizei >( positions.size( ) ) );

      // End Transform Feedback
      glEndTransformFeedback( );
      //glFlush( );
      glBindTransformFeedback( GL_TRANSFORM_FEEDBACK, 0 );

      // Unbind Vertex Array, unuse Program and enable rasterizer
      glBindVertexArray( 0 );
      glUseProgram( 0 );
      glDisable( GL_RASTERIZER_DISCARD );

      // Get results
      std::vector< float > resutlBuffer( positions.size( ) / 3 );
      glBindBuffer( GL_TRANSFORM_FEEDBACK_BUFFER, buffers->_tbo );
      glGetBufferSubData( GL_TRANSFORM_FEEDBACK_BUFFER, 0,
        positions.size( ) / 3 * sizeof( float ), resutlBuffer.data( ) );
      glBindBuffer( GL_TRANSFORM_FEEDBACK_BUFFER, 0 );

      // If any position is inside of rubberband, set this object selected
      if( std::find( resutlBuffer.begin( ), resutlBuffer.end( ), 1.0f )
        != resutlBuffer.end( ) )
      {
        object.first->setSelected( true );
        //getId only if you want to know which cube is selected
        idsMsg << object.first->getId( ) << " ";
        //selected only to std::cout number of selected objects
        selected++;
      }
      else
      {
        object.first->setSelected( false );
      }
    }
    if( !idsMsg.str( ).empty( ) )
    {
      std::stringstream selMsg;
      selMsg << "\nObjects ids:\n" << idsMsg.str( ) << "\n\n" << selected
        << " objects selected.";
      std::cout << selMsg.str( ) << std::endl;
    }
  }

  void TransformFeedback::addObject( Pickable* object )
  {
    _objects[ object ] = new Buffers( );
    generate( object );
  }

  void TransformFeedback::removeObject( Pickable* object )
  {
    _objects.erase( object );
  }

  reto::ShaderProgram* const& TransformFeedback::program( void ) const
  {
    return _program;
  }

  void TransformFeedback::clear( void )
  {
    for ( auto object : _objects )
    {
      object.second->clear( );
    }
    _objects.clear( );
    delete _program;
  }

  void TransformFeedback::generate( reto::Pickable* object )
  {
    const std::vector< float > positions = object->getPositions( );
    Buffers* buffers = _objects[ object ];

    // Vars
    GLuint vertexAttribIndex = 0;

    // Create objects
    glGenVertexArrays( 1, &buffers->_vao );
    glGenBuffers( 1, &buffers->_vbo );
    glGenTransformFeedbacks( 1, &buffers->_tfo );
    glGenBuffers( 1, &buffers->_tbo );

    //Vertex Array
    glBindVertexArray( buffers->_vao );
    glBindBuffer( GL_ARRAY_BUFFER, buffers->_vbo );
    glVertexAttribPointer( vertexAttribIndex, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glEnableVertexAttribArray( 0 );
    glBindVertexArray( 0 );

    // Vertex Buffer
    glBindBuffer( GL_ARRAY_BUFFER, buffers->_vbo );
    glBufferData( GL_ARRAY_BUFFER, positions.size( ) * sizeof( float ),
      positions.data( ), GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    // Transform Feedback
    glBindTransformFeedback( GL_TRANSFORM_FEEDBACK, buffers->_tfo );
    glBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, 0, buffers->_tbo );
    glBindTransformFeedback( GL_TRANSFORM_FEEDBACK, 0 );

    // Transform Feedback Buffer
    glBindBuffer( GL_TRANSFORM_FEEDBACK_BUFFER, buffers->_tbo );
    glBufferData( GL_TRANSFORM_FEEDBACK_BUFFER,
      positions.size( ) / 3 * sizeof( float ), NULL, GL_DYNAMIC_COPY);
    glBindBuffer( GL_TRANSFORM_FEEDBACK_BUFFER, 0 );
  }

  void TransformFeedback::Buffers::clear( void )
  {
    glDeleteBuffers( 1, &_tbo );
    glDeleteTransformFeedbacks( 1, &_tfo );
    glDeleteBuffers( 1, &_vbo );
    glDeleteVertexArrays( 1, &_vao );
  }

}
