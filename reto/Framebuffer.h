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


#ifndef __RETO__FRAMEBUFFER__
#define __RETO__FRAMEBUFFER__

//std
#include <map>

//glew
#ifndef __gl_h_
  #include <GL/glew.h>
#endif

//reto
#include <reto/api.h>
#include "ShaderProgram.h"
#include "TextureManager.h"

namespace reto
{

  /**
   * Class to manage a quad
   * @class Quad
   */
  class Quad
  {
    public:

      /**
       * Quad constructor
       * @param vertexCode: vertex shader code
       * @param fragmentCode: fragment shader code
       */
      RETO_API
      Quad( const std::string& vertexCode, const std::string& fragmentCode );

      /**
       * Quad destructor
       */
      RETO_API
      ~Quad( void );

      /**
       * Method to get program handler
       * @return program handler.
       */
      RETO_API
      reto::ShaderProgram* const& program( void ) const;

      /**
       * Method to draw quad
       */
      RETO_API
      void draw( void ) const;

      /**
       * Method to clear quad resources
       */
      RETO_API
      void clear( void );

    private:

      //! Vertex array handler
      GLuint _vao;

      //! Vertex buffer handler
      GLuint _vbo;

      //! Shader program
      reto::ShaderProgram* _program;

      //! Quad vertices
      std::vector< float > _vertices;

  }; /* class Quad */

  //!map of [ attachment, textureConfig ]
  using AttachmentsConfig = std::map< GLuint, reto::TextureConfig >;

  //!map of [ attachment, texture ]
  using Attachments2D = std::map< GLuint, reto::Texture2D* >;

  /**
   * Class to manage a framebuffer with 2D textures
   * @class Framebuffer2D
   */
  class Framebuffer2D : public Quad
  {

    public:

      /**
       * Framebuffer2D constructor
       * @param vertexCode: vertex shader code
       * @param fragmentCode: fragment shader code
       * @param attsConfig: attachments configuration
       * @param width: screen width
       * @param height: screen height
       */
      RETO_API
      Framebuffer2D( const std::string& vertexCode,
        const std::string& fragmentCode, const AttachmentsConfig& attsConfig,
        const unsigned int& width, const unsigned int& height );

      /**
       * Framebuffer2D destructor
       */
      RETO_API
      virtual ~Framebuffer2D( void );

      /**
       * Method to bind framebuffer
       */
      RETO_API
      void bind( void );

      /**
       * Method to bind default framebuffer
       */
      RETO_API
      void unbind( void );

      /**
       * Method to resize the framebuffer size
       * @param width: screen width
       * @param height: screen height
       */
      RETO_API
      void resize( const unsigned int& width, const unsigned int& height );

      /**
       * Method to bind all framebuffer attachments
       */
      RETO_API
      void bindAttachments( void );

      /**
       * Method to bind all framebuffer attachments
       */
      RETO_API
      void clear( void );

    private:

      //! Framebuffer handler
      GLuint _handler;

      //! Attachments configuration
      AttachmentsConfig _attsConfig;

      //! Attachments
      Attachments2D _attachments;

      //! Flag for depth texture
      bool _hasDepth;

      //! Flag for stencil texture
      bool _hasStencil;

      //! Flag for depth stencil texture
      bool _hasDepthStencil;

      //! Max color attachments allowed by GPU
      int _maxColorAttachments;

      /**
       * Method to check if framebuffer is complete
       */
      bool isValid( void ) const;

      /**
       * Method to generate framebuffer textures
       * @param width: screen width
       * @param height: screen height
       */
      void generateTextures( const unsigned int& width,
        const unsigned int& height );

      /**
       * Method to delete framebuffer textures
       * @param width: screen width
       * @param height: screen height
       */
      void deleteTextures( void );

  }; /* class Framebuffer2D */

} /* namespace reto */

#endif /* __RETO__FRAMEBUFFER__ */
