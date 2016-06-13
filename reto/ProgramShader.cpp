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

#include "ProgramShader.h"
#include <fstream>
#include <iostream>

ProgramShader::ProgramShader(void) {
    _program = -1;
    _attrsList.clear();
    _uniformList.clear();
    _uboList.clear();
    #ifdef SUBPROGRAMS
        _subprograms.clear();
    #endif
    _shaders.clear();

    #ifdef OCC_QUERY
        // Occlusion query object
        glGenQueries(1, &_occQuery);
    #endif
}

ProgramShader::~ProgramShader(void) {
    destroy();
}

void ProgramShader::loadFromText(const std::string& vsSource, const std::string& fsSource) {
    loadFromText(vsSource, GL_VERTEX_SHADER);
    loadFromText(fsSource, GL_FRAGMENT_SHADER);
}

void ProgramShader::loadFromText(const std::string& source, GLenum type) {
    // Create and compile shader
    GLuint shader;
    shader = glCreateShader(type);
    const char *c_str = source.c_str();
    glShaderSource(shader, 1, &c_str, NULL);

    GLint status;
    glCompileShader(shader);
    glGetShaderiv (shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;        
        glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *infoLog= new GLchar[infoLogLength];
        glGetShaderInfoLog (shader, infoLogLength, NULL, infoLog);
        std::cerr << "Compile log: " << infoLog << std::endl;
        delete [] infoLog;
        return;
    }

    // Add to shaders in use
    _shaders.push_back(shader);
}

void ProgramShader::loadFromTextVS(const std::string& source) {
    loadFromText(source, GL_VERTEX_SHADER);
}

void ProgramShader::loadFromTextFS(const std::string& source) {
    loadFromText(source, GL_FRAGMENT_SHADER);
}

#ifdef GEOMETRY_SHADERS
    void ProgramShader::loadFromTextGS(const std::string& source) {
        loadFromText(source, GL_GEOMETRY_SHADER);
    }
#endif

#ifdef TESSELATION_SHADERS
    void ProgramShader::loadFromTextTES(const std::string& source) {
        loadFromText(source, GL_TESS_EVALUATION_SHADER);
    }

    void ProgramShader::loadFromTextTCS(const std::string& source) {
        loadFromText(source, GL_TESS_CONTROL_SHADER);
    }
#endif

#ifdef COMPUTE_SHADERS 
    void ProgramShader::loadFromTextCS(const std::string& source) {
        loadFromText(source, GL_COMPUTE_SHADER);
    }
#endif

void ProgramShader::load(const std::string& fileName, GLenum type) {
    unsigned int fileLen;

    // Load file
    std::ifstream file;
    file.open(fileName.c_str(), std::ios::in);
    if (!file) {
        std::cout << "File " << fileName << " not found" << std::endl;
        return;
    }

    // File length
    file.seekg(0, std::ios::end);
    fileLen = int(file.tellg());
    file.seekg(std::ios::beg);

    // Read the file
    char *source = new char[fileLen + 1];

    int i = 0;
    while (file.good()) {
        source[i] = char(file.get());
        if (!file.eof()) i++;
        else fileLen = i;
    }
    source[fileLen] = '\0';
    file.close();

    // Create and compile shader
    GLuint shader;
    shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar **) &source, (const GLint *) &fileLen);

    GLint status;
    glCompileShader(shader);
    glGetShaderiv (shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;        
        glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *infoLog= new GLchar[infoLogLength];
        glGetShaderInfoLog (shader, infoLogLength, NULL, infoLog);
        std::cerr << "Compile log ("<< fileName << "): " << infoLog << std::endl;
        delete [] infoLog;
        return;
    }
    delete source;

    // Add to shaders in use
    _shaders.push_back(shader);
}

void ProgramShader::load(const std::string& vsFile, const std::string& fsFile) {
    load(vsFile, GL_VERTEX_SHADER);
    load(fsFile, GL_FRAGMENT_SHADER);
}

void ProgramShader::loadVS(const std::string& file) {
    load(file, GL_VERTEX_SHADER);
}

void ProgramShader::loadFS(const std::string& file) {
    load(file, GL_FRAGMENT_SHADER);
}

#ifdef GEOMETRY_SHADERS
    void ProgramShader::loadGS(const std::string& file) {
        load(file, GL_GEOMETRY_SHADER);
    }
#endif

