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
  class RETO_API PickingSystem
  {
    public:
      
      PickingSystem( );
      /**
       * Reuse a ShaderProgram that lacks fragment shader
       * @param prog: ProgramShader*
       **/
      
      PickingSystem( reto::ShaderProgram* prog );
      virtual ~PickingSystem( );

      /**
       * Method to add a Pickable object
       * @param pickSystem: Pickable object
       */
      
      void AddObject( reto::Pickable * pickSystem );
      /**
       * Method to remove a Pickable object
       * @param pickSystem: Pickable object
       */
      
      void RemoveObject( reto::Pickable * pickSystem );
      /**
       * Method to clear Pickable elements
       */
      
      void Clear( void );

      /**
       * Method to find front object in a specific point
       * @param point: Point (in OpenGL coordinates)
       * @return int: Indice that is visible
       */
      
      int click( Point point );

      /**
       * Method to find front object in a specific area
       * @param minPoint: minPoint (in OpenGL coordinates)
       * @param maxPoint: maxPoint (in OpenGL coordinates)
       * @return std::set<unsigned int> Indices that objects are visibles
       */
      
      std::set<unsigned int> area( Point minPoint, Point maxPoint );

      
      reto::ShaderProgram* const& program( ) const;

    protected:
      /**
       * This method is invoked in the constuctor after creating the program.
       * Override thist just like you want it ( Default: Cache model, view, proj and id)
       */
      
      virtual void init( void );

      /**
       * This method is invoked to render objects.
       * Override thist just like you want it (Default: Send id uniform)
       */
      
      virtual void renderObjects( void );

    //protected:
    public:
      reto::ShaderProgram* _program;
      std::set< reto::Pickable* > _objects;

      virtual std::string _VertexCode( void );
  };
};

#endif // __RETO__PICKING_SYSTEM__
