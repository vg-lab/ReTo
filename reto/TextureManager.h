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

#include <reto/api.h>

#ifndef __gl_h_
  #include <GL/glew.h>
#endif

#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>

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
    unsigned int packAlignment = 0;
    unsigned int unpackAlignment = 0;
  };
  class Texture
  {
  public:
    RETO_API
    virtual ~Texture( void ) = 0;

    /**
     * Method to bind this texture
     * @param slot: Active texture. If value < 0, this method don't call
     *   glActiveTexture
     */
    RETO_API
    void bind( int slot = -1 );

    /**
     * Method to unbind this texture
     */
    RETO_API
    void unbind( void );

    /**
     * Method to get texture raw id
     * @return GLuint
     */
    RETO_API
    inline unsigned int handler( void ) const
    {
      return this->_handler;
    }
    /**
     * Method to get texture target
     * @return GLuint
     */
    RETO_API
    inline unsigned int target( void ) const
    {
      return this->_target;
    }
    /**
     * Method to check if texture was loaded
     * @return bool
     */
    RETO_API
    inline bool isLoaded( void ) const
    {
      return this->_loaded;
    }
    /**
     * Method to resize texture
     * @param w: New width
     * @param h: New height
     */
    RETO_API
    virtual void resize( int w, int h );
    /**
     * Method to resize texture
     * @param w: New width
     * @param h: New height
     * @param data: New texture data
     */
    RETO_API
    virtual void resize( int w, int h, void* data );
  protected:
    Texture( const TextureConfig& options, unsigned int type );

    virtual void load( void ) = 0;

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

    unsigned int _packAlignment;
    unsigned int _unpackAlignment;
  };
  class Texture2D: public Texture
  {
  public:
    RETO_API
    Texture2D( const TextureConfig& options, unsigned int width, unsigned int height );
    RETO_API
    Texture2D( const TextureConfig& options, void* data, unsigned int width, unsigned int height );
    RETO_API
    Texture2D( const TextureConfig& options, const std::string src );
    virtual ~Texture2D( void );
    /**
     * Method to resize texture
     * @param w: New width
     * @param h: New height
     */
    RETO_API
    virtual void resize( int w, int h);
    /**
     * Method to resize texture
     * @param w: New width
     * @param h: New height
     * @param data: New texture data
     */
    RETO_API
    virtual void resize( int w, int h, void* data );
  protected:
    void configTexture( void* data = nullptr );
    virtual void load( void );

#ifdef RETO_USE_FREEIMAGE
    unsigned char* loadTexture( const char* fileName_,
      unsigned int& width_,
      unsigned int& height_ );
#endif

    std::string _src;
    unsigned int _width;
    unsigned int _height;
  };
  class Texture1D: public Texture
  {
  public:
    Texture1D( const TextureConfig& options, void* data, unsigned int width );
    virtual ~Texture1D( void );
    /**
     * Method to update texture content
     * @param w: New width
     * @param data: New texture data
     */
    RETO_API
    void update(void* data, unsigned int width);
    //virtual void resize( int w, int h);
  protected:
    unsigned int _width;
    void configTexture( void* data = nullptr );
    virtual void load( void );
  };
  class Texture2DArray: public Texture
  {
  public:
    RETO_API
    Texture2DArray( const TextureConfig& options, std::vector< void* > data,
      unsigned int width, unsigned int height );
    RETO_API
    virtual ~Texture2DArray( void );
    //virtual void resize( int w, int h);
  protected:
    virtual void load( void );
  };
  class Texture3D: public Texture
  {
  public:
    RETO_API
    Texture3D( const TextureConfig& options, void* data, unsigned int width,
      unsigned int height, unsigned int depth );
    RETO_API
    virtual ~Texture3D( void );
    /**
     * Method to update texture content
     * @param w: New width
     * @param h: New height
     * @param d: New depth
     * @param data: New texture data
     */
    RETO_API
    void update( int w, int h, int d, void* data );
    //virtual void resize( int w, int h);
  protected:
    virtual void load( void );
  };
  class TextureManager
  {
  public:
    RETO_API
    static TextureManager& getInstance( void );
    /**
     * Method to add new texture to TextureManager
     * @param alias: Texture alias
     * @param tex: Texture pointer
     */
    RETO_API
    void add( const std::string& alias, Texture* tex );
    /**
     * Method to remove a texture from TextureManager
     * @param alias: Texture alias
     */
    RETO_API
    void remove( const std::string& alias );
    /**
     * Method to get a texture from TextureManager
     * @param alias: Texture alias
     * @return Texture pointer
     */
    RETO_API
    Texture* get( const std::string& alias );
  protected:
    TextureManager( void ) { }
    ~TextureManager( void );
  protected:
    std::unordered_map< std::string, Texture* > _textures;
  };
};
#endif /* __RETO__TEXTURE_MANAGER__ */
