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

#include "Framebuffer.h"

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

  Quad::Quad( const std::string& vertexCode, const std::string& fragmentCode )
  {
    _program = new reto::ShaderProgram( );
    _program->loadVertexShaderFromText( vertexCode );
    _program->loadFragmentShaderFromText( fragmentCode );
    _program->compileAndLink();
    _program->autocatching();

    //Vars
    GLuint positionAttribIndex = 0;
    _vertices = {
      -1.0, -1.0,
      1.0, -1.0,
      -1.0,  1.0,
      1.0,  1.0,
    };

    //Create objects
    glGenVertexArrays( 1, &_vao );
    glGenBuffers( 1, &_vbo );

    //Vertex Array
    glBindVertexArray( _vao );

    //Vertex Buffer
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size( ) * sizeof( float ),
      &_vertices[ 0 ], GL_STATIC_DRAW );
    glEnableVertexArrayAttrib( _vao, positionAttribIndex );
    glVertexAttribPointer( positionAttribIndex, 2, GL_FLOAT, GL_FALSE, 0, 0 );
  }

  Quad::~Quad( void )
  {
    clear( );
  }

  reto::ShaderProgram* const& Quad::program( void ) const
  {
    return _program;
  }

  void Quad::draw( void ) const
  {
    glBindVertexArray( _vao );
    glDrawArrays( GL_TRIANGLE_STRIP, 0,
      static_cast< GLsizei >( _vertices.size( ) / 2 ) );
  }

  void Quad::clear( void )
  {
    glDeleteBuffers( 1, &_vbo );
    glDeleteVertexArrays( 1, &_vao );
    delete _program;
  }

  Framebuffer2D::Framebuffer2D( const std::string& vertexCode,
    const std::string& fragmentCode, const AttachmentsConfig& attsConfig,
    const unsigned int& width, const unsigned int& height )
      : reto::Quad( vertexCode, fragmentCode )
      , _attsConfig( attsConfig )
      , _hasDepth( false )
      , _hasStencil( false )
      , _hasDepthStencil( false )
  {    
    glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS, &_maxColorAttachments );

    //Create object
    glGenFramebuffers( 1, &_handler );

    //generateTextures
    generateTextures( width, height );
  }

  Framebuffer2D::~Framebuffer2D( void )
  {
    clear( );
  }

  void Framebuffer2D::bind( void )
  {
    glBindFramebuffer( GL_FRAMEBUFFER, _handler );
  }

  void Framebuffer2D::unbind( void )
  {
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
  }

  void Framebuffer2D::resize( const unsigned int& width,
    const unsigned int& height )
  {
    deleteTextures( );
    generateTextures( width, height );
  }

  void Framebuffer2D::bindAttachments( void )
  {
    size_t nAtts = _attachments.size( );
    int unit = 0;

    //First depth
    if ( _hasDepth )
    {
      _attachments[ GL_DEPTH_ATTACHMENT ]->bind( unit );
      --nAtts;
      ++unit;
    }

    //Stencil
    if ( _hasStencil )
    {
      _attachments[ GL_STENCIL_ATTACHMENT ]->bind( unit );
      --nAtts;
      ++unit;
    }

    //Depth stencil
    if ( _hasDepthStencil )
    {
      _attachments[ GL_DEPTH_STENCIL_ATTACHMENT ]->bind( unit );
      --nAtts;
      ++unit;
    }

    //Enlazamos las texturas
    for ( unsigned int i = 0; i < nAtts; ++i )
    {
      _attachments[ GL_COLOR_ATTACHMENT0 + i ]->bind( unit );
    }
  }

  void Framebuffer2D::clear( void )
  {
    deleteTextures( );
    glDeleteFramebuffers( 1, &_handler );
  }

  bool Framebuffer2D::isValid( void ) const
  {
    return ( glCheckFramebufferStatus( GL_FRAMEBUFFER )
      == GL_FRAMEBUFFER_COMPLETE );
  }

  void Framebuffer2D::generateTextures( const unsigned int& width,
    const unsigned int& height )
  {
    bind( );

    int colorAtts = 0;
    for( const auto& attConfig : _attsConfig )
    {
      if ( attConfig.first == GL_DEPTH_ATTACHMENT )
      {
        _hasDepth = true;
      }
      else if ( attConfig.first == GL_STENCIL_ATTACHMENT )
      {
        _hasStencil = true;
      }
      else if ( attConfig.first == GL_DEPTH_STENCIL_ATTACHMENT )
      {
        _hasDepthStencil = true;
      }
      else
      {
        colorAtts++;
        if ( colorAtts >= _maxColorAttachments )
        {
          throw std::runtime_error( "Error: Max color attachments reached." );
        }
      }

      _attachments[ attConfig.first ] = new reto::Texture2D(
        attConfig.second, width, height );

      glFramebufferTexture2D( GL_FRAMEBUFFER, attConfig.first, GL_TEXTURE_2D,
        _attachments[ attConfig.first ]->handler( ), 0 );
    }

    if ( !isValid( ) )
    {
      throw std::runtime_error( "Error: incomplete framebuffer." );
    }

    unbind( );
  }

  void Framebuffer2D::deleteTextures( void )
  {
    for ( auto& attachment : _attachments )
    {
      delete attachment.second;
    }
    _attachments.clear( );
  }

}
