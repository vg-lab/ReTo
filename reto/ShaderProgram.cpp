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

#include "ShaderProgram.h"
#include <fstream>
#include <iostream>


//OpenGL
#ifndef SKIP_GLEW_INCLUDE
#include <GL/glew.h>
#endif
#ifdef Darwin
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif


#include <limits>       // std::numeric_limits

#define MACRO_SP_WARNING(name, type) std::cerr << "WARNING: '" << name << "' " << type << " doesn't exist or appears unused." << std::endl;


namespace reto
{
  ShaderProgram::ShaderProgram( void )
  {
    _program = -1;
    _attrsList.clear( );
    _uniformList.clear( );
    _uboList.clear( );
    #ifdef RETO_SUBPROGRAMS
      _subprograms.clear( );
    #endif
    _shaders.clear( );

    _isLinked = false;

    #ifdef RETO_OCC_QUERY
      // Occlusion query object
      // TODO: You need to call this after OpenGL context creation
      //glGenQueries(1, &_occQuery);
    #endif
  }

  ShaderProgram::~ShaderProgram( void )
  {
    _destroy( );
  }

  bool ShaderProgram::isLinked( void )
  {
    return this->_isLinked;
  }

  bool ShaderProgram::isUniformCached( const std::string& unif )
  {
    return this->_uniformList.find(unif) != this->_uniformList.end( );
  }
  bool ShaderProgram::isAttributeCached( const std::string& attr )
  {
    return this->_attrsList.find(attr) != this->_attrsList.end( );
  }

  bool ShaderProgram::loadFromText( const std::string& vsSource,
                                    const std::string& fsSource )
  {
    return ( _loadFromText( vsSource, GL_VERTEX_SHADER ) &&
             _loadFromText( fsSource, GL_FRAGMENT_SHADER ));
  }

  bool ShaderProgram::_loadFromText( const std::string& source, int type )
  {
    // Create and compile shader
    unsigned int shader;
    shader = glCreateShader( type );
    const char* cStr = source.c_str( );
    glShaderSource( shader, 1, &cStr, nullptr );

    int status;
    glCompileShader( shader );
    glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
    if (status == GL_FALSE )
    {
      int infoLogLength;
      glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLogLength );
      GLchar* infoLog = new GLchar[ infoLogLength ];
      glGetShaderInfoLog( shader, infoLogLength, nullptr, infoLog );
      std::cerr << "Compile log: " << infoLog << std::endl;
      delete [ ] infoLog;
      return false;
    }

