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

#ifndef __RETO__TEXTURE_MANAGER__
#define __RETO__TEXTURE_MANAGER__

#ifndef __gl_h_
  #include <GL/glew.h>
#endif

#include <unordered_map>
#include <memory>
#include <vector>
#include <iostream>
#include <fstream>
#include <FreeImage.h>

namespace reto {
  struct TextureConfig {
    unsigned int internalFormat = GL_RGBA;
    unsigned int format = GL_RGBA;
    unsigned int border = 0;
    unsigned int minFilter = GL_LINEAR;
    unsigned int magFilter = GL_LINEAR;
    unsigned int type = GL_UNSIGNED_BYTE;
    unsigned int level = 0;
    unsigned int wrapS = GL_CLAMP_TO_EDGE;
    unsigned int wrapT = GL_CLAMP_TO_EDGE;
    unsigned int wrapR = GL_CLAMP_TO_EDGE;
  };
  class Texture
  {
    public:
      virtual ~Texture( ) = 0;
      void bind( int slot = -1 );
      void unbind( );
      inline unsigned int handler( )
      {
        return this->_handler;
      }
      inline unsigned int target( )
      {
        return this->_target;
      }
      /*virtual void resize( unsigned int, unsigned int )
      {

      }*/
      inline bool isLoaded( ) {
        return this->_loaded;
      }
    protected:
      Texture( TextureConfig& options, unsigned int type );

      virtual void load( ) = 0;

      bool _loaded = false;
      unsigned int _target;
      unsigned int _handler;

      unsigned int _internalFormat;
      unsigned int _format;
      unsigned int _border;
      unsigned int _minFilter;
      unsigned int _magFilter;
      unsigned int _type;
      unsigned int _level;
      unsigned int _wrapS;
      unsigned int _wrapT;
      unsigned int _wrapR;
  };
  class Texture2D: public Texture
  {
    public:
      Texture2D( TextureConfig& options, unsigned int width, unsigned int height );
      Texture2D( TextureConfig& options, void* data, unsigned int width, unsigned int height );
      Texture2D( TextureConfig& options, std::string src );
      virtual ~Texture2D( void );
    protected:
      void configTexture( void* data = nullptr );
      virtual void load( void );
      unsigned char* loadTexture( const char* fileName_,
        unsigned int &width_,
        unsigned int &height_ );

      std::string _src;
      unsigned int _width;
      unsigned int _height;
  };
  class Texture2DArray: public Texture
  {
    public:
      Texture2DArray( TextureConfig& options, std::vector< void* > data,
        unsigned int width, unsigned int height );
      virtual ~Texture2DArray( void );
    protected:
      virtual void load( void );
  };
  class Texture3D: public Texture
  {
    public:
      Texture3D( TextureConfig& options, void* data, unsigned int width,
        unsigned int height, unsigned int depth );
      virtual ~Texture3D( void );
    protected:
      virtual void load( void );
  };
  class TextureManager
  {
    public:
          static TextureManager& getInstance();
      void add( std::string alias, Texture* tex );
      void remove( std::string alias );
      Texture* get( std::string alias );
      protected:
      ~TextureManager( );
    protected:
      std::unordered_map< std::string, std::unique_ptr< Texture > > _textures;
  };
};
#endif /* __RETO__TEXTURE_MANAGER__ */
