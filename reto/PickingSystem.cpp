/*
 * Copyright (c) 2014-2017 VG-Lab/URJC.
 *
 * Authors: Cristian Rodríguez Bernal <cristian.rodriguez@urjc.es>
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

#include "PickingSystem.h"


//OpenGL
#ifndef SKIP_GLEW_INCLUDE
#include <GL/glew.h>
#endif
#ifdef Darwin
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif


namespace reto
{
  PickingSystem::PickingSystem( )
  {
    _program->loadFromText(
      _VertexCode( ),
      "#version 430\n"
      "layout(location = 0) out vec4 ourColor;\n"
      "in float pid;\n"

      "float module(float x, float y) {\n"
      "  return x - y * floor(x / y);\n"
      "}\n"

      "vec3 unpackColor(float f) {\n"
      "  vec3 color;\n"
      "  color.b = floor(f / (256 * 256));\n"
      "  color.g = floor((f - color.b * 256 * 256) / 256);\n"
      "  color.r = floor(module(f, 256.0));\n"
      "  return color / 255.0;\n"
      "}\n"

      "void main( ) {\n"
      "  vec3 cc = unpackColor(pid);\n"
      "  ourColor = vec4(cc, 1.0);\n"
      "}\n");
    _program->compileAndLink( );

    _program->autocatching( );
    //this->init();
  }

  void PickingSystem::init( )
  {
    //_program.addUniform("modelViewProj");
    //_program.addUniform("id");
  }

  PickingSystem::PickingSystem( reto::ShaderProgram* prog )
  {
    _program = prog;
    _program->loadFragmentShaderFromText(
      "#version 430\n"
      "layout(location = 0) out vec4 ourColor;\n"
      "in float pid;\n"

      "float module(float x, float y) {\n"
      "  return x - y * floor(x / y);\n"
      "}\n"

      "vec3 unpackColor(float f) {\n"
      "  vec3 color;\n"
      "  color.b = floor(f / (256 * 256));\n"
      "  color.g = floor((f - color.b * 256 * 256) / 256);\n"
      "  color.r = floor(module(f, 256.0));\n"
      "  return color / 255.0;\n"
      "}\n"

      "void main( ) {\n"
      "  vec3 cc = unpackColor(pid);\n"
      "  ourColor = vec4(cc, 1.0);\n"
      "}\n");
    _program->compileAndLink( );
    _program->autocatching( );
  }

  void PickingSystem::renderObjects( void )
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    unsigned int currentId = 0;
    //std::set< reto::Pickable* >::iterator it;
    for ( const auto& object : _objects )
    {
      currentId = object->sendId( currentId );
      // WARNING: SEND ID (OR ANOTHER VALUE) HERE!
      this->_program->sendUniformf("id", currentId);
      object->render( this->_program );
    }
  }

  PickingSystem::~PickingSystem( void )
  {
    this->Clear( );
  }

  int PickingSystem::click( Point point )
  {
    int selected = -1;
    glScissor( point.first, point.second, 1, 1 );
    glEnable(GL_SCISSOR_TEST);
    std::cout << _objects.size( ) << std::endl;
    this->renderObjects( );
    glDisable(GL_SCISSOR_TEST);

    GLubyte color[4];
    glReadPixels(point.first, point.second, 1, 1,
      GL_RGBA, GL_UNSIGNED_BYTE, color);
    int value = color[0] + color[1] * 256 + color[2] * 256 * 256;
    if (value < 3355443) {
       std::cout << value << std::endl;
    }
    std::cout << "R: " << (int)color[0] << ", G: " << (int)color[1] << ", B: " << (int)color[2] << std::endl;
    if( value < (int)_objects.size( ))
    {
      selected = value;
    }

    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return selected;
  }

  std::set< unsigned int > PickingSystem::area( Point minPoint, Point maxPoint )
  {
    std::set<unsigned int> ret;

    glScissor( minPoint.first, minPoint.second, maxPoint.first, maxPoint.second );
    glEnable(GL_SCISSOR_TEST);
    this->renderObjects( );
    //glDisable(GL_SCISSOR_TEST);

    GLubyte color[4];
    unsigned int value;

    for( auto x = minPoint.first; x < maxPoint.first; x++ )
    {
      for( auto y = minPoint.second; y < maxPoint.second; y++ )
      {
        glReadPixels( x, y, 1, 1, GL_RGBA,
                      GL_UNSIGNED_BYTE, color );
        value = ( unsigned int )( color[2] + color[1] * 256 +
                              color[0] * 256 * 256 );
        if( value < _objects.size( ))
        {
          ret.insert( value );
        }
      }
    }

    return ret;
  }

  std::string PickingSystem::_VertexCode( void )
  {
    return std::string("#version 430\n"
      "layout (location = 0) in vec3 Position;\n"
      "uniform mat4 modelViewProj;\n"
      "uniform int id;\n"
      "out float pid;\n"
      "void main( ) {\n"
      "    pid = float(id);\n"
      "    gl_Position = modelViewProj * vec4(Position,1.0);\n"
      "}");
  }

  void PickingSystem::AddObject( Pickable* pickSystem )
  {
    _objects.insert( pickSystem );
  }

  void PickingSystem::RemoveObject( Pickable* pickSystem )
  {
    _objects.erase( pickSystem );
  }

  void PickingSystem::Clear ( void )
  {
    _objects.clear( );
  }

  reto::ShaderProgram* const& PickingSystem::program( ) const
  {
    return this->_program;
  }
}
