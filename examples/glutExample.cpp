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
#include <glutExampleShaders.h>
#include <GL/glew.h>
#include "cube.h"

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

Camera camera;
ShaderProgram prog;

unsigned int vao;
unsigned int posVBO;
unsigned int colorVBO;
unsigned int normalVBO;
unsigned int texCoordVBO;
unsigned int triangleIndexVBO;

void renderFunc( void );
void resizeFunc( int width, int height );
void idleFunc( void );
void keyboardFunc( unsigned char key, int x, int y );
void mouseFunc( int button, int state, int x, int y );

void initContext( int argc, char** argv );
void initOGL( void );
void initShader( const char *vname, const char *fname );
void initObj( void );
void destroy( void);

int main( int argc, char** argv )
{
	initContext( argc, argv );
	initOGL( );
	initShader( RETO_EXAMPLE_SHADERS_V0_VERT,
              RETO_EXAMPLE_SHADERS_V0_FRAG );
	//initObj( );

	//glutMainLoop( );
	//destroy( );

	return 0;
}

void initContext( int argc, char** argv )
{
	glutInit( &argc,argv );
	glutInitContextVersion( 3,3 );
	glutInitContextFlags( GLUT_FORWARD_COMPATIBLE );
	glutInitContextProfile( GLUT_CORE_PROFILE );

	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize( 500, 500 );
	glutInitWindowPosition( 0, 0 );
	glutCreateWindow( "GLUT example" );

	glewExperimental = GL_TRUE;
	GLenum err = glewInit( );
	if ( GLEW_OK != err ) {
		std::cout << "Error: " << glewGetErrorString( err ) << std::endl;
		exit ( -1 );
	}
	const GLubyte *oglVersion = glGetString( GL_VERSION );
	std::cout << "This system supports OpenGL Version: "
            << oglVersion << std::endl;

	glutReshapeFunc( resizeFunc );
	glutDisplayFunc( renderFunc );
	glutIdleFunc( idleFunc );
	glutKeyboardFunc( keyboardFunc );
	glutMouseFunc( mouseFunc );
}

void initOGL( void )
{
	glEnable( GL_DEPTH_TEST );
	glClearColor( 0.2f, 0.2f, 0.2f, 0.0f );

	glFrontFace( GL_CCW );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glEnable( GL_CULL_FACE );

}

void destroy( void )
{
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	glDeleteBuffers( 1, &posVBO );
	glDeleteBuffers( 1, &colorVBO );
	glDeleteBuffers( 1, &normalVBO );
	glDeleteBuffers( 1, &texCoordVBO );

	glDeleteBuffers( 1, &triangleIndexVBO );
	glBindVertexArray( 0 );
	glDeleteVertexArrays( 1, &vao );

}

void initShader( const char *vname, const char *fname )
{
	prog.load( vname, fname );
	///////////////////////////////
	// 			SAME CODE
	///////////////////////////////
	// prog.loadVertexShader( vname );
	// prog.loadFragmentShader( fname );

	prog.create( );
	prog.bindAttribute( "inPos", 0 );
	prog.bindAttribute( "inColor", 1 );
	prog.bindAttribute( "inNormal", 2 );
	prog.bindAttribute( "inTexCoord", 3 );
	prog.link( );

	prog.addUniform( "model" );
	prog.addUniform( "normal" );
	prog.addUniform( "View" );
	prog.addUniform( "ViewProj" );

	std::cout << "---------------------------" << std::endl;

	std::cout << "model: " << prog["model"] << std::endl;
	std::cout << "normal: " << prog["normal"] << std::endl;
	std::cout << "View: " << prog["View"] << std::endl;
	std::cout << "ViewProj: " << prog["ViewProj"] << std::endl;

	prog.addAttribute( "inPos" );
	prog.addAttribute( "inColor" );
	prog.addAttribute( "inNormal" );
	prog.addAttribute( "inTexCoord" );

	std::cout << "---------------------------" << std::endl;

	std::cout << "inPos: " << prog( "inPos" ) << std::endl;
	std::cout << "inColor: " << prog( "inColor" ) << std::endl;
	std::cout << "inNormal: " << prog( "inNormal" ) << std::endl;
	std::cout << "inTexCoord: " << prog( "inTexCoord" ) << std::endl;

  prog.autocatching( );
}

void initObj( )
{
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	glGenBuffers( 1, &posVBO );
	glBindBuffer( GL_ARRAY_BUFFER, posVBO );
	glBufferData( GL_ARRAY_BUFFER, cubeNVertex * sizeof( float ) * 3,
                cubeVertexPos, GL_STATIC_DRAW );
	glVertexAttribPointer( prog( "inPos" ), 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray( prog( "inPos" ));

	glGenBuffers( 1, &colorVBO );
  //GL_ARRAY_BUFFERS para atributos
	glBindBuffer( GL_ARRAY_BUFFER, colorVBO );
	glBufferData( GL_ARRAY_BUFFER, cubeNVertex * sizeof( float ) * 3,
                cubeVertexColor, GL_STATIC_DRAW );
	glVertexAttribPointer( prog( "inColor" ), 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray( prog( "inColor" ));

	glGenBuffers( 1, &normalVBO );
	glBindBuffer( GL_ARRAY_BUFFER, normalVBO );
	glBufferData( GL_ARRAY_BUFFER, cubeNVertex * sizeof( float ) * 3,
                cubeVertexNormal, GL_STATIC_DRAW );
	glVertexAttribPointer( prog( "inNormal" ), 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray( prog( "inNormal" ));

	glGenBuffers( 1, &texCoordVBO );
	glBindBuffer( GL_ARRAY_BUFFER, texCoordVBO );
	glBufferData( GL_ARRAY_BUFFER, cubeNVertex * sizeof( float ) * 2,
                cubeVertexTexCoord, GL_STATIC_DRAW );
	glVertexAttribPointer( prog( "inTexCoord" ), 2, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray( prog( "inTexCoord" ));

	glGenBuffers( 1, &triangleIndexVBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                cubeNTriangleIndex * sizeof( unsigned int ) * 3,
                cubeTriangleIndex, GL_STATIC_DRAW );

}

void renderFunc( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	prog.use( );

	prog.sendUniform4m( "View", camera.viewMatrix( ));
	auto vp = camera.viewProjectionMatrix( );
	prog.sendUniform4m( "ViewProj", vp );

	glBindVertexArray( vao );
	glDrawElements( GL_TRIANGLES, cubeNTriangleIndex * 3,
                  GL_UNSIGNED_INT, ( void* ) 0 );
	prog.unuse( );

	glutSwapBuffers( );

}

void resizeFunc( int width, int height )
{
	glViewport( 0, 0, width, height );
}

void idleFunc( void )
{

	static float angle = 0.0f;
	angle = ( angle > 2.0f * float( M_PI )) ? 0 : angle + 0.01f;
	glutPostRedisplay( );

}

void keyboardFunc( unsigned char, int, int )
{
}

void mouseFunc( int, int, int, int )
{
}
