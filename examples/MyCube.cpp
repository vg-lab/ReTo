/*
 * Copyright (c) 2014-2017 GMRV/URJC.
 *
 * Authors: Cristian Rodríguez Bernal <cristian.rodriguez@urjc.es>
 *
 * This file is part of ReTo <https://gitlab.gmrv.es/nsviz/ReTo>
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

#include "MyCube.h"

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

#include <vector>

MyCube::MyCube( float side )
  : _selected( false )
{
    side /= 2.0f;
    _v = {
      // Front
     -side, -side, side,
      side, -side, side,
      side,  side, side,
     -side,  side, side,
     // Right
      side, -side, side,
      side, -side, -side,
      side,  side, -side,
      side,  side, side,
     // Back
     -side, -side, -side,
     -side,  side, -side,
      side,  side, -side,
      side, -side, -side,
     // Left
     -side, -side, side,
     -side,  side, side,
     -side,  side, -side,
     -side, -side, -side,
     // Bottom
     -side, -side, side,
     -side, -side, -side,
      side, -side, -side,
      side, -side, side,
     // Top
     -side,  side, side,
      side,  side, side,
      side,  side, -side,
     -side,  side, -side
    };

    std::vector< float > n = {
      // Front
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      // Right
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      // Back
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      // Left
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      // Bottom
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      // Top
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f
    };

    _c = {
      //Front (blue)
      0.0f, 0.0f, 1.0f, 1.0f,
      0.0f, 0.0f, 1.0f, 1.0f,
      0.0f, 0.0f, 1.0f, 1.0f,
      0.0f, 0.0f, 1.0f, 1.0f,
      //Right (cyan)
      0.0f, 1.0f, 1.0f, 1.0f,
      0.0f, 1.0f, 1.0f, 1.0f,
      0.0f, 1.0f, 1.0f, 1.0f,
      0.0f, 1.0f, 1.0f, 1.0f,
      //Back (red)
      1.0f, 0.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 0.0f, 1.0f,
      //Left (purple)
      1.0f, 0.0f, 1.0f, 1.0f,
      1.0f, 0.0f, 1.0f, 1.0f,
      1.0f, 0.0f, 1.0f, 1.0f,
      1.0f, 0.0f, 1.0f, 1.0f,
      //Bottom (green)
      0.0f, 1.0f, 0.0f, 1.0f,
      0.0f, 1.0f, 0.0f, 1.0f,
      0.0f, 1.0f, 0.0f, 1.0f,
      0.0f, 1.0f, 0.0f, 1.0f,
      //Top (yellow)
      1.0f, 1.0f, 0.0f, 1.0f,
      1.0f, 1.0f, 0.0f, 1.0f,
      1.0f, 1.0f, 0.0f, 1.0f,
      1.0f, 1.0f, 0.0f, 1.0f
     };

    std::vector< float > tex = {
      // Front
      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
      // Right
      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
      // Back
      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
      // Left
      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
      // Bottom
      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
      // Top
      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f
    };

  std::vector< GLuint > el = {
    0,1,2,0,2,3,
    4,5,6,4,6,7,
    8,9,10,8,10,11,
    12,13,14,12,14,15,
    16,17,18,16,18,19,
    20,21,22,20,22,23
  };

  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);

  glGenBuffers(5, _handle);

  _size = 36;

  glBindBuffer(GL_ARRAY_BUFFER, _handle[0]);
  glBufferData(GL_ARRAY_BUFFER, _v.size( ) * sizeof(float), _v.data( ), GL_STATIC_DRAW);
  glVertexAttribPointer( (GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);  // Vertex position

  glBindBuffer(GL_ARRAY_BUFFER, _handle[1]);
  glBufferData(GL_ARRAY_BUFFER, n.size( ) * sizeof(float), n.data( ), GL_STATIC_DRAW);
  glVertexAttribPointer( (GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);  // Vertex normal

  glBindBuffer(GL_ARRAY_BUFFER, _handle[2]);
  glBufferData(GL_ARRAY_BUFFER, _c.size( ) * sizeof(float), _c.data( ), GL_STATIC_DRAW);
  glVertexAttribPointer( (GLuint)2, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(2);  // Vertex colors

  glBindBuffer(GL_ARRAY_BUFFER, _handle[3]);
  glBufferData(GL_ARRAY_BUFFER, tex.size( ) * sizeof(float), tex.data( ), GL_STATIC_DRAW);
  glVertexAttribPointer( (GLuint)3, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(3);  // texture coords

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _handle[4]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, el.size( ) * sizeof(GLuint), el.data( ), GL_STATIC_DRAW);

  glBindVertexArray(0);
}

void MyCube::render() {
  glBindVertexArray(_vao);
  glDrawElements(GL_TRIANGLES, (GLsizei)_size, GL_UNSIGNED_INT, 0);
  //glDrawArrays(GL_TRIANGLES, 0, _size);
}

void MyCube::render( reto::ShaderProgram* ss )
{
  //std::cout << this->model[12] << ", " << this->model[13] << ", " << this->model[14] << std::endl;
  ss->sendUniform4m("model", this->_model.data( ));
  if ( !_selected )
  {
    ss->sendUniform4v( "uColor", { 0.5f, 0.0f, 0.0f, 0.0f } );
  }
  else
  {
    ss->sendUniform4v( "uColor", { 0.0f, 0.5f, 0.0f, 0.0f } );
  }
  glBindVertexArray(_vao);
  glDrawElements(GL_TRIANGLES, (GLsizei)_size, GL_UNSIGNED_INT, 0);
}
std::vector< float > MyCube::getModel( void ) const
{
  return _model;
}

void MyCube::setModel( const std::vector< float > value )
{
  this->_model = value;
}

std::vector< float > MyCube::getPositions( void ) const
{
  return _v;
}

bool MyCube::getSelected( void ) const
{
  return _selected;
}

void MyCube::setSelected( const bool& selected )
{
  _selected = selected;
}
