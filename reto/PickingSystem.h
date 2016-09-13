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
#ifndef __RETO__PICKING_SYSTEM__
#define __RETO__PICKING_SYSTEM__

#include <vector>
#include "ShaderProgram.h"

#include <vector>
#include <set>
#include "ShaderProgram.h"
#include "Camera.h"
#include "Pickable.h"

#include <tuple>
#include <reto/api.h>

namespace reto
{
  typedef std::pair<unsigned int, unsigned int> Point;
  class PickingSystem
  {
    public:
      RETO_API
      PickingSystem( );
      /**
       * Reuse a ShaderProgram that lacks fragment shader
       * @param prog: ProgramShader
       **/
      RETO_API
      PickingSystem( const reto::ShaderProgram& prog );
      virtual ~PickingSystem( );

      /**
       * Method to add a Pickable object
       * @param pickSystem: Pickable object
       */
      RETO_API
      void AddObject( reto::Pickable * pickSystem );
      /**
       * Method to remove a Pickable object
       * @param pickSystem: Pickable object
       */
      RETO_API
      void RemoveObject( reto::Pickable * pickSystem );
      /**
       * Method to clear Pickable elements
       */
      RETO_API
      void Clear( void );

      /**
       * Method to find front object in a specific point
       * @param point: Point (in OpenGL coordinates)
       * @return int: Indice that is visible
       */
      RETO_API
      int click( Point point );

      /**
       * Method to find front object in a specific area
       * @param minPoint: minPoint (in OpenGL coordinates)
       * @param maxPoint: maxPoint (in OpenGL coordinates)
       * @return std::set<unsigned int> Indices that objects are visibles
       */
      RETO_API
      std::set<unsigned int> area( Point minPoint, Point maxPoint );

      RETO_API
      reto::ShaderProgram const& program( ) const;

    protected:
      /**
       * This method is invoked in the constuctor after creating the program.
       * Override thist just like you want it ( Default: Cache model, view, proj and id)
       */
      RETO_API
      virtual void init( void );

      /**
       * This method is invoked to render objects.
       * Override thist just like you want it (Default: Send id uniform)
       */
      RETO_API
      virtual void renderObjects( void );

    protected:
      reto::ShaderProgram _program;
      std::set< reto::Pickable* > _objects;

      virtual std::string _VertexCode( void );
  };
};

#endif // __RETO__PICKING_SYSTEM__
