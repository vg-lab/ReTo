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

#ifndef __RETO__TRANSFORM_FEEDBACK__
#define __RETO__TRANSFORM_FEEDBACK__

//std
#include <vector>
#include <map>

//glew
#ifndef __gl_h_
  #include <GL/glew.h>
#endif

//reto
#include <reto/api.h>
#include "ShaderProgram.h"
#include "Pickable.h"

namespace reto
{

  /**
   * Class to manage transform feedbacks
   * @class TransformFeedback
   */
  class TransformFeedback
  {

    public:

      /**
       * TransformFeedback constructor
       * @param vertexCode: vertex shader code
       * @param varyings: transform feedback varying names
       * @param mode: transform feedback mode
       */
      RETO_API
      TransformFeedback( const std::string& vertexCode,
        std::vector< const char* > varyings, int mode );

      /**
       * TransformFeedback destructor
       */
      virtual ~TransformFeedback( void );

      /**
       * Method to draw transform feedback
       */
      RETO_API
      void draw( void );

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
       * Method to clear transform feedback
       */
      RETO_API
      void clear( void );

    private:

      //! Shader program
      reto::ShaderProgram* _program;

      /**
       * Struct to manage object buffers
       * @struct Buffers
       */
      struct Buffers
      {
        //! Vertex array handler
        unsigned int _vao;

        //! Vertex buffer handler
        unsigned int _vbo;

        //! Transform feedback handler
        unsigned int _tfo;

        //! Transform feedback buffer handler
        unsigned int _tbo;

        /**
         * Method to clear buffers per object
         */
        void clear( void );

      };

      //! Map of [ object, buffers ]
      std::map< reto::Pickable*, Buffers* > _objects;

      /**
       * Method to generate transform feedback for an object
       * @return program handler.
       */
      void generate( reto::Pickable* object );

  }; /* class TransformFeedback */

} /* namespace reto */

#endif /* __RETO__TRANSFORM_FEEDBACK__ */
