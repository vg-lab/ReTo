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

/***
 * TODO:
 *  -glDispatchComputeIndirect(GLintptr indirect);
 *  -TransformFeedback in vertex and geometry shaders
 *  -Check dont used uniforms
 *  -Geometry I/O don´t work now
 */

#ifndef __RETO__PROGRAM_SHADER__
#define __RETO__PROGRAM_SHADER__

#include <map>
#include <map>
#include <string>
#include <vector>

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

#define RETO_GEOMETRY_SHADERS
#define RETO_COMPUTE_SHADERS
#define RETO_TESSELATION_SHADERS
#define RETO_SUBPROGRAMS
#define RETO_OCC_QUERY

#ifdef RETO_OCC_QUERY
  #include <functional>
#endif

#include <reto/api.h>

namespace reto
{

  class ShaderProgram
  {
  public:
    RETO_API
    ShaderProgram(void);
    RETO_API
    ~ShaderProgram( void );

    /**
     * Method to load and add a vertex and fragment shaders from string
     * @param source: Vertex shader source
     * @param source: Fragment shader source
     * @return Shader loaded
     */
    RETO_API
    bool load( const std::string& vsFile, const std::string& fsFile );
    /**
     * Method to load and add a vertex shader from string
     * @param source: Shader source
     * @return Shader loaded
     */
    RETO_API
    bool loadVertexShader( const std::string& file );
    /**
     * Method to load and add a fragment shader from string
     * @param source: Shader source
     * @return Shader loaded
     */
    RETO_API
    bool loadFragmentShader( const std::string& file );
  #ifdef RETO_GEOMETRY_SHADERS
    /**
     * Method to load and add a geometry shader from string
     * @param source: Shader source
     * @return Shader loaded
     */
    RETO_API
    bool loadGeometryShader( const std::string& file );
  #endif
  #ifdef RETO_TESSELATION_SHADERS
    /**
     * Method to load and add a tesselation evaluation shader from string
     * @param source: Shader source
     * @return Shader loaded
     */
    RETO_API
    bool loadTesselationEvaluationShader( const std::string& file );
    /**
     * Method to load and add a tesselation control shader from string
     * @param source: Shader source
     * @return Shader loaded
     */
    RETO_API
    bool loadTesselationControlShader( const std::string& file );
  #endif
  #ifdef RETO_COMPUTE_SHADERS
    /**
     * Method to load and add a computer shader from string
     * @param source: Shader source
     * @return Shader loaded
     */
    RETO_API
    bool loadComputeShader( const std::string& file );
  #endif
    /**
     * Method to load and add a vertex and fragment shaders from text
     * @param source: Vertex shader source
     * @param source: Fragment shader source
     * @return Shader loaded
     */
    RETO_API
    bool loadFromText( const std::string& vsSource,
                       const std::string& fsSource );
    /**
     * Method to load and add a vertex shader from text
     * @param source: Shader source
     * @return Shader loaded
     */
    RETO_API
    bool loadVertexShaderFromText( const std::string& source );
    /**
     * Method to load and add a fragment shader from text
     * @param source: Shader source
     * @return Shader loaded
     */
    RETO_API
    bool loadFragmentShaderFromText( const std::string& source );
  #ifdef RETO_GEOMETRY_SHADERS
    /**
     * Method to load and add a geometry shader from text
     * @param source: Shader source
     * @return Shader loaded
     */
    RETO_API
    bool loadGeometryShaderFromText( const std::string& source );
  #endif
  #ifdef RETO_TESSELATION_SHADERS
    /**
     * Method to load and add a tesselation evaluation shader from text
     * @param source: Shader source
     * @return Shader loaded
     */
    RETO_API
    bool loadTesselationEvaluationShaderFromText( const std::string& source );
    /**
     * Method to load and add a tesselation control shader from text
     * @param source: Shader source
     * @return Shader loaded
     */
    RETO_API
    bool loadTesselationControlShaderFromText( const std::string& source );
  #endif
  #ifdef RETO_COMPUTE_SHADERS
    /**
     * Method to load and add a computer shader from text
     * @param source: Shader source
     * @return Shader loaded
     */
    RETO_API
    bool loadComputeShaderFromText( const std::string& source );
  #endif

    /**
     * Method to compile a program
     * @return If program compile and link OK
     */
    RETO_API
    bool compileAndLink( void );
    /**
     * Method to get Program id
     * @return program identifier.
     */
    RETO_API
    GLuint program( void );

