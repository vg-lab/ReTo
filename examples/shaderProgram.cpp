/*
 * Copyright (c) 2014-2016 GMRV/URJC.
 *
 * Authors: Cristian Rodriguez
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
#include <limits.h>
#include <reto/reto.h>
#include <GL/glew.h>

#ifdef Darwin
  #define __gl_h_
  #define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <GL/freeglut.h>
#else
  #include <GL/gl.h>
  #include <GL/freeglut.h>
#endif

#include <iostream>
#include <string>

using namespace reto;

#ifndef RETO_GEOMETRY_SHADERS
int main(int, char**)
{
  std::cerr << "ReTo not built with geometry shaders" << std::endl;
  return -1;
}
#else
int main( int argc, char** argv )
{

  glutInit(&argc, argv);

  glutInitContextVersion( 4, 3 );
  glutInitContextFlags( GLUT_FORWARD_COMPATIBLE );
  glutInitContextProfile( GLUT_CORE_PROFILE );

  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
  glutInitWindowSize( 500, 500 );
  glutInitWindowPosition( 0, 0 );
  glutCreateWindow( "ReTo shader program example" );

  glewExperimental = GL_TRUE;
  GLenum err = glewInit( );

  if ( GLEW_OK != err )
  {
    std::cout << "Error: " << glewGetErrorString( err ) << std::endl;
    return -1;
  }

  const GLubyte *oglVersion = glGetString( GL_VERSION );
  std::cout << "This system supports OpenGL Version: "
            << oglVersion << std::endl;


  std::string vsShader (
    "#version 430 core\n"
    "layout(location = 0) in vec3 position;\n"
    "uniform mat4 MVP;\n"
    "void main() {\n"
    " gl_Position = MVP * vec4(position, 1.0);\n"
    "}"
  );
  std::string fsShader (
    "#version 430 core\n"
    "out vec4 fragColor;\n"
    "void main() {\n"
    " fragColor = vec4(1.0);\n"
    "}"
  );
  std::string gsShader (
    "#version 430 core\n"
    "layout (points) in;\n"
    "layout (line_strip) out;\n"
    "//layout (max_vertices = 2) out;\n"
    "void main() {\n"
    "    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);\n"
    "    EmitVertex();\n"
    "    gl_Position = gl_in[0].gl_Position + vec4(0.1, 0.0, 0.0, 0.0);\n"
    "    EmitVertex();\n"
    "    EndPrimitive();\n"
    "}"
  );

  reto::ShaderProgram prog;
  bool vsCreate = prog.loadVertexShaderFromText( vsShader );
  std::cout << std::boolalpha << vsCreate << std::endl;

  bool fsCreate = prog.loadFragmentShaderFromText( fsShader );
  std::cout << std::boolalpha << fsCreate << std::endl;

  bool compile = prog.compileAndLink( );
  std::cout << std::boolalpha << compile << std::endl;

  prog.addUniform( "MVP" );
  std::cout << "[MVP] => " << prog["MVP"] << std::endl;
  std::cout << "uniform(MVP) => " << prog.uniform( "MVP" ) << std::endl;
  std::cout << std::boolalpha << "[MVP] == uniform(MVP) => "
            << ( prog["MVP"] == prog.uniform( "MVP" )) << std::endl;


  reto::ShaderProgram prog2;
  vsCreate = prog2.loadVertexShaderFromText( vsShader );
  std::cout << std::boolalpha << vsCreate << std::endl;

  fsCreate = prog2.loadFragmentShaderFromText( fsShader );
  std::cout << std::boolalpha << fsCreate << std::endl;

  bool gsCreate = prog2.loadGeometryShaderFromText( gsShader );
  std::cout << std::boolalpha << gsCreate << std::endl;

  prog2.create( );
  int programId = prog2.program( );
  int inputType = GL_TRIANGLES;
  int outputType = GL_TRIANGLE_STRIP;
  glProgramParameteri( programId, GL_GEOMETRY_INPUT_TYPE, inputType );
  glProgramParameteri( programId, GL_GEOMETRY_OUTPUT_TYPE, outputType );

  int maxVerts;
  glGetIntegerv( GL_MAX_GEOMETRY_OUTPUT_VERTICES, &maxVerts );
  glProgramParameteri( programId, GL_GEOMETRY_VERTICES_OUT, 8 );
  std::cout << "GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT  count = "
            << maxVerts << std::endl;

  prog2.link( );
  std::cout << prog2.getGeometryInputType( ) << " " << inputType << std::endl;
  std::cout << prog2.getGeometryOutputType( ) << " " << outputType << std::endl;

  return 0;

}

#endif // RETO_GEOMETRY_SHADERS