#ifdef TESSELATION_SHADERS
    void ProgramShader::loadTES(const std::string& file) {
        load(file, GL_TESS_EVALUATION_SHADER);
    }

    void ProgramShader::loadTCS(const std::string& file) {
        load(file, GL_TESS_CONTROL_SHADER);
    }
#endif

#ifdef COMPUTE_SHADERS 
    void ProgramShader::loadCS(const std::string& file) {
        load(file, GL_COMPUTE_SHADER);
    }
#endif

void ProgramShader::destroy() {
    _program = -1;
    _attrsList.clear();
    _uniformList.clear();
    _uboList.clear();

    #ifdef SUBPROGRAMS
        _subprograms.clear();
    #endif
    _shaders.clear();
    GLuint size = _shaders.size();
    for(GLuint i = 0; i < size; i++) {
        if(_shaders[i] != 0) {
            glDetachShader(_program, _shaders[i]);
        }
    }
    glDeleteProgram(_program);
}

void ProgramShader::create() {
    _program = glCreateProgram();
    GLuint size = _shaders.size();
    for(GLuint i = 0; i < size; i++) {
        if(_shaders[i] != 0) {
            glAttachShader(_program, _shaders[i]);
        }
    }
}

void ProgramShader::link() {
    // link and check whether the program links fine
    GLint status;
    glLinkProgram (_program);
    glGetProgramiv (_program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        
        glGetProgramiv (_program, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *infoLog = new GLchar[infoLogLength];
        glGetProgramInfoLog (_program, infoLogLength, NULL, infoLog);
        std::cerr << "Link log: " << infoLog << std::endl;
        delete [] infoLog;
    }
}

void ProgramShader::use() {
    glUseProgram(_program);
}

void ProgramShader::unuse() {
    glUseProgram(-1);
}

void ProgramShader::compile_and_link() {
    create();
    link();
} 
 
GLuint ProgramShader::program() { 
    return _program; 
} 

void ProgramShader::add_attribute(const std::string& attr) {
    _attrsList[attr] = glGetAttribLocation(_program, attr.c_str());
}

void ProgramShader::add_attributes(const std::vector<char*> attrs) {
    for(auto& a: attrs) {
        add_attribute(a);
    }
}

void ProgramShader::add_uniform(const std::string& uniform_name) {
    _uniformList[uniform_name] = glGetUniformLocation(_program, uniform_name.c_str());
}

void ProgramShader::add_uniforms(const std::vector<char*> uniforms) {
    for(auto& u: uniforms) {
        add_uniform(u);
    }
}

void ProgramShader::add_ubo(const std::string& ubo_name) {
    _uboList[ubo_name] = glGetUniformBlockIndex(_program, ubo_name.c_str());
}

#ifdef SUBPROGRAMS
    void ProgramShader::add_subroutine(const std::string& name, GLenum shaderType) {
        GLuint idx = glGetSubroutineIndex(_program, shaderType, name.c_str());
        auto sub = SubProgram(name.c_str(), idx);
        _subprograms.insert(std::pair<GLenum, SubProgram>(shaderType, sub));
    }
#endif

void ProgramShader::bind_attribute(const std::string& attr, GLuint index) {
    glBindAttribLocation(_program, index, attr.c_str());
}

GLuint ProgramShader::attribute(const std::string& attr) {
    return _attrsList[attr];
}

GLuint ProgramShader::operator ()(const std::string& attr) {
    return  attribute(attr);
}

GLuint ProgramShader::uniform(const std::string& uniform_name) {
    return _uniformList[uniform_name];
}

GLuint ProgramShader::operator [](const std::string& attr) {
    return  uniform(attr);
}

GLuint ProgramShader::ubo(const std::string& _ubo) {
    return _uboList[_ubo];
}

#ifdef SUBPROGRAMS
    GLuint ProgramShader::subprogram(const std::string& name, GLenum shaderType) {
        std::multimap<GLenum, SubProgram>::iterator v = _subprograms.find(shaderType);
        int number = _subprograms.count(shaderType);
        for(int i = 0; i < number; i++) {
            if((*v).second.name == name) {
                return (*v).second.index;
            }
            v++;
        }
        std::cerr << "Subroutine not found" << std::endl;
        return -1;
    }
#endif

void ProgramShader::bind_uniform(const std::string& unif, GLuint idx) {
    if(_uniformList.find(unif) == _uniformList.end()) {
        _uniformList[unif] = idx;
    } else {
        std::cerr << "Uniform exist" << std::endl;
    }
}

void ProgramShader::send_uniform(const std::string& uniform_name, float x, float y, float z) {
    GLint loc = uniform(uniform_name);
    glUniform3f(loc, x, y, z);
}

void ProgramShader::send_uniform2v(const std::string& uniform_name, const std::vector< float > & v) {
    GLint loc = uniform(uniform_name);
    glUniform2fv(loc, 1, v.data( ));
}

void ProgramShader::send_uniform3v(const std::string& uniform_name, const std::vector< float > & v) {
    GLint loc = uniform(uniform_name);
    glUniform3fv(loc, 1, v.data( ));
}

void ProgramShader::send_uniform4v(const std::string& uniform_name, const std::vector< float > & v) {
    GLint loc = uniform(uniform_name);
    glUniform4fv(loc, 1,v.data( ));
}

void ProgramShader::send_uniform4m(const std::string& uniform_name, const std::vector< float > & m, GLboolean inverse) {
    GLint loc = uniform(uniform_name);
    glUniformMatrix4fv(loc, 1, inverse, m.data( ));
}

void ProgramShader::send_uniform3m(const std::string& uniform_name, const std::vector< float > & m) {
    GLint loc = uniform(uniform_name);
    glUniformMatrix3fv(loc, 1, GL_FALSE, m.data( ));
}

void ProgramShader::send_uniformf(const std::string& uniform_name, GLfloat val) {
    GLint loc = uniform(uniform_name);
    glUniform1f(loc, val);
}

void ProgramShader::send_uniformi(const std::string& uniform_name, int val) {
    GLint loc = uniform(uniform_name);
    glUniform1i(loc, val);
}

void ProgramShader::send_uniformb(const std::string& uniform_name, bool val) {
    GLint loc = uniform(uniform_name);
    glUniform1i(loc, val);
}

void ProgramShader::send_uniformu(const std::string& uniform_name, GLuint val) {
    GLint loc = uniform(uniform_name);
    glUniform1ui(loc, val);
}

#ifdef SUBPROGRAMS
    void ProgramShader::active_subprogram(const std::string& name, GLenum shaderType) {
        std::multimap<GLenum, SubProgram>::iterator v = _subprograms.find(shaderType);
        int number = _subprograms.count(shaderType);
        for(int i = 0; i < number; i++) {
            if((*v).second.name == name) {
                glUniformSubroutinesuiv(shaderType, 1, &(*v).second.index);
                return;
            }
            v++;
        }
        std::cerr << "Subroutine not found" << std::endl;
    }
#endif

#ifdef COMPUTE_SHADERS
    void ProgramShader::launchComputeWork(GLuint nGx, GLuint nGy, GLuint nGz) {
        glDispatchCompute(nGx, nGy, nGz);
    }
#endif

#ifdef TESSELATION_SHADERS
    void ProgramShader::patchVertices(GLuint n) {
        glPatchParameteri(GL_PATCH_VERTICES, n);
    }
    void ProgramShader::innerLevel(GLfloat l) {
        glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, &l);
    }
    void ProgramShader::outerLevel(GLfloat l) {
        glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, &l);

    }