    /**
     * Method to enable a program
     */
    RETO_API
    void use( void );
    /**
     * Method to disable a program (not necessary)
     */
    RETO_API
    void unuse( void );

    /**
     * Method to catching an attribute value of a vertex shader
     * @param attr: Attribute name
     */
    RETO_API
    void addAttribute( const std::string& attr );

    /**
     * Method to catching an array of attribute values of a vertex shader
     * @param attrs: Attribute vector names
     */
    RETO_API
    void addAttributes( const std::vector<char*> attrs );
    /**
     * Method to bind a specific index to a attribute value
     * @param attr: Attribute name
     * @param index: Attribute index
     */
    RETO_API
    void bindAttribute( const std::string& attr, GLuint index );
    /**
     * Method to catching an uniform value.
     * @param uniform: Uniform name
     */
    RETO_API
    void addUniform( const std::string& uniform );
    /**
     * Method to catching an array of uniform values.
     * @param uniforms: Uniform vector names
     */
    RETO_API
    void addUniforms( const std::vector<char*> uniforms );
    /**
     * Method to bind a specific index to a uniform value
     * @param uniform: Uniform name
     * @param index: Uniform index
     */
    RETO_API
    void bindUniform( const std::string& uniform, GLuint index );
    /**
     * Method to catching an uniform buffer object
     * @param _ubo: Uniform Buffer Object name
     */
    RETO_API
    void addUbo( const std::string& _ubo );

    #ifdef RETO_SUBPROGRAMS
    /**
     * Method to cathing a subprogram in a specific kind of shader
     * @param name: Subprogram name
     * @param shaderType: OpenGL Shader type
     */
      RETO_API
      void addSubroutine( const std::string& name, GLenum shaderType );
    #endif

    /**
     * Method to get a attribute index in cache
     * @param _attr: Attribute name
     * @return Attribute index
     */
    RETO_API
    GLuint attribute( const std::string& _attr );
    /**
     * Method to get a uniform index in cache
     * @param _unif: Uniform name
     * @return Uniform index
     */
    RETO_API
    GLuint uniform( const std::string& _unif );
    /**
     * Method to get a Uniform Buffer Object index in cache
     * @param _ubo: Uniform Buffer Object name
     * @return Uniform Buffer Object index
     */
    RETO_API
    GLuint ubo( const std::string& _ubo );
    /**
     * Method to get a subprogram index of a specific kind of shader 
     *    in cache
     * @param name: Subprogram name
     * @param shaderType: OpenGL Shader type
     * @return Subprogram index
     */
    RETO_API
    GLuint subprogram( const std::string& name, GLenum shaderType );
    /**
     * Method to get a attribute index in cache
     * @param _attr: Attribute name
     * @return Attribute index
     */
    RETO_API
    GLuint operator( )( const std::string& _attr );
    /**
     * Method to get a uniform index in cache
     * @param _unif: Uniform name
     * @return Uniform index
     */
    GLuint operator[]( const std::string& _unif );

    /**
     * Method to send a boolean
     * @param uniform: Uniform name
     * @param val: Boolean data
     */
    RETO_API
    void sendUniformb( const std::string& uniform, GLboolean val );
    /**
     * Method to send an integer
     * @param uniform: Uniform name
     * @param v: Int data
     */
    RETO_API
    void sendUniformi( const std::string& uniform, GLint val );
    /**
     * Method to send an unsigned integer
     * @param uniform: Uniform name
     * @param val: Unsigned integer data
     */
    RETO_API
    void sendUniformu( const std::string& uniform, GLuint val );
    /**
     * Method to send a float
     * @param uniform: Uniform name
     * @param val: Float data
     */
    RETO_API
    void sendUniformf( const std::string& uniform, GLfloat val );

    /**
     * Method to send a vec3 with separated data
     * @param uniform: Uniform name
     * @param x: First element
     * @param y: Second element
     * @param z: Third element
     */
    RETO_API
    void sendUniform( const std::string& uniform, float x, float y, float z );
    /**
     * Method to send a vec2
     * @param uniform: Uniform name
     * @param v: Data
     */
    RETO_API
    void sendUniform2v( const std::string& uniform,
                        const std::vector< float > & v );
    /**
     * Method to send a vec3
     * @param uniform: Uniform name
     * @param v: Data
     */
    RETO_API
    void sendUniform3v( const std::string& uniform,
                        const std::vector< float > & v );
    /**
     * Method to send a vec4
     * @param uniform: Uniform name
     * @param v: Data
     */
    RETO_API
    void sendUniform4v( const std::string& uniform,
                        const std::vector< float > & v );
    /**
     * Method to send a mat3
     * @param uniform: Uniform name
     * @param v: Data
     */
    RETO_API
    void sendUniform3m( const std::string& uniform,
                        const std::vector< float > & m );
    /**
     * Method to send a mat4
     * @param uniform: Uniform name
     * @param v: Data
     * @param inverse: Specifies whether to transpose 
     *    the matrix as the values are loaded into the uniform variable
     */
    RETO_API
    void sendUniform4m( const std::string& uniform,
                        const std::vector< float > & m,
                        GLboolean inverse = GL_FALSE );

