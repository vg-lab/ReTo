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

#include "TextureManager.h"

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

#ifdef RETO_USE_FREEIMAGE
  // Image processing.
  #include <FreeImage.h>
#endif

namespace reto {
  Texture::Texture( const TextureConfig& options, unsigned int type )
    : _handler( -1 )
  {
    this->_target = type;
    this->_level = options.level;
    this->_border = options.border;
    this->_internalFormat = options.internalFormat;
    this->_format = options.format;
    this->_type = options.type;

    this->_wrapS = options.wrapS;
    this->_wrapT = options.wrapT;
    this->_wrapR = options.wrapR;

    this->_minFilter = options.minFilter;
    this->_magFilter = options.magFilter;

    this->_packAlignment = options.packAlignment;
    this->_unpackAlignment = options.unpackAlignment;
  }
  Texture::~Texture( )
  {
    glDeleteTextures( 1, &this->_handler );
    this->_handler = -1;
  }
  void Texture::bind( int slot )
  {
    this->load( );
    if ( slot >= 0 )
    {
      glActiveTexture( GL_TEXTURE0 + slot );
    }
    glBindTexture( this->_target, this->_handler );
  }
  void Texture::unbind( )
  {
    glBindTexture( this->_target, -1 );
  }
  void Texture::resize( int, int ) {}
  void Texture::resize( int, int, void* ) {}

  unsigned int Texture::handler( void ) const
  {
    return this->_handler;
  }

  unsigned int Texture::target( void ) const
  {
    return this->_target;
  }

  bool Texture::isLoaded( void ) const
  {
    return this->_loaded;
  }

  Texture1D::Texture1D( const TextureConfig& options, void* data, unsigned int width )
    : Texture(options, GL_TEXTURE_1D)
    , _width( width )
    {
    glGenTextures(1, &this->_handler);

    glBindTexture( this->_target, this->_handler );

    this->configTexture( data );
    this->_loaded = true;
    }
  Texture1D::~Texture1D( void ) { }

  void Texture1D::load( void )
  { }
  void Texture1D::update(void* data, unsigned int width)
  {
    _width = width;
    bind( );
    configTexture( data );
    unbind( );
  }
  void Texture1D::configTexture( void* data )
  {
    if (_packAlignment > 0)
    {
      glPixelStorei(GL_PACK_ALIGNMENT, this->_packAlignment);
    }
    if (_unpackAlignment > 0)
    {
      glPixelStorei(GL_UNPACK_ALIGNMENT, this->_unpackAlignment);
    }

    glTexParameteri( this->_target, GL_TEXTURE_MIN_FILTER, this->_minFilter );
    glTexParameteri( this->_target, GL_TEXTURE_MAG_FILTER, this->_magFilter );
    glTexParameteri( this->_target, GL_TEXTURE_WRAP_S, this->_wrapS );
    glTexParameteri( this->_target, GL_TEXTURE_WRAP_T, this->_wrapT );

    glTexImage1D(
      this->_target,
      this->_level,
      this->_internalFormat,
      this->_width,
      this->_border,
      this->_format,
      this->_type,
      data
    );

    this->unbind( );
  }

  Texture2D::Texture2D( const TextureConfig& options, unsigned int width, unsigned int height )
    : Texture2D( options, nullptr, width, height )
  {
  }

  Texture2D::Texture2D( const TextureConfig& options, void* data, unsigned int width, unsigned int height )
    : Texture( options, GL_TEXTURE_2D )
    , _width( width )
    , _height( height )
  {
    glGenTextures(1, &this->_handler);

    this->bind();
    glBindTexture( this->_target, this->_handler );

    this->configTexture( data );
    this->_loaded = true;
  }

  Texture2D::Texture2D( const TextureConfig& options, const std::string src )
    : Texture( options, GL_TEXTURE_2D )
    , _src ( src )
    , _width {0}
    , _height {0}
  {
  }

  Texture2D::~Texture2D( void )
  {
  }

  void Texture2D::configTexture( void* data )
  {
    glTexImage2D(
      this->_target,
      this->_level,
      this->_internalFormat,
      this->_width,
      this->_height,
      this->_border,
      this->_format,
      this->_type,
      data
    );

    glTexParameteri( this->_target, GL_TEXTURE_MIN_FILTER, this->_minFilter );
    glTexParameteri( this->_target, GL_TEXTURE_MAG_FILTER, this->_magFilter );
    glTexParameteri( this->_target, GL_TEXTURE_WRAP_S, this->_wrapS );
    glTexParameteri( this->_target, GL_TEXTURE_WRAP_T, this->_wrapT );

    this->unbind( );
  }
  void Texture2D::resize( int w, int h)
  {
    resize( w, h, nullptr );
  }
  void Texture2D::resize( int w, int h, void* data)
  {
    _width = w;
    _height = h;
    this->bind();
    configTexture(data);
  }
  void Texture2D::load( void )
  {
    if ( !this->_loaded )
    {
      glGenTextures( 1, &this->_handler );

      glBindTexture( this->_target, this->_handler );

#ifdef RETO_USE_FREEIMAGE
      auto pixels = this->loadTexture( this->_src.c_str( ), this->_width, this->_height );

      this->configTexture( pixels );
#else
      this->configTexture( nullptr );
#endif
      this->_loaded = true;
    }
  }
#ifdef RETO_USE_FREEIMAGE
  unsigned char* Texture2D::loadTexture( const char* fileName_,
    unsigned int& width_,
    unsigned int& height_ )
  {
    FreeImage_Initialise( TRUE );

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType( fileName_, 0 );
    if ( format == FIF_UNKNOWN )
      format = FreeImage_GetFIFFromFilename( fileName_ );
    if ( ( format == FIF_UNKNOWN ) || !FreeImage_FIFSupportsReading( format ) )
      return nullptr;

    FIBITMAP* img = FreeImage_Load( format, fileName_ );
    if ( img == nullptr )
      return nullptr;

    FIBITMAP* tempImg = img;
    img = FreeImage_ConvertTo32Bits( img );
    FreeImage_Unload( tempImg );

    width_ = FreeImage_GetWidth( img );
    height_ = FreeImage_GetHeight( img );

    //BGRA a RGBA
    unsigned char * map = new unsigned char[ 4 * width_*height_ ];
    char *buff = ( char* )FreeImage_GetBits( img );

    for ( unsigned int j = 0; j < width_*height_; ++j )
    {
      map[ j * 4 + 0 ] = buff[ j * 4 + 2 ];
      map[ j * 4 + 1 ] = buff[ j * 4 + 1 ];
      map[ j * 4 + 2 ] = buff[ j * 4 + 0 ];
      map[ j * 4 + 3 ] = buff[ j * 4 + 3 ];
    }

    FreeImage_Unload( img );
    FreeImage_DeInitialise( );

    return map;
  }
#endif

