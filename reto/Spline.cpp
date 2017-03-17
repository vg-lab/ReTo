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

#include "Spline.h"
#include <stdexcept>
#include <iostream>
#include <math.h>

namespace reto
{

  Spline::Spline( void )
  {
    std::vector< Eigen::Vector3f > eyes_;
    eyes_.push_back( Eigen::Vector3f( 0.0f ) );
    this->_eyes = eyes_;

    std::vector< Eigen::Vector3f > centers_;
    centers_.push_back( Eigen::Vector3f( 0.0f ) );
    this->_centers = centers_;

    std::vector< Eigen::Vector3f > ups_;
    ups_.push_back( Eigen::Vector3f( 0.0f ) );
    this->_ups = ups_;

    //this->_olddt = this->_currentdt = 0.0f;
  }

  Spline::Spline( const std::vector< Eigen::Vector3f >& eyes_,
                  const std::vector< Eigen::Vector3f >& centers_,
                  const std::vector< Eigen::Vector3f >& ups_ )
  {
    this->_eyes = eyes_;
    this->_centers = centers_;
    this->_ups = ups_;
    //this->_olddt = this->_currentdt = 0.0f;
  }

  Eigen::Vector3f Spline::evaluatePosition( unsigned int firstId,
                                            float t )
  {
    //this->_olddt = this->_currentdt;
    //this->_currentdt = dt;

    unsigned int secondId = ( firstId == this->_eyes.size( ) - 1 )
                            ? 0
                            : firstId + 1;

    unsigned int preFirstId = ( firstId == 0 )
                              ?  this->_eyes.size( ) - 1
                              :  firstId - 1;

    unsigned int postSecondId = ( secondId == this->_eyes.size( ) - 1 )
                              ?  0
                              :  secondId + 1;

    Eigen::Vector3f p0 = this->_eyes[ preFirstId ];
    Eigen::Vector3f p1 = this->_eyes[ firstId ];
    Eigen::Vector3f p2 = this->_eyes[ secondId  ];
    Eigen::Vector3f p3 = this->_eyes[ postSecondId ];

    return Eigen::Vector3f
      {
        this->_catmullRom(
          p0( 0 ), p1( 0 ), p2( 0 ), p3( 0 ), t
        ),
        this->_catmullRom(
          p0( 1 ), p1( 1 ), p2( 1 ), p3( 1 ), t
        ),
        this->_catmullRom(
          p0( 2 ), p1( 2 ), p2( 2 ), p3( 2 ), t
        )
      };
  }