    #ifdef RETO_SUBPROGRAMS
      /**
       * Method to active a subprogram in a specific shader
       * @param name: Subprogram name
       * @param shaderType: OpenGL Shader type 
       */
      RETO_API
      void activeSubprogram( const std::string& name, GLenum shaderType );
    #endif

    #ifdef RETO_OCC_QUERY
      /**
       * Method to check if object is visible in frustrum
       * @return object is visible
       */
      RETO_API
      bool occlusionQuery( std::function<void( )> renderFunc );
    #endif

    #ifdef RETO_COMPUTE_SHADERS
      /**
       * Method to launch one or more compute work groups.
       * @param numGroupX: The number of work groups to be launched in the X dimension.
       * @param numGroupY: The number of work groups to be launched in the Y dimension.
       * @param numGroupZ: The number of work groups to be launched in the Z dimension. 
       */
      RETO_API
      void launchComputeWork( GLuint numGroupX, GLuint numGroupY, GLuint numGroupZ );
    #endif
    #ifdef RETO_TESSELATION_SHADERS
      /**
       * Method to get patch vertices.
       * @return Num of patches
       */
     RETO_API
      GLuint getPatchVertices( void );
      /**
       * Method to get inner tesselation level.
       * @return Inner level
       */
      RETO_API
      GLfloat getInnerLevel( void );
      /**
       * Method to get outer tesselation level.
       * @return Outer level
       */
     RETO_API
      GLfloat getOuterLevel( void );

      /**
       * Method to set patch vertices.
       * @param n: Num of patches
       */
      RETO_API
      void setPatchVertices( GLuint n );
      /**
       * Method to set inner tesselation level.
       * @param l: Inner level
       */
      RETO_API
      void setInnerLevel( GLfloat l );
      /**
       * Method to set outer tesselation level.
       * @param l: Outer level
       */
      RETO_API
      void setOuterLevel( GLfloat l );
    #endif
    #ifdef RETO_GEOMETRY_SHADERS
      /**
       * Method to get max output vertices.
       * @return GLint: Máx. num of vertices
       */
      RETO_API
      GLint getGeometryMaxOutput( void );
      /**
       * Method to get max input geometry type.
       * @return GLint: Input geometry type
       */
      RETO_API
      GLint getGeometryInputType( void );
      /**
       * Method to get output geometry type.
       * @return GLint: Output geometry type
       */
      RETO_API
      GLint getGeometryOutputType( void );

      /**
       * Method to set max output vertices.
       * @param maxValue: Max. num of vertices
       */
      RETO_API
      void setGeometryMaxOutput( GLuint maxValue );
      /**
       * Method to set input geometry type.
       * @param inputValue: Input geometry type
       */
      RETO_API
      void setGeometryInputType( GLuint inputValue );
      /**
       * Method to set output geometry type.
       * @param outputType: Output geometry type
       */
      RETO_API
      void setGeometryOutputType( GLuint outputType );
    #endif

    /**
     * Method to create program and attach all shaders
     */
    RETO_API
    void create( void );
    /**
     * Method to link program and check status 
     */
    RETO_API
    bool link( void );
  protected:
    void destroy( );
    bool load( const std::string& file, GLenum type );
    bool loadFromText( const std::string& source, GLenum type );

    GLuint _program;
    std::map<std::string, GLuint> _attrsList;
    std::map<std::string, GLuint> _uniformList;
    std::map<std::string, GLuint> _uboList;

    #ifdef RETO_SUBPROGRAMS
      typedef struct SubProgram
      {
        const char* name;
        GLuint index;
        SubProgram( const char* n, GLuint i )
        {
          this->name = n;
          this->index = i;
        }
      } SubProgram;
      std::multimap<GLenum, SubProgram> _subprograms;
    #endif
    std::vector<GLuint> _shaders;

    #ifdef RETO_OCC_QUERY
      GLuint _occQuery;
    #endif
  };
}
#endif // __RETO__PROGRAM_SHADER__