  Texture2DArray::Texture2DArray( const TextureConfig& options, std::vector< void* > data,
    unsigned int width, unsigned int height )
    : Texture( options, GL_TEXTURE_2D_ARRAY )
  {
    this->load( );
    glBindTexture( this->_target, this->_handler );
    glTexImage3D( this->_target, this->_level, this->_internalFormat,
      width, height, GLsizei(data.size( )), 0, this->_format, this->_type, nullptr );

    unsigned int i = 0;
    for ( const auto& layer: data )
    {
      glTexSubImage3D(
        this->_target, 0, 0, 0, i, width, height, 1,
        this->_format, this->_type, layer
      );
      ++i;
    }
    glTexParameteri( this->_target, GL_TEXTURE_MIN_FILTER, this->_minFilter );
    glTexParameteri( this->_target, GL_TEXTURE_MAG_FILTER, this->_magFilter );
    glTexParameteri( this->_target, GL_TEXTURE_WRAP_S, this->_wrapS );
    glTexParameteri( this->_target, GL_TEXTURE_WRAP_T, this->_wrapT );
    glTexParameteri( this->_target, GL_TEXTURE_WRAP_R, this->_wrapR );
    this->unbind( );
  }
  Texture2DArray::~Texture2DArray( void )
  {
  }
  void Texture2DArray::load( void )
  {
    if ( !this->_loaded ) {
      glGenTextures( 1, &this->_handler );
      glBindTexture( this->_target, this->_handler );
      this->_loaded = true;
    }
  }

  Texture3D::Texture3D( const TextureConfig& options, void* data, unsigned int width,
    unsigned int height, unsigned int depth ):
    Texture( options, GL_TEXTURE_3D )
  {
    this->load( );
    glTexParameteri( this->_target, GL_TEXTURE_MIN_FILTER, this->_minFilter );
    glTexParameteri( this->_target, GL_TEXTURE_MAG_FILTER, this->_magFilter );
    glTexParameterf( this->_target, GL_TEXTURE_WRAP_S, this->_wrapS);
    glTexParameterf( this->_target, GL_TEXTURE_WRAP_T, this->_wrapT);
    glTexParameteri( this->_target, GL_TEXTURE_WRAP_R, this->_wrapR);

    // Set the mipmap levels (base and max)
    glTexParameteri( this->_target, GL_TEXTURE_BASE_LEVEL, 0 );
    glTexParameteri( this->_target, GL_TEXTURE_MAX_LEVEL, 4 );

    if (_packAlignment > 0)
    {
      glPixelStorei(GL_PACK_ALIGNMENT, _packAlignment);
    }
    if (_unpackAlignment > 0)
    {
      glPixelStorei(GL_UNPACK_ALIGNMENT, _unpackAlignment);
    }
    update(width, height, depth, data);
  }
  void Texture3D::update( int width, int height, int depth, void* data )
  {
    this->bind();
    glTexImage3D(
      this->_target,
      this->_level,
      this->_internalFormat,
      width,
      height,
      depth,
      0,
      this->_format,
      this->_type,
      data
    );
    this->unbind();
  }
  Texture3D::~Texture3D( void )
  {
  }
  void Texture3D::load( void )
  {
    if ( !this->_loaded )
    {
      glGenTextures(1, &this->_handler);
      glBindTexture( this->_target, this->_handler );
      this->_loaded = true;
    }
  }

  TextureManager& TextureManager::getInstance()
  {
    static TextureManager instance; // Guaranteed to be destroyed.
                                    // Instantiated on first use.
    return instance;
  }

  void TextureManager::add( const std::string& alias, Texture* tex )
  {
    this->_textures[ alias ] = tex;
  }

  void TextureManager::remove( const std::string& alias )
  {
    this->_textures.erase( alias );
  }

  Texture* TextureManager::get( const std::string& alias )
  {
    return this->_textures[ alias ];
  }

  TextureManager::~TextureManager( )
  {
    for(auto& pair : this->_textures) delete pair.second;
    this->_textures.clear( );
  }

};
