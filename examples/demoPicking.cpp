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


using namespace reto;

const int cubeNVertex = 24;
const int cubeNTriangleIndex = 12;

const unsigned int cubeTriangleIndex[] = {
	0,1,2,			1,3,2,
	4,6,5,			5,6,7,
	8,10,9,			9,10,11,
	12,13,14,		13,15,14,
	16,17,18,		17,19,18,
	20,22,21,		21,22,23,
};

const float cubeVertexPos[] = {
	-1.0f,	-1.0f,	 1.0f,
	1.0f,	-1.0f,	 1.0f,
	-1.0f,	 1.0f,	 1.0f,
	1.0f,	 1.0f,	 1.0f,

	-1.0f,	-1.0f,	-1.0f,
	1.0f,	-1.0f,	-1.0f,
	-1.0f,	 1.0f,	-1.0f,
	1.0f,	 1.0f,	-1.0f,

	1.0f,	-1.0f,	-1.0f,
	1.0f,	-1.0f,	 1.0f,
	1.0f,	 1.0f,	-1.0f,
	1.0f,	 1.0f,	 1.0f,

	-1.0f,	-1.0f,	-1.0f,
	-1.0f,	-1.0f,	 1.0f,
	-1.0f,	 1.0f,	-1.0f,
	-1.0f,	 1.0f,	 1.0f,

	-1.0f,	 1.0f,	-1.0f,
	-1.0f,	 1.0f,	 1.0f,
	1.0f,	 1.0f,	-1.0f,
	1.0f,	 1.0f,	 1.0f,

	-1.0f,	-1.0f,	-1.0f,
	-1.0f,	-1.0f,	 1.0f,
	1.0f,	-1.0f,	-1.0f,
	1.0f,	-1.0f,	 1.0f
};

#include "Model.h"

PickingSystem* ps;

//glm::mat4	proj = glm::mat4(1.0f);
//glm::mat4	view = glm::mat4(1.0f);
//glm::mat4	model = glm::mat4(1.0f);

std::vector<Model*> models(81);

float w = 800, h = 800;

float angle = 0.0f;

unsigned int vao;

unsigned int posVBO;
unsigned int triangleIndexVBO;

ShaderProgram fwShader;
bool getID = false;
int mouseX, mouseY;

void renderFunc();
void draw(ShaderProgram& ss);
void renderCube(ShaderProgram& ss);
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);

void initContext(int argc, char** argv);
void initOGL();
void initObj();
void destroy();

int main(int argc, char** argv)
{
  std::cout << "LOADING CONTEXT ..." << std::endl;
	initContext(argc, argv);
  std::cout << "LOADING OGL ..." << std::endl;
	initOGL();
  std::cout << "LOADING SHADER ..." << std::endl;
	fwShader.load("./fwRendering.v0.vert", "./fwRendering.v0.frag");
	fwShader.compileAndLink();
	fwShader.addAttribute("inPos");
	fwShader.addUniform("modelViewProj");
	fwShader.addUniform("id");

  std::cout << "SHADER LOADED!" << std::endl;
	initObj();

	/*ps = new PickingSystem();

	for (int i = 0; i < 81; i++) {
		models[i] = new Model(vao);
		ps->AddObject(models[i]);
	}

	glutMainLoop();*/

	destroy();

	return 0;
}

void initContext(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitContextVersion(4, 3);
	//glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Picking and camera demo");

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}

	const GLubyte *oglVersion = glGetString(GL_VERSION);
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;

	glutReshapeFunc(resizeFunc);
	glutDisplayFunc(renderFunc);
	glutIdleFunc(idleFunc);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);
}

void initOGL()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	//proj = glm::perspective(glm::radians(60.0f), 1.0f, 1.0f, 50.0f);
	//view = glm::mat4(1.0f);
	//view[3].z = -35.0f;
}

void destroy()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	if (fwShader.isAttributeCached("inPos")) glDeleteBuffers(1, &posVBO);
	glDeleteBuffers(1, &triangleIndexVBO);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);
}

void initObj()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &posVBO);
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3,
		cubeVertexPos, GL_STATIC_DRAW);
	glVertexAttribPointer(fwShader.attribute("inPos"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(fwShader.attribute("inPos"));

	glGenBuffers(1, &triangleIndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		cubeNTriangleIndex * sizeof(unsigned int) * 3, cubeTriangleIndex,
		GL_STATIC_DRAW);

	//model = glm::mat4(1.0f);
}

void renderFunc() {
	draw(fwShader);
}

void draw(ShaderProgram& ss) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// | GL_STENCIL_BUFFER_BIT);

	ss.use();

	//glEnable(GL_STENCIL_TEST);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	int n = 0;
	//int offset = 16;
	for (int i = 0; i < 22; i++) {
		for (int j = 0; j < 22; j++) {
			for (int k = 0; k < 3; k++) {
				//model = glm::translate(glm::mat4(), glm::vec3(i * 1.5 - offset, j * 1.5 - offset, k * 3.0 - offset));
				//model = glm::scale(model, glm::vec3(0.5f));
				//glStencilFunc(GL_ALWAYS, n, -1);
				ss.sendUniformf("id", n);
				renderCube(ss);
				n = models[0]->sendId(n);
			}
		}
	}
	if (getID == true) {
		mouseY = h - mouseY;
		ps->click(Point{ mouseX, mouseY });
		getID = false;
	}

	glutSwapBuffers();
}

void renderCube(ShaderProgram&)
{
	//glm::mat4 modelViewProj = proj * view * model;

	//ss.sendUniform4m("modelViewProj", glm::value_ptr(modelViewProj));

	models[0]->render();
}

void resizeFunc(int width, int height)
{
	glViewport(0, 0, width, height);
	//proj = glm::perspective(glm::radians(60.0f), float(width) / float(height), 1.0f, 50.0f);
	w = width;
	h = height;
	glutPostRedisplay();
}

void idleFunc()
{
	angle = (angle > 3.141592f * 2.0f) ? 0 : angle + 0.02f;

	glutPostRedisplay();
}

void keyboardFunc(unsigned char, int, int) { }
void mouseFunc(int button, int state, int x, int y) {
	if (state == 0 && button == 2) {
		std::cout << "Pressed on " << x << ", " << y << std::endl;
		getID = true;
		mouseX = x;
		mouseY = y;
	}
}
