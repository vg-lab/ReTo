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

void checkEqualMatrices( const Eigen::Matrix4f& firstMatrix,
                         const Eigen::Matrix4f& secondMatrix )
{
  // Column 0.
  BOOST_CHECK_EQUAL( concreteRound( firstMatrix( 0, 0 ) ),
                     concreteRound( secondMatrix( 0, 0 ) ) );
  BOOST_CHECK_EQUAL( concreteRound( firstMatrix( 1, 0 ) ),
                     concreteRound( secondMatrix( 1, 0 ) ) );
  BOOST_CHECK_EQUAL( concreteRound( firstMatrix( 2, 0 ) ),
                     concreteRound( secondMatrix( 2, 0 ) ) );
  BOOST_CHECK_EQUAL( concreteRound( firstMatrix( 3, 0 ) ),
                     concreteRound( secondMatrix( 3, 0 ) ) );

  // Column 1.
  BOOST_CHECK_EQUAL( concreteRound( firstMatrix( 0, 1 ) ),
                     concreteRound( secondMatrix( 0, 1 ) ) );
  BOOST_CHECK_EQUAL( concreteRound( firstMatrix( 1, 1 ) ),
                     concreteRound( secondMatrix( 1, 1 ) ) );
  BOOST_CHECK_EQUAL( concreteRound( firstMatrix( 2, 1 ) ),
                     concreteRound( secondMatrix( 2, 1 ) ) );
  BOOST_CHECK_EQUAL( concreteRound( firstMatrix( 3, 1 ) ),
                     concreteRound( secondMatrix( 3, 1 ) ) );

  // Column 2.
  BOOST_CHECK_EQUAL( concreteRound( firstMatrix( 0, 2 ) ),
                     concreteRound( secondMatrix( 0, 2 ) ) );
  BOOST_CHECK_EQUAL( concreteRound( firstMatrix( 1, 2 ) ),
                     concreteRound( secondMatrix( 1, 2 ) ) );
  BOOST_CHECK_EQUAL( concreteRound( firstMatrix( 2, 2 ) ),
                     concreteRound( secondMatrix( 2, 2 ) ) );
  BOOST_CHECK_EQUAL( concreteRound( firstMatrix( 3, 2 ) ),
                     concreteRound( secondMatrix( 3, 2 ) ) );

  // Column 3.
  BOOST_CHECK_EQUAL( concreteRound( firstMatrix( 0, 3 ) ),
                     concreteRound( secondMatrix( 0, 3 ) ) );
  BOOST_CHECK_EQUAL( concreteRound( firstMatrix( 1, 3 ) ),
                     concreteRound( secondMatrix( 1, 3 ) ) );
  BOOST_CHECK_EQUAL( concreteRound( firstMatrix( 2, 3 ) ),
                     concreteRound( secondMatrix( 2, 3 ) ) );
  BOOST_CHECK_EQUAL( concreteRound( firstMatrix( 3, 3 ) ),
                     concreteRound( secondMatrix( 3, 3 ) ) );
}

BOOST_AUTO_TEST_CASE( camera_controller_constructor )
{
  // Default constructor.
  {
    CameraController cameraController;
    BOOST_CHECK_EQUAL( cameraController.projection( ),
                       CameraController::TProjection::PERSPECTIVE );
    BOOST_CHECK_EQUAL( cameraController.cameraType( ),
                       CameraController::TCamera::STANDARD );
    BOOST_CHECK_EQUAL( cameraController.pivot( ), Eigen::Vector3f( 0.0f, 0.0f, 0.0f ) );
    BOOST_CHECK_EQUAL( cameraController.radius( ), 500.0f );
    BOOST_CHECK_EQUAL( cameraController.animationDuration( ), 5.0f );
    BOOST_CHECK_EQUAL( cameraController.tStep( ), 0.01f );
  }
  // Constructor.
  {
    Camera* camera = new Camera( );
    Path* path = new Path( );
    CameraController cameraController( camera,
                                       path,
                                       CameraController::TProjection::ORTHOGRAPHIC,
                                       CameraController::TCamera::ORBITAL,
                                       Eigen::Vector3f( 0.0f, 0.0f, 10.0f ),
                                       490.0f,
                                       3.0f, 0.02f );
    BOOST_CHECK_EQUAL( cameraController.projection( ),
                       CameraController::TProjection::ORTHOGRAPHIC );
    BOOST_CHECK_EQUAL( cameraController.cameraType( ),
                       CameraController::TCamera::ORBITAL );
    BOOST_CHECK_EQUAL( cameraController.pivot( ), Eigen::Vector3f( 0.0f, 0.0f, 10.0f ) );
    BOOST_CHECK_EQUAL( cameraController.radius( ), 490.0f );
    BOOST_CHECK_EQUAL( cameraController.animationDuration( ), 3.0f );
    BOOST_CHECK_EQUAL( cameraController.tStep( ), 0.02f );
  }
}