    // Add to shaders in use
    _shaders.push_back( shader );
    return true;
  }

  bool ShaderProgram::loadVertexShaderFromText( const std::string& source )
  {
    return _loadFromText( source, GL_VERTEX_SHADER );
  }

  bool ShaderProgram::loadFragmentShaderFromText( const std::string& source )
  {
    return _loadFromText( source, GL_FRAGMENT_SHADER );
  }

  #ifdef RETO_GEOMETRY_SHADERS
    bool ShaderProgram::loadGeometryShaderFromText( const std::string& source )
    {
      return _loadFromText( source, GL_GEOMETRY_SHADER );
    }
  #endif

  #ifdef RETO_TESSELATION_SHADERS
    bool ShaderProgram::loadTesselationEvaluationShaderFromText(
      const std::string& source )
    {
      return _loadFromText( source, GL_TESS_EVALUATION_SHADER );
    }

    bool ShaderProgram::loadTesselationControlShaderFromText(
      const std::string& source )
    {
      return _loadFromText(source, GL_TESS_CONTROL_SHADER);
    }
  #endif

  #ifdef RETO_COMPUTE_SHADERS
    bool ShaderProgram::loadComputeShaderFromText( const std::string& source )
    {
      return _loadFromText( source, GL_COMPUTE_SHADER );
    }
  #endif

  #ifdef RETO_TRANSFORM_FEEDBACK
    void ShaderProgram::feedbackVarying( const char** varyings, int num, int mode )
    {
      if (_isLinked)
      {
        throw "Call this function just before linked.";
      }
      glTransformFeedbackVaryings( _program, num, varyings, mode );
    }
  #endif

  bool ShaderProgram::_load( const std::string& fileName, int type )
  {
    unsigned int fileLen;

    // Load file
    std::ifstream file;
    file.open( fileName.c_str( ), std::ios::in );
    if ( !file )
    {
      std::cout << "File " << fileName << " not found" << std::endl;
      return false;
    }

    // File length
    file.seekg( 0, std::ios::end );
    fileLen = int( file.tellg( ));
    file.seekg( std::ios::beg );

    // Read the file
    char* source = new char[fileLen + 1];

    int i = 0;
    while (file.good( ))
    {
      source[ i ] = char( file.get( ));
      if ( !file.eof( )) i++;
      else fileLen = i;
    }
    source[fileLen] = '\0';
    file.close( );

    // Create and compile shader
    unsigned int shader;
    shader = glCreateShader( type );
    glShaderSource( shader, 1, ( const GLchar** ) &source,
                    ( const int* ) &fileLen );

    int status;
    glCompileShader( shader );
    glGetShaderiv ( shader, GL_COMPILE_STATUS, &status );
    if ( status == GL_FALSE )
    {
      int infoLogLength;
      glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &infoLogLength );
      GLchar *infoLog= new GLchar[infoLogLength];
      glGetShaderInfoLog( shader, infoLogLength, nullptr, infoLog );
      std::cerr << "Compile log ("<< fileName << "): " << infoLog << std::endl;
      delete [ ] infoLog;
      return false;
    }
    delete[] source;

    // Add to shaders in use
    _shaders.push_back( shader );
    return true;
  }

  bool ShaderProgram::load(const std::string& vsFile, const std::string& fsFile)
  {
    return ( _load( vsFile, GL_VERTEX_SHADER) &&
             _load( fsFile, GL_FRAGMENT_SHADER ));
  }

  bool ShaderProgram::loadVertexShader( const std::string& file )
  {
    return _load( file, GL_VERTEX_SHADER );
  }

  bool ShaderProgram::loadFragmentShader(const std::string& file)
  {
    return _load( file, GL_FRAGMENT_SHADER );
  }

  #ifdef RETO_GEOMETRY_SHADERS
    bool ShaderProgram::loadGeometryShader( const std::string& file )
    {
      return _load( file, GL_GEOMETRY_SHADER );
    }
  #endif

  #ifdef RETO_TESSELATION_SHADERS
    bool ShaderProgram::loadTesselationEvaluationShader(
      const std::string& file )
    {
      return _load( file, GL_TESS_EVALUATION_SHADER );
    }

    bool ShaderProgram::loadTesselationControlShader( const std::string& file )
    {
      return _load( file, GL_TESS_CONTROL_SHADER );
    }
  #endif

  #ifdef RETO_COMPUTE_SHADERS
    bool ShaderProgram::loadComputeShader( const std::string& file )
    {
      return _load( file, GL_COMPUTE_SHADER );
    }
  #endif

  void ShaderProgram::_destroy( void )
  {
    _program = -1;
    _attrsList.clear( );
    _uniformList.clear( );
    _uboList.clear( );

    #ifdef RETO_SUBPROGRAMS
      _subprograms.clear( );
    #endif
    _shaders.clear( );
    size_t size = _shaders.size( );
    for( size_t i = 0; i < size; ++i )
    {
      if( _shaders[ i ] != 0 )
      {
        glDetachShader( _program, _shaders[ i ] );
      }
    }
    glDeleteProgram( _program );
  }

  void ShaderProgram::create( void )
  {
    _program = glCreateProgram( );
    size_t size = _shaders.size( );
    for( size_t i = 0; i < size; i++ )
    {
      if( _shaders[ i ] != 0 )
      {
        glAttachShader( _program, _shaders[ i ] );
      }
    }
  }

  bool ShaderProgram::link( void )
  {
    // link and check whether the program links fine
    int status;
    glLinkProgram( _program );
    glGetProgramiv( _program, GL_LINK_STATUS, &status );
    if ( status == GL_FALSE )
    {
      int infoLogLength;

      glGetProgramiv( _program, GL_INFO_LOG_LENGTH, &infoLogLength );
      GLchar* infoLog = new GLchar[infoLogLength];
      glGetProgramInfoLog( _program, infoLogLength, nullptr, infoLog );
      std::cerr << "Link log: " << infoLog << std::endl;
      delete [ ] infoLog;
      return false;
    }
    this->_isLinked = true;
    return true;
  }

  void ShaderProgram::use( void )
  {
    glUseProgram( _program );
  }

  void ShaderProgram::unuse( void )
  {
    glUseProgram( -1 );
  }

  bool ShaderProgram::compileAndLink( void )
  {
    create( );
    return link( );
  }

  unsigned int ShaderProgram::program( void )
  {
    return _program;
  }

  void ShaderProgram::addAttribute( const std::string& attr )
  {
    unsigned int index = glGetAttribLocation( _program, attr.c_str( ) );
    if( index != std::numeric_limits< unsigned int >::max( ) )
    {
      _attrsList[ attr ] = index;
    }
    else
    {
      MACRO_SP_WARNING( attr, "attribute" )
    }
  }

  void ShaderProgram::addAttributes( const std::vector<char*> attrs )
  {
    for( auto& a: attrs )
    {
      addAttribute( a );
    }
  }

  void ShaderProgram::addUniform( const std::string& uniformName )
  {
    unsigned int index = glGetUniformLocation( _program, uniformName.c_str( ) );
    if( index != std::numeric_limits<unsigned int>::max( ) )
    {
      _uniformList[ uniformName ] = index;
    }
    else
    {
      MACRO_SP_WARNING( uniformName, "uniform" );
    }
  }

  void ShaderProgram::addUniforms( const std::vector<char*> uniforms )
  {
    for( auto& u: uniforms )
    {
      addUniform( u );
    }
  }

  void ShaderProgram::addUbo( const std::string& uboName )
  {
    unsigned int index = glGetUniformBlockIndex( _program, uboName.c_str( ) );
    if( index != std::numeric_limits< unsigned int >::max( ) ) {
      _uboList[ uboName ] = index;
    }
    else
    {
      MACRO_SP_WARNING( uboName, "ubo" );
    }
  }

  #ifdef RETO_SUBPROGRAMS
    void ShaderProgram::addSubroutine( const std::string& name,
                                       int shaderType )
    {
      unsigned int idx = glGetSubroutineIndex( _program, shaderType, name.c_str( ) );
      if( idx != std::numeric_limits< unsigned int >::max( ) )
      {
        auto sub = SubProgram( name.c_str( ), idx );
        _subprograms.insert( std::pair< int, SubProgram >( shaderType, sub ) );
      }
      else
      {
        MACRO_SP_WARNING( name, "subprogram" );
      }
    }
  #endif

  void ShaderProgram::bindAttribute( const std::string& attr, unsigned int index )
  {
    glBindAttribLocation( _program, index, attr.c_str( ) );
  }

  int ShaderProgram::attribute( const std::string& attr )
  {
    auto it = _attrsList.find( attr );
    if ( it != _attrsList.end( ) )
    {
      return it->second;
    }
    else
    {
      return -1;
    }
  }

  int ShaderProgram::operator( )( const std::string& attr )
  {
    return  attribute( attr );
  }

  int ShaderProgram::uniform( const std::string& uniformName )
  {
    auto it = _uniformList.find( uniformName );
    if ( it != _uniformList.end( ) )
    {
      return it->second;
    }
    else
    {
      return -1;
    }
  }

  int ShaderProgram::operator[ ]( const std::string& attr )
  {
    return  uniform( attr );
  }

  int ShaderProgram::ubo( const std::string& _ubo )
  {

    auto it = _uboList.find( _ubo );
    if ( it != _uboList.end( ) )
    {
      return it->second;
    }
    else
    {
      return -1;
    }
  }

  void ShaderProgram::autocatching( bool attributes, bool uniforms )
  {
    int count;

    int size; // Variable size
    GLenum type; // Variable type (float, vecX, matX, ...)

    const GLsizei bufSize = 32;
    char name[bufSize]; // GLSL variable name
    int length; // Name length

    if ( attributes )
    {
      glGetProgramiv( this->_program, GL_ACTIVE_ATTRIBUTES, &count);
      //printf("Active Attributes: %d\n", count);

      for (auto i = 0; i < count; ++i)
      {
        glGetActiveAttrib( this->_program, (GLuint)i, bufSize, &length,
            &size, &type, name);

        this->addAttribute( name );

        //printf("Attribute #%d Type: %u Name: %s\n", i, type, name);
      }
    }

    if ( uniforms )
    {
      glGetProgramiv( this->_program, GL_ACTIVE_UNIFORMS, &count );
      //printf("Active Uniforms: %d\n", count);

      for (auto i = 0; i < count; ++i)
      {
        glGetActiveUniform( this->_program, (GLuint)i, bufSize, &length,
            &size, &type, name );

        this->addUniform( name );

        //printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
      }

    }
  }

  #ifdef RETO_SUBPROGRAMS
    int ShaderProgram::subprogram( const std::string& name,
                                      int shaderType )
    {
      std::pair<std::multimap<int, SubProgram>::iterator,
                std::multimap<int, SubProgram>::iterator> ret;
      ret = _subprograms.equal_range( shaderType );
      for( std::multimap<int, SubProgram>::iterator it = ret.first;
           it != ret.second; it++ )
      {
        if( it->second.name == name )
        {
          return it->second.index;
        }
      }
      std::cerr << "Subroutine not found" << std::endl;
      return -1;
    }
  #endif

  void ShaderProgram::bindUniform( const std::string& unif,
                                   unsigned int idx )
  {
    if( _uniformList.find( unif ) == _uniformList.end( ) )
    {
      _uniformList[ unif ] = idx;
    }
    else
    {
      std::cerr << "Uniform exist" << std::endl;
    }
  }

  void ShaderProgram::sendUniform( const std::string& uniformName,
                                   float x, float y, float z )
  {
    int loc = uniform( uniformName );
    if (loc >= 0) glUniform3f( loc, x, y, z );
  }

  void ShaderProgram::sendUniform2v( const std::string& uniformName,
                                     const std::vector< float >& data )
  {
    int loc = uniform( uniformName );
    if (loc >= 0) glUniform2fv( loc, 1, data.data( ) );
  }

  void ShaderProgram::sendUniform2v( const std::string& uniformName,
                                     const float* data )
  {
    int loc = uniform( uniformName );
    if (loc >= 0) glUniform2fv( loc, 1, data );
  }

  void ShaderProgram::sendUniform3v( const std::string& uniformName,
                                     const std::vector< float >& data )
  {
    int loc = uniform( uniformName );
    if (loc >= 0) glUniform3fv( loc, 1, data.data( ) );
  }

  void ShaderProgram::sendUniform3v( const std::string& uniformName,
                                     const float* data )
  {
    int loc = uniform( uniformName );
    if (loc >= 0) glUniform3fv( loc, 1, data );
  }

  void ShaderProgram::sendUniform4v( const std::string& uniformName,
                                     const std::vector< float >& data )
  {
    int loc = uniform( uniformName );
    if (loc >= 0) glUniform4fv( loc, 1, data.data( ) );
  }

  void ShaderProgram::sendUniform4v( const std::string& uniformName,
                                     const float* data )
  {
    int loc = uniform( uniformName );
    if (loc >= 0) glUniform4fv( loc, 1, data );
  }

  void ShaderProgram::sendUniform2iv( const std::string& uniformName,
                                      const unsigned int* data )
  {
    int loc = uniform( uniformName );
    if (loc >= 0) glUniform2i( loc, data[0], data[1]);
  }

  void ShaderProgram::sendUniform2iv( const std::string& uniformName,
                                      const std::vector< unsigned int > & data )
  {
    int loc = uniform( uniformName );
    if (loc >= 0) glUniform2i( loc, data[0], data[1]);
  }

  void ShaderProgram::sendUniform3iv( const std::string& uniformName,
                                      const unsigned int* data )
  {
    int loc = uniform( uniformName );
    if (loc >= 0) glUniform3i( loc, data[0], data[1], data[2]);
  }

  void ShaderProgram::sendUniform3iv( const std::string& uniformName,
                                      const std::vector< unsigned int > & data )
  {
    int loc = uniform( uniformName );
    if (loc >= 0) glUniform3i( loc, data[0], data[1], data[2]);
  }

  void ShaderProgram::sendUniform4iv( const std::string& uniformName,
                                      const unsigned int* data )
  {
    int loc = uniform( uniformName );
    if (loc >= 0) glUniform4i( loc, data[0], data[1], data[2], data[3]);
  }

  void ShaderProgram::sendUniform4iv( const std::string& uniformName,
                                      const std::vector< unsigned int > & data )
  {
    int loc = uniform( uniformName );
    if (loc >= 0) glUniform4i( loc, data[0], data[1], data[2], data[3]);
  }

  void ShaderProgram::sendUniform4m( const std::string& uniformName,
    const std::vector< float > & data, bool inverse )
  {
    int loc = uniform( uniformName );
    if (loc >= 0) glUniformMatrix4fv( loc, 1, inverse, data.data( ) );
  }

  void ShaderProgram::sendUniform4m( const std::string& uniformName,
    const float* data, bool inverse )
  {
    int loc = uniform( uniformName );
    if (loc >= 0) glUniformMatrix4fv( loc, 1, inverse, data );
  }

  void ShaderProgram::sendUniform3m( const std::string& uniformName,
                                     const std::vector< float > & data )
  {
    int loc = uniform( uniformName );
    if (loc >= 0) glUniformMatrix3fv( loc, 1, GL_FALSE, data.data( ) );
  }

  void ShaderProgram::sendUniform3m( const std::string& uniformName,
                                     const float* data )
  {
    int loc = uniform( uniformName );
    if (loc >= 0) glUniformMatrix3fv( loc, 1, GL_FALSE, data );
  }

  void ShaderProgram::sendUniformf( const std::string& uniformName,
                                    float val )
  {
    int loc = uniform( uniformName );
    if (loc >= 0) glUniform1f( loc, val );
  }

  void ShaderProgram::sendUniformi( const std::string& uniformName, int val )
  {
    int loc = uniform( uniformName );
    if (loc >= 0) glUniform1i( loc, val );
  }

  void ShaderProgram::sendUniformb( const std::string& uniformName,
                                    bool val)
  {
    int loc = uniform( uniformName );
    if (loc >= 0) glUniform1i( loc, val );
  }

  void ShaderProgram::sendUniformu( const std::string& uniformName, unsigned int val )
  {
    int loc = uniform( uniformName );
    if (loc >= 0) glUniform1ui( loc, val );
  }

  #ifdef RETO_SUBPROGRAMS
    void ShaderProgram::activeSubprogram( const std::string& name,
                                          int shaderType )
    {
      std::pair<std::multimap<int, SubProgram>::iterator,
                std::multimap<int, SubProgram>::iterator> ret;
      ret = _subprograms.equal_range( shaderType );
      for( std::multimap<int, SubProgram>::iterator it = ret.first;
           it != ret.second; it++ )
      {
        if(it->second.name == name)
        {
          glUniformSubroutinesuiv( shaderType, 1, &it->second.index );
          return;
        }
      }
      std::cerr << "Subroutine not found" << std::endl;
    }
  #endif

  #ifdef RETO_COMPUTE_SHADERS
    void ShaderProgram::launchComputeWork( unsigned int numGroupX, unsigned int numGroupY,
                                           unsigned int numGroupZ )
    {
      glDispatchCompute( numGroupX, numGroupY, numGroupZ );
    }
  #endif

  #ifdef RETO_TESSELATION_SHADERS

    unsigned int ShaderProgram::getPatchVertices( void )
    {
      int n;
      glGetIntegerv( GL_PATCH_VERTICES, &n );
      return n;
    }
    float ShaderProgram::getInnerLevel( void )
    {
      float l;
      glGetFloatv( GL_PATCH_DEFAULT_INNER_LEVEL, &l );
      return l;
    }
    float ShaderProgram::getOuterLevel( void )
    {
      float l;
      glGetFloatv( GL_PATCH_DEFAULT_OUTER_LEVEL, &l );
      return l;
    }

    void ShaderProgram::setPatchVertices( unsigned int n )
    {
      glPatchParameteri( GL_PATCH_VERTICES, n );
    }
    void ShaderProgram::setInnerLevel( float level )
    {
      glPatchParameterfv( GL_PATCH_DEFAULT_INNER_LEVEL, &level );
    }
    void ShaderProgram::setOuterLevel( float level )
    {
      glPatchParameterfv( GL_PATCH_DEFAULT_OUTER_LEVEL, &level );
    }
  #endif

  #ifdef RETO_OCC_QUERY
    bool ShaderProgram::occlusionQuery( std::function<void( )> renderFunc )
    {
      // Disable writing to the color buffer and depth buffer.
      // We just wanna check if they would be rendered, not actually render them
      glColorMask( false, false, false, false );
      glDepthMask( GL_FALSE );
      glBeginQuery( GL_SAMPLES_PASSED, _occQuery );
      renderFunc( );
      glEndQuery( GL_SAMPLES_PASSED );
      int samplesPassed = 0;
      glGetQueryObjectiv( _occQuery, GL_QUERY_RESULT, &samplesPassed );
      // Re-enable writing to color buffer and depth buffer
      glColorMask( true, true, true, true );
      glDepthMask( GL_TRUE );
      return samplesPassed > 0;
    }
  #endif

  #ifdef RETO_GEOMETRY_SHADERS
    void ShaderProgram::setGeometryMaxOutput( unsigned int maxOutput )
    {
      glProgramParameteri( _program, GL_GEOMETRY_VERTICES_OUT, maxOutput );
    }
    void ShaderProgram::setGeometryInputType( unsigned int inputType )
    {
      glProgramParameteri( _program, GL_GEOMETRY_INPUT_TYPE, inputType );
    }
    void ShaderProgram::setGeometryOutputType( unsigned int outputType )
    {
      glProgramParameteri( _program, GL_GEOMETRY_OUTPUT_TYPE, outputType );
    }

    int ShaderProgram::getGeometryMaxOutput( void )
    {
      int maxOutput;
      glGetProgramiv( _program, GL_GEOMETRY_VERTICES_OUT, &maxOutput );
      return maxOutput;
    }

    int ShaderProgram::getGeometryInputType( void )
    {
      int inputType;
      glGetProgramiv( _program, GL_GEOMETRY_INPUT_TYPE, &inputType );
      return inputType;
    }

    int ShaderProgram::getGeometryOutputType( void )
    {
      int outputType;
      glGetProgramiv( _program, GL_GEOMETRY_OUTPUT_TYPE, &outputType );
      return outputType;
    }
  #endif
}
