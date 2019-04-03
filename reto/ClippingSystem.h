/*
 * Copyright (c) 2014-2019 GMRV/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
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


#ifndef __RETO__CLIPPING_SYSTEM__
#define __RETO__CLIPPING_SYSTEM__

//std
#include <vector>
#include <set>
#include <map>

//glew
#ifndef __gl_h_
  #include <GL/glew.h>
#endif

//reto
#include <reto/api.h>
#include "ShaderProgram.h"
#include "PickingSystem.h"

namespace reto
{

  /**
  * Enum class to set clipping mode for clipping planes
  * @enum class ClippingMode
  */
  enum class ClippingMode
  {
    Local,
    Global
  };

  /**
   * Class to manage a clipping plane
   * @class ClippingPlane
   */
  class ClippingPlane
  {

    public:

      /**
       * ClippingPlane constructor
       * @param clippingMode: clipping mode (global or local, global by default)
       */
      RETO_API
      ClippingPlane(
        const reto::ClippingMode& clippingMode = ClippingMode::Global );

      /**
       * ClippingPlane constructor
       * @param a: x coefficient
       * @param b: y coefficient
       * @param c: z coefficient
       * @param d: distance from the origin
       * @param clippingMode: clipping mode (global or local, global by default)
       */
      RETO_API
      ClippingPlane( const float& a, const float& b, const float& c,
        const float& d,
        const reto::ClippingMode& clippingMode = ClippingMode::Global );

      /**
       * ClippingPlane destructor
       */
      ~ClippingPlane( void );

      /**
       * Method to get the clipping plane equation
       * @return clipping plane equation
       */
      RETO_API
      std::vector< float > getEquation( void ) const;

      /**
       * Method to set the clipping plane equation
       * @param a: x coefficient
       * @param b: y coefficient
       * @param c: z coefficient
       * @param d: distance from the origin
       */
      RETO_API
      void setEquation( const float& a, const float& b, const float& c,
        const float& d );

      /**
       * Method to set the clipping plane equation
       * @param point: point on the plane
       * @param normal: normal vector
       */
      RETO_API
      void setEquationByPointAndNormal( const Eigen::Vector3f& point,
        const Eigen::Vector3f& normal );

      /**
       * Method to set the clipping plane equation
       * @param point: point on the plane
       * @param vector1: vector on the plane
       * @param vector2: vector on the plane
       */
      RETO_API
      void setEquationByPointAndVectors( const Eigen::Vector3f& point,
        const Eigen::Vector3f& vector1, const Eigen::Vector3f& vector2 );

      /**
       * Method to set the clipping plane equation
       * @param point1: point on the plane
       * @param point2: point on the plane
       * @param point3: point on the plane
       */
      RETO_API
      void setEquationByPoints( const Eigen::Vector3f& point1,
        const Eigen::Vector3f& point2, const Eigen::Vector3f& point3 );

      /**
       * Method to get the plane clipping mode
       * @return clipping mode
       */
      RETO_API
      reto::ClippingMode getClippingMode( void ) const;

      /**
       * Method to set the plane clipping mode
       * @param clippingMode: clipping mode (global or local)
       */
      RETO_API
      void setClippingMode( const reto::ClippingMode& clippingMode );

      /**
       * Method to activate the clipping plane and activate it's uniforms
       * @param program: program pointer
       * @param index: index of clipping plane
       */
      RETO_API
      void activate( reto::ShaderProgram* program, const unsigned int& index ) const;

      /**
       * Method to deactivate the clipping plane
       * @param index: index of clipping plane
       */
      RETO_API
      void deactivate( const unsigned int& index ) const;

      /**
       * Method to clear clipping plane
       */
      RETO_API
      void clear( void );

    private:

      //! Equation of plane
      std::vector< float > _equation;

      //! Clipping mode of plane
      reto::ClippingMode _clippingMode;

  }; /* class ClippingPlane */

  /**
   * Class to manage clipping planes
   * @class ClippingSystem
   */
  class ClippingSystem
  {
    public:

      /**
       * ClippingSystem constructor
       */
      RETO_API
      ClippingSystem( void );

      /**
       * ClippingSystem constructor passing vertex shader code
       */
      RETO_API
      ClippingSystem( const std::string& vertexCode );

      /**
       * ClippingSystem destructor
       */
      ~ClippingSystem( void );

      /**
       * Method to get a clipping plane from ClippingSystem
       * @param alias: ClippingPlane alias
       * @return ClippingPlane pointer
       */
      RETO_API
      reto::ClippingPlane* get( const std::string& alias ) const;

      /**
       * Method to set a new clipping plane to ClippingSystem
       * @param alias: ClippingPlane alias
       * @param tex: ClippingPlane pointer
       */
      RETO_API
      void set( const std::string& alias, reto::ClippingPlane* plane );

      /**
       * Method to remove a clipping plane from ClippingSystem
       * @param alias: ClippingPlane alias
       */
      RETO_API
      void remove( const std::string& alias );

      /**
       * Method to activate clipping planes
       */
      RETO_API
      void activatePlanes( void ) const;

      /**
       * Method to deactivate clipping planes
       */
      RETO_API
      void deactivatePlanes( void ) const;

      /**
       * Method to draw objects using clipping system
       */
      RETO_API
      void draw( void ) const;

      /**
       * Method to add a pickable object
       * @param object: Pickable object
       */
      RETO_API
      void addObject( reto::Pickable* object );

      /**
       * Method to remove a pickable object
       * @param object: Pickable object
       */
      RETO_API
      void removeObject( reto::Pickable* object );

      /**
       * Method to get program handler
       * @return program handler.
       */
      RETO_API
      reto::ShaderProgram* const& program( void ) const;

      /**
       * Method to clear clipping planes
       */
      RETO_API
      void clear( void );

    private:
      //! Shader program
      reto::ShaderProgram* _program;

      //! Map of [ alias, clipping plane ]
      std::map< std::string, reto::ClippingPlane* > _planes;

      //! Max of clipping planes allowed by GPU
      int _maxPlanes;

      //! Vertex shader code for clipping planes
      std::string _VertexCode( void ) const;

      //! Fragment shader code for clipping planes
      std::string _FragmentCode( void ) const;

      //! Set of objects using clipping system
      std::set< reto::Pickable* > _objects;

  }; /* class ClippingSystem */

}

#endif /* __RETO__CLIPPING_SYSTEM__ */
