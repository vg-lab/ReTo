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

float concreteRound( float input )
{
  // Three decimals.
  return roundf( input * 1000.f ) / 1000.f;
}

void checkEqualVectors( const Eigen::Vector3f& firstVector,
                        const Eigen::Vector3f& secondVector )
{
  BOOST_CHECK_EQUAL( concreteRound( firstVector.x( ) ),
                     concreteRound( secondVector.x( ) ) );
  BOOST_CHECK_EQUAL( concreteRound( firstVector.y( ) ),
                     concreteRound( secondVector.y( ) ) );
  BOOST_CHECK_EQUAL( concreteRound( firstVector.z( ) ),
                     concreteRound( secondVector.z( ) ) );
}

BOOST_AUTO_TEST_CASE( path_constructor )
{
  // Default constructor.
  {
    Path path;
    BOOST_CHECK_EQUAL( path.empty(), true );
    BOOST_CHECK_EQUAL( path.interpolationMethod( ),
                       Path::TInterpolationMethod::LERP );
  }
  // Constructor.
  {
    Eigen::Vector3f position0( 0.0f, 0.0f, 0.0f );
    Eigen::Vector3f lookAt0( 0.0f, 0.0f, 1.0f );
    Eigen::Vector3f up0( 0.0f, 1.0f, 0.0f );
    std::vector< Eigen::Vector3f > positions;
    std::vector< Eigen::Vector3f > lookAts;
    std::vector< Eigen::Vector3f > ups;
    positions.push_back( position0 );
    lookAts.push_back( lookAt0 );
    ups.push_back( up0 );

    Path path( positions, lookAts, ups,
               Path::TInterpolationMethod::CATMULL_ROM );

    BOOST_CHECK_EQUAL( path.nodesSize( ), 1 );
    BOOST_CHECK_EQUAL( path.interpolationMethod( ),
                       Path::TInterpolationMethod::CATMULL_ROM );
  }
}

BOOST_AUTO_TEST_CASE( path_add_and_clear )
{
  Path path;

  Eigen::Vector3f position0( 0.0f, 0.0f, -20.0f );
  Eigen::Vector3f lookAt0( 0.0f, 0.0f, 1.0f );
  Eigen::Vector3f up0( 0.0f, 1.0f, 0.0f );
  path.addNode( position0, lookAt0, up0 );
  BOOST_CHECK_EQUAL( path.nodesSize( ), 1 );

  Eigen::Vector3f position1( 20.0f, 0.0f, 0.0f );
  Eigen::Vector3f lookAt1( -1.0f, 0.0f, 0.0f );
  Eigen::Vector3f up1( 0.0f, 1.0f, 0.0f );
  path.addNode( position1, lookAt1, up1 );
  BOOST_CHECK_EQUAL( path.nodesSize( ), 2 );

  path.clearNodes( );
  BOOST_CHECK_EQUAL( path.empty( ), true );
}

BOOST_AUTO_TEST_CASE( path_evaluate_position )
{
  Path path;

  Eigen::Vector3f position0( 0.0f, 0.0f, -20.0f );
  Eigen::Vector3f lookAt0( 0.0f, 0.0f, 1.0f );
  Eigen::Vector3f up0( 0.0f, 1.0f, 0.0f );
  path.addNode( position0, lookAt0, up0 );

  Eigen::Vector3f position1( 20.0f, 0.0f, 0.0f );
  Eigen::Vector3f lookAt1( -1.0f, 0.0f, 0.0f );
  Eigen::Vector3f up1( 0.0f, 1.0f, 0.0f );
  path.addNode( position1, lookAt1, up1 );

  Eigen::Vector3f position2( 0.0f, 0.0f, 20.0f );
  Eigen::Vector3f lookAt2( 0.0f, 0.0f, -1.0f );
  Eigen::Vector3f up2( 0.0f, 1.0f, 0.0f );
  path.addNode( position2, lookAt2, up2 );

  Eigen::Vector3f position3( -20.0f, 0.0f, 0.0f );
  Eigen::Vector3f lookAt3( 1.0f, 0.0f, 0.0f );
  Eigen::Vector3f up3( 0.0f, 1.0f, 0.0f );
  path.addNode( position3, lookAt3, up3 );

  Eigen::Vector3f expectedPositionUsingLerp( 10.0f, 0.0f, 10.0f );
  checkEqualVectors( path.evaluatePosition( 1, 0.5f ),
                     expectedPositionUsingLerp );

  path.interpolationMethod( Path::TInterpolationMethod::CATMULL_ROM );

  Eigen::Vector3f expectedPositionUsingCatmullRom( 12.5f, 0.0f, 12.5 );
  checkEqualVectors( path.evaluatePosition( 1, 0.5f ),
                     expectedPositionUsingCatmullRom );
}