BOOST_AUTO_TEST_CASE( camera_controller_center )
{
  // Standard.
  {
    CameraController cameraController;
    cameraController.center( Eigen::Vector3f( 100.0f, 30.0f, -20.0f ),
                             Eigen::Vector3f( 2.0f, 1.0f, 0.0f ),
                             Eigen::Vector3f( -50.0f, 45.0f, 15.0f ),
                             Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
                             500.0f, 90.0f, 0.0f );
    Eigen::Matrix4f expectedViewMatrix;
    expectedViewMatrix << -0.104194f,  0.208389f, -0.97248f,   -15.2818f,
                           0.680314f,  0.728188f,  0.0831495f, -88.2141f,
                           0.725476f, -0.652929f, -0.217643f,  -57.3126f,
                                0.0f,       0.0f,       0.0f,       1.0f;
    checkEqualMatrices( cameraController.camera( )->viewMatrix( ),
                        expectedViewMatrix );
  }
  // Orbital.
  {
    CameraController cameraController( new Camera( ),
                                       new Path( ),
                                       reto::CameraController::TProjection::PERSPECTIVE,
                                       reto::CameraController::TCamera::ORBITAL,
                                       Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
                                       500.0f,
                                       5.0f, 0.01f );
    cameraController.center( Eigen::Vector3f( 0.0f, 0.0f, -500.0f ),
                             Eigen::Vector3f( 0.0f, 1.0f, 0.0f ),
                             Eigen::Vector3f( 0.0f, 0.0f, 1.0f ),
                             Eigen::Vector3f( -7.0f, 2.0f, 5.0f ),
                             200.0f, 44.0f, -69.0f );
    Eigen::Matrix4f expectedViewMatrix;
    expectedViewMatrix <<  0.347917f,       0.0f, 0.937526f, -2.25221f,
                          -0.629606f,  0.740949f, 0.233647f, -7.05737f,
                          -0.694658f, -0.671561f, 0.257788f, -204.808f,
                                0.0f,       0.0f,      0.0f,      1.0f;
    checkEqualMatrices( cameraController.camera( )->viewMatrix( ),
                        expectedViewMatrix );
  }
}

BOOST_AUTO_TEST_CASE( camera_controller_translate )
{
  // Standard.
  CameraController cameraController;
  cameraController.translate( Eigen::Vector3f( 20.0f, 5.0f, -10.0f ) );
  Eigen::Matrix4f expectedViewMatrix;
  expectedViewMatrix <<  -1.0f,  0.0f,  0.0f,   20.0f,
                          0.0f,  1.0f,  0.0f,   -5.0f,
                          0.0f,  0.0f, -1.0f, -510.0f,
                          0.0f,  0.0f,  0.0f,    1.0f;
  checkEqualMatrices( cameraController.camera( )->viewMatrix( ),
                      expectedViewMatrix );

}

BOOST_AUTO_TEST_CASE( camera_controller_local_orientation )
{
  // Standard.
  CameraController cameraController;
  cameraController.localOrientation( 100.0f, 10.0f );
  Eigen::Matrix4f expectedViewMatrix;
  expectedViewMatrix <<   -0.984808f,       0.0f, -0.173648f,  -86.8241f,
                          0.0301537f,  0.984808f,  -0.17101f,   -85.505f,
                            0.17101f, -0.173648f, -0.969846f,  -484.923f,
                                0.0f,       0.0f,       0.0f,       1.0f;
  checkEqualMatrices( cameraController.camera( )->viewMatrix( ),
                      expectedViewMatrix );
}

