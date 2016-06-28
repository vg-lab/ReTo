/*
 * Copyright (c) 2014-2016 GMRV/URJC.
 *
 * Authors: Cristian Rodríguez Bernal
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
#include "retoTests.h"

using namespace reto;

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

BOOST_AUTO_TEST_CASE( test_program_shader )
{
	char *myargv [1];
	int myargc = 1;
	myargv [0] = 'a';
	glutInit(&myargc, myargv);

	glutInitContextVersion(4, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Prácticas GLSL");

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		exit (-1);
	}
	const GLubyte *oglVersion = glGetString(GL_VERSION);
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;


	std::string vsShader = (
		"#version 430 core\n"
		"layout(location = 0) in vec3 position;\n"
		"uniform mat4 MVP;\n"
		"void main() {\n"
		"	gl_Position = MVP * vec4(position, 1.0);\n"
		"}"
	);
	std::string fsShader = (
		"#version 430 core\n"
		"out vec4 fragColor;\n"
		"void main() {\n"
		"	fragColor = vec4(1.0);\n"
		"}"
	);
	std::string gsShader = (
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

	//reto::Camera c;
	reto::ShaderProgram prog;
	bool vsCreate = prog.loadVertexShaderFromText(vsShader);
	BOOST_CHECK( vsCreate == true );

	bool fsCreate = prog.loadFragmentShaderFromText(fsShader);
	BOOST_CHECK( fsCreate == true );

	bool compile = prog.compileAndLink();
	BOOST_CHECK( compile == true );

	prog.addUniform("MVP");
	BOOST_CHECK(prog["MVP"] == 0);
	BOOST_CHECK(prog.uniform("MVP") == 0);


	reto::ShaderProgram prog2;
	vsCreate = prog2.loadVertexShaderFromText(vsShader);
	BOOST_CHECK( vsCreate == true );

	fsCreate = prog2.loadFragmentShaderFromText(fsShader);
	BOOST_CHECK( fsCreate == true );

	bool gsCreate = prog2.loadGeometryShaderFromText(gsShader);
	BOOST_CHECK( gsCreate == true );





	prog2.create();
	glProgramParameteriEXT(prog2.program(), GL_GEOMETRY_VERTICES_OUT, 2);
	prog2.link();
	//compile = prog2.compileAndLink();
	//BOOST_CHECK( compile == true );

	BOOST_CHECK(prog2.getGeometryInputType() == GL_POINTS);
	BOOST_CHECK(prog2.getGeometryOutputType() == GL_LINE_STRIP);
	BOOST_CHECK(prog2.getGeometryMaxOutput() == 1024);
    BOOST_TEST_MESSAGE( "xDDDDDDDDDDDDDD" );
	//prog2.use();
	//write
	//glProgramParameteri(prog2.program(), GL_GEOMETRY_VERTICES_OUT, 8);
	//BOOST_CHECK(prog2.getGeometryMaxOutput() == 2);

/*
	//read 
	int temp;
	glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &temp);
	BOOST_CHECK( temp == 2 );*/


	//prog2.setGeometryMaxOutput(8);
	//BOOST_CHECK(prog2.getGeometryMaxOutput() == 2);
	//prog2.setGeometryInputType(GL_TRIANGLES);
	//prog2.setGeometryOutputType(GL_TRIANGLE_STRIP);

	//BOOST_CHECK(prog2.getGeometryInputType() == GL_TRIANGLES);
	//BOOST_CHECK(prog2.getGeometryOutputType() == GL_TRIANGLE_STRIP);
	//BOOST_CHECK(prog2.getGeometryMaxOutput() == 8);
	//glProgramParameteri( prog2.program(),GL_GEOMETRY_VERTICES_OUT, 101 );
	//GLint maxOutput;
	//glGetProgramiv( prog2.program(), GL_GEOMETRY_VERTICES_OUT, &maxOutput );
	//BOOST_CHECK( 101 == maxOutput );
}
