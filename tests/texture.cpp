/*
 * Copyright (c) 2014-2017 GMRV/URJC.
 *
 * Authors: Cristian Rodriguez Bernal <cristian.rodriguez@urjc.es>
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

// OpenGL, GLEW, GLUT.
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

BOOST_AUTO_TEST_CASE( texture_example )
{
  char fooParam[] = "foo";
  char *fooargv[] = { fooParam, NULL };
  int fooargc = 1;

  glutInit( &fooargc, fooargv );

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
  std::cout << "This system supports OpenGL Version: "
            << oglVersion << std::endl;

  TextureConfig config;
  config.packAlignment = 1;
  config.unpackAlignment = 1;

  BOOST_CHECK_EQUAL( config.format, GL_RGBA );
  BOOST_CHECK_EQUAL( config.wrapR, GL_CLAMP_TO_EDGE );

  BOOST_CHECK( &TextureManager::getInstance( ) != nullptr );

  float values[ ] = { 1.0f, 1.0f, 1.0f, 1.0f };
  Texture1D* tex1 = new Texture1D( config, values, 1 );
  tex1->update( values, 1 );

  Texture* tex2 = new Texture2D( config, 500, 500 );

  tex2->resize( 150, 150 );

  tex1->bind( 0 );
  tex2->bind( 1 );

  TextureManager::getInstance( ).add( "example1D", tex1 );
  TextureManager::getInstance( ).add( "example2D", tex2 );

  BOOST_CHECK( TextureManager::getInstance( ).get( "example2D" ) == tex2 );
  BOOST_CHECK( TextureManager::getInstance( ).get( "hello" ) == nullptr );

  TextureManager::getInstance( ).remove( "example2D" );

  BOOST_CHECK( TextureManager::getInstance( ).get( "example2D" ) == nullptr );

  delete tex2;
}
