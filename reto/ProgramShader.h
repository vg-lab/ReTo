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
 *  -Methods to get tess and geom I/O
 */
 
#ifndef _PROGRAMSHADER_H_ 
#define _PROGRAMSHADER_H_ 
 
#include <map> 
#include <map> 
#include <string> 
#include <vector>

//OpenGL
#ifndef SKIP_GLEW_INCLUDE
#include <GL/glew.h>
#endif
#ifdef Darwin
#include <gl.h>
#include <glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#define GEOMETRY_SHADERS
#define COMPUTE_SHADERS
#define TESSELATION_SHADERS
#define SUBPROGRAMS
#define OCC_QUERY

#ifdef OCC_QUERY
  #include <functional>
#endif
 
namespace reto
{

  class ProgramShader { 
  public: 
    ProgramShader(void); 
    ~ProgramShader(void); 
   
    void destroy(); 
     
    bool load(const std::string& vsFile, const std::string& fsFile); 
    bool load(const std::string& file, GLenum type); 
    bool loadVS(const std::string& file);
    bool loadFS(const std::string& file);
  #ifdef GEOMETRY_SHADERS
    bool loadGS(const std::string& file);
  #endif
  #ifdef TESSELATION_SHADERS
    bool loadTES(const std::string& file);
    bool loadTCS(const std::string& file);
  #endif
  #ifdef COMPUTE_SHADERS 
    bool loadCS(const std::string& file);
  #endif
    bool loadFromText(const std::string& vsSource, const std::string& fsSource); 
    bool loadFromText(const std::string& source, GLenum type); 
    bool loadFromTextVS(const std::string& source);
    bool loadFromTextFS(const std::string& source);
  #ifdef GEOMETRY_SHADERS
    bool loadFromTextGS(const std::string& source);
  #endif
  #ifdef TESSELATION_SHADERS
    bool loadFromTextTES(const std::string& source);
    bool loadFromTextTCS(const std::string& source);
  #endif
  #ifdef COMPUTE_SHADERS 
    bool loadFromTextCS(const std::string& source); 
  #endif

    bool compile_and_link();
    GLuint program();

    void use();  
    void unuse();
   
    void add_attribute(const std::string& attr);
    void add_attributes(const std::vector<char*> attrs);
    void bind_attribute(const std::string& attr, GLuint index);
    void add_uniform(const std::string& uniform);
    void add_uniforms(const std::vector<char*> uniforms);
    void bind_uniform(const std::string& uniform, GLuint index);
    void add_ubo(const std::string& _ubo);

    #ifdef SUBPROGRAMS
      void add_subroutine(const std::string& name, GLenum shaderType);
    #endif
   
    GLuint attribute(const std::string& _attr); 
    GLuint uniform(const std::string& _unif);
    GLuint ubo(const std::string& _ubo);
    GLuint subprogram(const std::string& name, GLenum shaderType);
    GLuint operator()(const std::string& _attr);
    GLuint operator[](const std::string& _unif);

    void send_uniformb(const std::string& uniform, GLboolean val);
    void send_uniformi(const std::string& uniform, GLint val);
    void send_uniformu(const std::string& uniform, GLuint val);
    void send_uniformf(const std::string& uniform, GLfloat val);

    void send_uniform(const std::string& uniform, float x, float y, float z); 
    void send_uniform2v(const std::string& uniform, const std::vector< float > & v); 
    void send_uniform3v(const std::string& uniform, const std::vector< float > & v); 
    void send_uniform4v(const std::string& uniform, const std::vector< float > & v); 
    void send_uniform3m(const std::string& uniform, const std::vector< float > & m);
    void send_uniform4m(const std::string& uniform, const std::vector< float > & m, GLboolean inverse = GL_FALSE); 

    #ifdef SUBPROGRAMS
      void active_subprogram(const std::string& name, GLenum shaderType);
    #endif

    #ifdef OCC_QUERY
      bool occlusion_query(std::function<void()> renderFunc);
    #endif

    #ifdef COMPUTE_SHADERS
      void launchComputeWork(GLuint nGx, GLuint nGy, GLuint nGz);
    #endif
    #ifdef TESSELATION_SHADERS
      GLuint getPatchVertices();
      GLfloat getInnerLevel();
      GLfloat getOuterLevel();

      void setPatchVertices(GLuint n);
      void setInnerLevel(GLfloat l);
      void setOuterLevel(GLfloat l);
    #endif
    #ifdef GEOMETRY_SHADERS
      GLint getGeometryMaxOutput();
      GLint getGeometryInputType();
      GLint getGeometryOutputType();

      void setGeometryMaxOutput(GLuint o);
      void setGeometryInputType(GLuint i);
      void setGeometryOutputType(GLuint o);
    #endif
  protected: 
    void create(); 
    bool link();

    GLuint _program;
    std::map<std::string, GLuint> _attrsList; 
    std::map<std::string, GLuint> _uniformList;
    std::map<std::string, GLuint> _uboList;

    #ifdef SUBPROGRAMS
      typedef struct SubProgram {
        const char* name;
        GLuint index;
        SubProgram(const char* n, GLuint i) {
          this->name = n;
          this->index = i;
        }
      } SubProgram;
      std::multimap<GLenum, SubProgram> _subprograms;
    #endif
    std::vector<GLuint> _shaders; 

    #ifdef OCC_QUERY
      GLuint _occQuery;
    #endif
  }; 
}
#endif /* _PROGRAMSHADER_H_ */ 

