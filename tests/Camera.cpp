/*
 * Copyright (c) 2014-2016 GMRV/URJC.
 *
 * Author: Juan Guerrero Martín
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

BOOST_AUTO_TEST_CASE( camera_constructor )
{
  // Default constructor.
  {
    Camera camera;
    BOOST_CHECK_EQUAL( camera.width( ), 1920 );
    BOOST_CHECK_EQUAL( camera.height( ), 1080 );
    BOOST_CHECK_EQUAL( camera.nearPlane( ), 0.1f );
    BOOST_CHECK_EQUAL( camera.farPlane( ), 10000.0f );
    BOOST_CHECK_EQUAL( camera.position( ), Eigen::Vector3f( 0.0f, 0.0f, -500.0f ) );
    BOOST_CHECK_EQUAL( camera.up( ), Eigen::Vector3f( 0.0f, 1.0f, 0.0f ) );
    BOOST_CHECK_EQUAL( camera.lookAt( ), Eigen::Vector3f( 0.0f, 0.0f, 1.0f ) );
    BOOST_CHECK_EQUAL( camera.viewMatrix( ), Eigen::Matrix4f::Identity( ) );
    BOOST_CHECK_EQUAL( camera.projMatrix( ), Eigen::Matrix4f::Identity( ) );
    BOOST_CHECK_EQUAL( camera.viewProjMatrix( ), Eigen::Matrix4f::Identity( ) );
    BOOST_CHECK_EQUAL( camera.fov( ), 45.0f );
  }
  // Constructor.
  {
    Camera camera( 1600, 1200,
                   0.2f, 1000.0f,
                   Eigen::Vector3f( 0.0f, 0.0f, 500.0f ),
                   Eigen::Vector3f( 0.0f, 1.0f, 0.0f ),
                   Eigen::Vector3f( 0.0f, 0.0f, -1.0f ),
                   60.f );
    BOOST_CHECK_EQUAL( camera.width( ), 1600 );
    BOOST_CHECK_EQUAL( camera.height( ), 1200 );
    BOOST_CHECK_EQUAL( camera.nearPlane( ), 0.2f );
    BOOST_CHECK_EQUAL( camera.farPlane( ), 1000.0f );
    BOOST_CHECK_EQUAL( camera.position( ), Eigen::Vector3f( 0.0f, 0.0f, 500.0f ) );
    BOOST_CHECK_EQUAL( camera.up( ), Eigen::Vector3f( 0.0f, 1.0f, 0.0f ) );
    BOOST_CHECK_EQUAL( camera.lookAt( ), Eigen::Vector3f( 0.0f, 0.0f, -1.0f ) );
    BOOST_CHECK_EQUAL( camera.viewMatrix( ), Eigen::Matrix4f::Identity( ) );
    BOOST_CHECK_EQUAL( camera.projMatrix( ), Eigen::Matrix4f::Identity( ) );
    BOOST_CHECK_EQUAL( camera.viewProjMatrix( ), Eigen::Matrix4f::Identity( ) );
    BOOST_CHECK_EQUAL( camera.fov( ), 60.0f );
  }
}