  Eigen::Matrix3f Spline::evaluateOrientation( unsigned int firstId,
                                               float t )
  {
    unsigned int secondId = ( firstId == this->_eyes.size( ) - 1 )
                            ? 0
                            : firstId + 1;

    Eigen::Vector3f firstCenterPoint = this->_centers[ firstId ];
    std::cout << "firstCenterPoint: (" << firstCenterPoint.x() << ", "
                                       << firstCenterPoint.y() << ", "
                                       << firstCenterPoint.z() << ")" << std::endl;
    Eigen::Vector3f firstEyePoint = this->_eyes[ firstId ];
    std::cout << "firstEyePoint: (" << firstEyePoint.x() << ", "
                                    << firstEyePoint.y() << ", "
                                    << firstEyePoint.z() << ")" << std::endl;
    Eigen::Vector3f firstCenterVector = firstCenterPoint - firstEyePoint;
    std::cout << "firstCenterVector: (" << firstCenterVector.x() << ", "
                                        << firstCenterVector.y() << ", "
                                        << firstCenterVector.z() << ")" << std::endl;
    firstCenterVector.normalize();
    Eigen::Vector3f firstUpPoint = this->_ups[ firstId ];
    Eigen::Vector3f firstUpVector = firstUpPoint.normalized();
    Eigen::Vector3f firstTangentVector = firstCenterVector.cross( firstUpVector );
    firstTangentVector.normalize();
    firstUpVector = firstTangentVector.cross( firstCenterVector );

    /*
    Eigen::Matrix3f firstOrientation;
    firstOrientation << firstTangentVector.x( ), firstUpVector.x( ), -firstCenterVector.x( ),
                        firstTangentVector.y( ), firstUpVector.y( ), -firstCenterVector.y( ),
                        firstTangentVector.z( ), firstUpVector.z( ), -firstCenterVector.z( );

    std::cout << "firstOrientation: " << std::endl;
    std::cout << "("  << firstOrientation(0,0) << ", " << firstOrientation(1,0) << ", "  << firstOrientation(2,0) << ")" << std::endl;
    std::cout << "("  << firstOrientation(0,1) << ", " << firstOrientation(1,1) << ", "  << firstOrientation(2,1) << ")" << std::endl;
    std::cout << "("  << firstOrientation(0,2) << ", " << firstOrientation(1,2) << ", "  << firstOrientation(2,2) << ")" << std::endl;
    */

    Eigen::Vector3f secondCenterPoint = this->_centers[ secondId ];
    std::cout << "secondCenterPoint: (" << secondCenterPoint.x() << ", "
                                        << secondCenterPoint.y() << ", "
                                        << secondCenterPoint.z() << ")" << std::endl;
    Eigen::Vector3f secondEyePoint = this->_eyes[ secondId ];
    std::cout << "secondEyePoint: (" << secondEyePoint.x() << ", "
                                     << secondEyePoint.y() << ", "
                                     << secondEyePoint.z() << ")" << std::endl;
    Eigen::Vector3f secondCenterVector = secondCenterPoint - secondEyePoint;
    std::cout << "secondCenterVector: (" << secondCenterVector.x() << ", "
                                         << secondCenterVector.y() << ", "
                                         << secondCenterVector.z() << ")" << std::endl;
    secondCenterVector.normalize( );
    Eigen::Vector3f secondUpPoint = this->_ups[ secondId ];
    Eigen::Vector3f secondUpVector = secondUpPoint.normalized( );
    Eigen::Vector3f secondTangentVector = secondCenterVector.cross( secondUpVector );
    secondTangentVector.normalize();
    secondUpVector = secondTangentVector.cross( secondCenterVector );

    /*
    Eigen::Matrix3f secondOrientation;
    secondOrientation << secondTangentVector.x( ), secondUpVector.x( ), -secondCenterVector.x( ),
                         secondTangentVector.y( ), secondUpVector.y( ), -secondCenterVector.y( ),
                         secondTangentVector.z( ), secondUpVector.z( ), -secondCenterVector.z( );

    std::cout << "secondOrientation: " << std::endl;
    std::cout << "("  << secondOrientation(0,0) << ", " << secondOrientation(1,0) << ", "  << secondOrientation(2,0) << ")" << std::endl;
    std::cout << "("  << secondOrientation(0,1) << ", " << secondOrientation(1,1) << ", "  << secondOrientation(2,1) << ")" << std::endl;
    std::cout << "("  << secondOrientation(0,2) << ", " << secondOrientation(1,2) << ", "  << secondOrientation(2,2) << ")" << std::endl;
    */

    Eigen::Quaternionf qa;
    qa = Eigen::Quaternionf::Identity();

    // Quaternio center.
    Eigen::Quaternionf qb1;
    qb1.setFromTwoVectors( firstCenterVector, secondCenterVector );
    Eigen::Vector3f ipoCenterVector = (qa.slerp(t,qb1)) * firstCenterVector;

    // Quaternio up.
    Eigen::Quaternionf qb2;
    qb2.setFromTwoVectors( firstUpVector, secondUpVector );
    Eigen::Vector3f ipoUpVector = (qa.slerp(t,qb2)) * firstUpVector;

    // Quaternio tangent.
    Eigen::Quaternionf qb3;
    qb3.setFromTwoVectors( firstTangentVector, secondTangentVector );
    Eigen::Vector3f ipoTangentVector = (qa.slerp(t,qb3)) * firstTangentVector;

    /**
    Eigen::Quaternionf firstQuaternion( firstOrientation );
    std::cout << "firstQuaternion: (" << firstQuaternion.x() << ", "
                                      << firstQuaternion.y() << ", "
                                      << firstQuaternion.z() << ", "
                                      << firstQuaternion.w() << ")" << std::endl;
    Eigen::Quaternionf secondQuaternion( secondOrientation );
    std::cout << "secondQuaternion: (" << secondQuaternion.x() << ", "
                                       << secondQuaternion.y() << ", "
                                       << secondQuaternion.z() << ", "
                                       << secondQuaternion.w() << ")" << std::endl;
    Eigen::Quaternionf interpolatedQuaternion = firstQuaternion.slerp( t, secondQuaternion );
    interpolatedQuaternion.normalize();
    Eigen::Matrix3f finalOrientation = interpolatedQuaternion.toRotationMatrix( );
    **/

    std::cout << "T: " << t << std::endl;

    Eigen::Matrix3f finalOrientation;
    finalOrientation << ipoTangentVector.x( ), ipoUpVector.x( ), -ipoCenterVector.x( ),
                        ipoTangentVector.y( ), ipoUpVector.y( ), -ipoCenterVector.y( ),
                        ipoTangentVector.z( ), ipoUpVector.z( ), -ipoCenterVector.z( );

    std::cout << "finalOrientation: " << std::endl;
    std::cout << "("  << finalOrientation(0,0) << ", " << finalOrientation(1,0) << ", "  << finalOrientation(2,0) << ")" << std::endl;
    std::cout << "("  << finalOrientation(0,1) << ", " << finalOrientation(1,1) << ", "  << finalOrientation(2,1) << ")" << std::endl;
    std::cout << "("  << finalOrientation(0,2) << ", " << finalOrientation(1,2) << ", "  << finalOrientation(2,2) << ")" << std::endl;

    return finalOrientation;
  }

  /**
  Eigen::Vector3f Spline::getTangent( void )
  {
    return this->getTangent( this->_olddt, this->_currentdt );
  }
  Eigen::Vector3f Spline::getTangent( float dt0, float dt1 )
  {
    Eigen::Vector3f p1 = this->evaluate( dt0 );
    Eigen::Vector3f p2 = this->evaluate( dt1 );
    Eigen::Vector3f rem = p2 - p1;
    rem.normalize( );
    return rem;
  }
  float Spline::angleBetweenPoints( void )
  {
    return this->angleBetweenPoints( this->_olddt, this->_currentdt );
  }

  float Spline::angleBetweenPoints( float dt0, float dt1 )
  {
    Eigen::Vector3f p1 = this->evaluate( dt0 );
    Eigen::Vector3f p2 = this->evaluate( dt1 );
    float angle = atan2( p2( 2 ) - p1( 2 ), p2( 0 ) - p1( 0 ) );
    return angle * M_PI / 180.0f;
  }
  **/

  float Spline::_catmullRom( float p0, float p1, float p2, float p3, float t )
  {
    float
      v0 = ( p2 - p0 ) * 0.5f,
      v1 = ( p3 - p1 ) * 0.5f,
      t2 = t * t,
      t3 = t * t2;
    return ( 2.0f * p1 - 2.0f * p2 + v0 + v1 ) * t3 +
      ( -3.0f * p1 + 3.0f * p2 - 2.0f * v0 - v1 ) * t2 + v0 * t + p1;
  };
}
