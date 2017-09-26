/*
 * Copyright (c) 2014-2016 GMRV/URJC.
 *
 * Authors: TBD
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

BOOST_AUTO_TEST_CASE( test_camera )
{
  Camera* c = new Camera( );

  c->localTranslation( Eigen::Vector3f( 0.0f, 0.0f, 0.0f ) );
  c->localRotation( 0.0f, 0.0f );

  BOOST_CHECK_EQUAL( c->fov( ), 0.392699093f );
  BOOST_CHECK_EQUAL( c->position( )[ 0 ], 0.0f );

  c->radius( 5.0f );
  BOOST_CHECK_EQUAL( c->radius( ), 5.0f );

  c->pivot( Eigen::Vector3f( 0.0f, 1.0f, 0.0f ) );

  BOOST_CHECK_EQUAL( c->pivot( )[ 1 ], 1.0f );

  c->setWindowSize( 1000, 1000 );

  BOOST_CHECK_EQUAL( c->width( ), 1000 );
  BOOST_CHECK_EQUAL( c->height( ), 1000 );

  delete c;
}
