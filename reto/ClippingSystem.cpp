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

#include "ClippingSystem.h"

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

  ClippingPlane::ClippingPlane( const reto::ClippingMode& clippingMode )
    : _equation( { 0.0f, 0.0f, 0.0f, 0.0f } )
    , _clippingMode( clippingMode )
  {

  }

  ClippingPlane::ClippingPlane( const float& a, const float& b, const float& c,
    const float&d, const reto::ClippingMode& clippingMode )
    : _equation( { a, b, c, d } )
    , _clippingMode( clippingMode )
  {

  }

  ClippingPlane::~ClippingPlane( void )
  {
    clear( );
  }

  std::vector< float > ClippingPlane::getEquation( void ) const
  {
    return _equation;
  }

  void ClippingPlane::setEquation( const float& a, const float& b,
    const float& c, const float& d )
  {
    _equation = { a, b, c, d };
  }

  void ClippingPlane::setEquationByPointAndNormal( const Eigen::Vector3f& point,
    const Eigen::Vector3f& normal )
  {
    Eigen::Vector3f normNormal = normal.normalized( );
    _equation = { normNormal[ 0 ], normNormal[ 1 ], normNormal[ 2 ],
      -point.dot( normNormal ) };
  }

  void ClippingPlane::setEquationByPointAndVectors(
    const Eigen::Vector3f& point, const Eigen::Vector3f& vector1,
    const Eigen::Vector3f& vector2 )
  {
    Eigen::Vector3f normal = vector1.cross( vector2 );
    setEquationByPointAndNormal( point, normal );
  }

  void ClippingPlane::setEquationByPoints( const Eigen::Vector3f& point1,
    const Eigen::Vector3f& point2, const Eigen::Vector3f& point3 )
  {
    Eigen::Vector3f normal =
      ( ( point2 - point1 ).cross( point3 - point1 ) ).normalized( );
    setEquationByPointAndNormal( point1, normal );
  }

  reto::ClippingMode ClippingPlane::getClippingMode( void ) const
  {
    return _clippingMode;
  }

  void ClippingPlane::setClippingMode( const reto::ClippingMode& clippingMode )
  {
    _clippingMode = clippingMode;
  }

  void ClippingPlane::activate( reto::ShaderProgram* program,
                                const unsigned int& index ) const
  {
    glEnable( GL_CLIP_DISTANCE0 + index );

    std::string indexStr = std::to_string( index );
    program->sendUniform4v( "plane[" + indexStr + "]", &_equation[ 0 ] );

    switch( _clippingMode )
    {
      case reto::ClippingMode::Global:
        program->sendUniformb( "isLocal[" + indexStr + "]", false );
        break;
      case reto::ClippingMode::Local:
        program->sendUniformb( "isLocal[" + indexStr + "]", true );
        break;
    };
  }

  void ClippingPlane::deactivate( const unsigned int& index ) const
  {
    glDisable( GL_CLIP_DISTANCE0 + index );
  }

  void ClippingPlane::clear( void )
  {
    _equation.clear( );
  }

  ClippingSystem::ClippingSystem( void )
  {
    glGetIntegerv( GL_MAX_CLIP_PLANES, &_maxPlanes );

    _program = new reto::ShaderProgram( );
    _program->loadVertexShaderFromText( _VertexCode( ) );
    _program->loadFragmentShaderFromText( _FragmentCode( ) );
    _program->compileAndLink( );
    _program->autocatching( );
  }  

  ClippingSystem::ClippingSystem( const std::string& vertexCode )
  {
    glGetIntegerv( GL_MAX_CLIP_PLANES, &_maxPlanes );

    _program = new reto::ShaderProgram( );
    _program->loadVertexShaderFromText( vertexCode );
    _program->loadFragmentShaderFromText( _FragmentCode( ) );
    _program->compileAndLink( );
    _program->autocatching( );
  }

  ClippingSystem::~ClippingSystem( void )
  {
    clear( );
  }

  reto::ClippingPlane* ClippingSystem::get( const std::string& alias ) const
  {
    auto it = _planes.find( alias );
    if ( it == _planes.end( ) )
    {
      std::cerr << "Warning: Can't get '" << alias
        << "' plane. CLipping plane not found." << std::endl;
    }
    return it->second;
  }

  void ClippingSystem::set( const std::string& alias,
    reto::ClippingPlane* plane )
  {
    auto it = _planes.find( alias );
    if (it == _planes.end())
    {
      //Not found -> check max planes
      if ( _planes.size( ) < static_cast< size_t >( _maxPlanes ) )
      {
        _planes[ alias ] = plane;
      }
      else
      {
        std::cerr << "Warning: Can't add '" << alias
          << "' plane. Max number of clipping planes reached." << std::endl;
      }
    }
    else
    {
      //Found -> overwrite
      _planes[ alias ] = plane;
    }
  }

  void ClippingSystem::remove( const std::string& alias )
  {
    auto it = _planes.find( alias );
    if ( it != _planes.end( ) )
    {
      _planes.erase( it );
    }
    else
    {
      std::cerr << "Warning: Can't remove '" << alias
        << "' plane. CLipping plane not found." << std::endl;
    }
  }

  void ClippingSystem::activatePlanes( void ) const
  {
    unsigned int i = 0;
    _program->sendUniformi( "nPlanes",
      static_cast< GLint >( _planes.size( ) ) );
    for( const auto& plane : _planes )
    {
      plane.second->activate( _program, i );
      i++;
    }
  }

  void ClippingSystem::deactivatePlanes( void ) const
  {
    unsigned int i = 0;
    for( const auto& plane : _planes )
    {
      plane.second->deactivate( i );
      i++;
    }
  }

  void ClippingSystem::draw( void ) const
  {
    unsigned int i = 0;
    _program->sendUniformi( "nPlanes",
      static_cast< GLint >( _planes.size( ) ) );
    for( const auto& plane : _planes )
    {
      plane.second->activate( _program, i );
      i++;
    }

    for( const auto& object : _objects )
    {
      _program->sendUniform4v( "model", object->getModel( ) );
      object->render( _program );
    }

    i = 0;
    for( const auto& plane : _planes )
    {
      plane.second->deactivate( i );
      i++;
    }
  }

  std::string ClippingSystem::_VertexCode( void ) const
  {
    std::string maxPlanesStr = std::to_string( _maxPlanes );
    return std::string("#version 430 core\n"
      "in vec3 inPos;\n"
      "in vec3 inNormal;\n"
      "uniform mat4 proj;\n"
      "uniform mat4 view;\n"
      "uniform mat4 model;\n"
      "uniform int nPlanes;\n"
      "uniform vec4 plane[") + maxPlanesStr + ("];\n"
      "uniform bool isLocal[") + maxPlanesStr + ("];\n"
      "out float gl_ClipDistance[") + maxPlanesStr + ("];\n"
      "out vec3 norm;\n"

      "void main( void )\n"
      "{\n"
      "  for( int i = 0; i < nPlanes; i++ )\n"
      "  {\n"
      "    gl_ClipDistance[ i ] = isLocal[ i ] ?"
      "      dot( vec4( inPos, 1.0 ), plane[ i ] ) :"
      "      dot( model * vec4( inPos, 1.0 ), plane[ i ] );\n"
      "  }\n"
      "  mat3 normal = mat3( inverse( transpose( view * model ) ) );\n"
      "  norm = normal * inNormal;\n"
      "  gl_Position =  proj * view * model * vec4( inPos, 1.0 );\n"
      "}\n");
  }

  std::string ClippingSystem::_FragmentCode( void ) const
  {
    return std::string("#version 430 core\n"
      "out vec4 outColor;\n"
      "in vec3 norm;\n"

      "void main( void )\n"
      "{\n"
      "  outColor = vec4( norm, 1.0 );\n"
      "}\n");
  }

  void ClippingSystem::addObject( Pickable* object )
  {
    _objects.insert( object );
  }

  void ClippingSystem::removeObject( Pickable* object )
  {
    _objects.erase( object );
  }

  reto::ShaderProgram* const& ClippingSystem::program( void ) const
  {
    return _program;
  }

  void ClippingSystem::clear( void )
  {
    for ( auto& plane : _planes )
    {
      delete plane.second;
    }
    _planes.clear( );
    delete _program;
  }
}