#endif

#ifdef OCC_QUERY
    bool ProgramShader::occlusion_query(std::function<void()> renderFunc) {
        // Disable writing to the color buffer and depth buffer. 
        // We just wanna check if they would be rendered, not actually render them
        glColorMask(false, false, false, false);
        glDepthMask(GL_FALSE);
        glBeginQuery(GL_SAMPLES_PASSED, _occQuery);
        renderFunc();
        glEndQuery(GL_SAMPLES_PASSED);
        GLint samplesPassed = 0;
        glGetQueryObjectiv(_occQuery, GL_QUERY_RESULT, &samplesPassed);
        // Re-enable writing to color buffer and depth buffer
        glColorMask(true, true, true, true);
        glDepthMask(GL_TRUE);
        return samplesPassed > 0;
    }
#endif

#ifdef GEOMETRY_SHADERS
    void ProgramShader::geometryMaxOutput(GLuint o) {
        glProgramParameteri(_program, GL_GEOMETRY_VERTICES_OUT, o);
    }
    void ProgramShader::geometryInputType(GLuint i) {
        glProgramParameteri(_program, GL_GEOMETRY_INPUT_TYPE, i);
    }
    void ProgramShader::geometryOutputType(GLuint o) {
        glProgramParameteri(_program, GL_GEOMETRY_OUTPUT_TYPE, o);
    }
#endif