BOOST_AUTO_TEST_CASE( camera_controller_local_rotation )
{
  // Orbital.
  CameraController cameraController( new Camera( ),
                                     new Path( ),
                                     reto::CameraController::TProjection::PERSPECTIVE,
                                     reto::CameraController::TCamera::ORBITAL,
                                     Eigen::Vector3f( 1.0f, 1.0f, 1.0f ),
                                     500.0f,
                                     5.0f, 0.01f );
  cameraController.localRotation( 63.0f, -17.0f );
  Eigen::Matrix4f expectedViewMatrix;
  expectedViewMatrix <<  0.438029f,       0.0f,  0.898961f,  -1.33699f,
                        -0.119323f,  0.991152f, 0.0581413f,  -0.92997f,
                        -0.891007f, -0.132734f,  0.434153f,   -499.41f,
                              0.0f,       0.0f,       0.0f,       1.0f;
  checkEqualMatrices( cameraController.camera( )->viewMatrix( ),
                      expectedViewMatrix );
}

BOOST_AUTO_TEST_CASE( camera_controller_zoom )
{
  // Perspective.
  {
    CameraController cameraController;
    cameraController.zoom( -0.4f );
    // Camera default aspect ratio: 1.78.
    Eigen::Matrix4f expectedProjMatrix;
    expectedProjMatrix << 2.34298f,       0.0f,       0.0f,       0.0f,
                              0.0f,    4.1653f,       0.0f,       0.0f,
                              0.0f,       0.0f,  -1.00002f, -0.200002f,
                              0.0f,       0.0f,      -1.0f,       0.0f;
    checkEqualMatrices( cameraController.camera( )->projMatrix( ),
                        expectedProjMatrix );
  }
  // Orthographic.
  {
    CameraController cameraController( new Camera( ),
                                       new Path( ),
                                       reto::CameraController::TProjection::ORTHOGRAPHIC,
                                       reto::CameraController::TCamera::STANDARD,
                                       Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
                                       500.0f,
                                       5.0f, 0.01f );
    cameraController.zoom( -0.4f );
    // Camera default width | height: 1920 | 1080.
    Eigen::Matrix4f expectedProjMatrix;
    expectedProjMatrix << 0.00173611f,          0.0f,          0.0f,          0.0f,
                                 0.0f,   0.00308642f,          0.0f,          0.0f,
                                 0.0f,          0.0f, -0.000200002f,     -1.00002f,
                                 0.0f,          0.0f,          0.0f,          1.0f;

    checkEqualMatrices( cameraController.camera( )->projMatrix( ),
                        expectedProjMatrix );
  }
}

BOOST_AUTO_TEST_CASE( camera_controller_resize )
{
  // Perspective.
  {
    CameraController cameraController;
    cameraController.resize( 576.0f, 768.0f );
    Eigen::Matrix4f expectedProjMatrix;
    expectedProjMatrix << 3.21895f,       0.0f,       0.0f,       0.0f,
                              0.0f,   2.41421f,       0.0f,       0.0f,
                              0.0f,       0.0f,  -1.00002f, -0.200002f,
                              0.0f,       0.0f,      -1.0f,       0.0f;
    checkEqualMatrices( cameraController.camera( )->projMatrix( ),
                        expectedProjMatrix );
  }
  // Orthographic.
  {
    CameraController cameraController( new Camera( ),
                                       new Path( ),
                                       reto::CameraController::TProjection::ORTHOGRAPHIC,
                                       reto::CameraController::TCamera::STANDARD,
                                       Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
                                       500.0f,
                                       5.0f, 0.01f );
    cameraController.resize( 576.0f, 768.0f );
    Eigen::Matrix4f expectedProjMatrix;
    expectedProjMatrix << 0.00347222f,          0.0f,          0.0f,          0.0f,
                                 0.0f,   0.00260417f,          0.0f,          0.0f,
                                 0.0f,          0.0f, -0.000200002f,     -1.00002f,
                                 0.0f,          0.0f,          0.0f,          1.0f;
    checkEqualMatrices( cameraController.camera( )->projMatrix( ),
                        expectedProjMatrix );
  }
}



