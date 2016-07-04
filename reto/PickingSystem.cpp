/*
 * Copyright (c) 2014-2016 GMRV/URJC.
 *
 * Authors: Cristian Rodriguez Bernal (crodriguez)
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
#include "PickingSystem.h"
namespace reto
{
  PickingSystem::PickingSystem( reto::Camera* camera )
    : _camera(camera)
  {
    _program.loadFromText(
      _VertexCode( ),
      "#version 430\n"
      "out vec4 ourColor;\n"
      "in float pid;\n"
      "vec3 unpackColor3(float f) {\n"
      "    vec3 color;\n"
      "    color.r = floor(f / 256.0 / 256.0);\n"
      "    color.g = floor((f - color.r * 256.0 * 256.0) / 256.0);\n"
      "    color.b = floor(f - color.r * 256.0 * 256.0 - color.g * 256.0);\n"
      "    // vec3 with the 3 components in range [0..256]. Normalizing it.\n"
      "    return color / 255.0;\n"
      "}\n"
      "void main( ) {\n"
      "  ourColor = vec4(unpackColor3(pid), 1.0);\n"
      "}");
    _program.compileAndLink( );
    _program.addUniform("modelViewProj");
    _program.addUniform("id");
  }

  PickingSystem::~PickingSystem( void )
  {
    this->Clear( );
  }

  int PickingSystem::click( Point point )
  {
    int selected = -1;
    int WINDOW_HEIGHT = 500;  // TODO: Hardcoded

    unsigned int currentId = 0;
    std::set< reto::Pickable* >::iterator it;
    for ( const auto& object : _objects )
    {
      currentId = object->sendId( currentId );
      object->render( );
    }

    GLubyte color[4];
    glReadPixels( point.x, WINDOW_HEIGHT - point.y, 1, 1,
                  GL_RGBA, GL_UNSIGNED_BYTE, color );
    unsigned int value = ( unsigned int ) ( color[2] + color[1] * 256.0 +
                                        color[0] * 256.0 * 256.0 );
    if( value < _objects.size( ))
    {
      selected = value;
    }

    return selected;
  }

  std::set< unsigned int > PickingSystem::area( Point minPoint, Point maxPoint )
  {
    std::set<unsigned int> ret;
    int WINDOW_HEIGHT = 500;  // TODO: Hardcoded

    unsigned int currentId = 0;
    std::set< reto::Pickable* >::iterator it;
    for ( const auto& object : _objects )
    {
      currentId = object->sendId( currentId );
      object->render( );
    }

    GLubyte color[4];
    unsigned int value;

    for( int x = minPoint.x; x < maxPoint.x; x++ )
    {
      for( int y = minPoint.y; y < maxPoint.y; y++ )
      {
        glReadPixels( x, WINDOW_HEIGHT - y, 1, 1, GL_RGBA,
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
}
