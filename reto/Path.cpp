/*
 * Copyright (c) 2014-2016 GMRV/URJC.
 *
 * Authors: Juan Guerrero Martín
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

#include "Path.h"

namespace reto
{

  Path::Path( TInterpolationMethod interpolationMethod_ )
  {
    _interpolationMethod = interpolationMethod_;
  }

  Path::Path( const std::vector< Eigen::Vector3f >& positions_,
              const std::vector< Eigen::Vector3f >& lookAts_,
              const std::vector< Eigen::Vector3f >& ups_,
              TInterpolationMethod interpolationMethod_ )
  {
    _positions = positions_;
    _lookAts = lookAts_;
    _ups = ups_;
    _interpolationMethod = interpolationMethod_;
  }

  Path::~Path( void )
  {
    clearNodes( );
  }

  bool Path::empty( void )
  {
    return ( _positions.empty( ) &&
             _lookAts.empty( ) &&
             _ups.empty( ) );
  }

  unsigned int Path::nodesSize( void )
  {
    return _positions.size( );
  }

  void Path::clearNodes( void )
  {
    _positions.clear( );
    _lookAts.clear( );
    _ups.clear( );
  }

  void Path::addNode( const Eigen::Vector3f& position_,
                      const Eigen::Vector3f& lookAt_,
                      const Eigen::Vector3f& up_ )
  {
    _positions.push_back( position_ );
    _lookAts.push_back( lookAt_ );
    _ups.push_back( up_ );
  }

  Eigen::Vector3f Path::evaluatePosition( unsigned int originNodeId_,
                                          float t_ )
  {
    Eigen::Vector3f evaluatedPosition( -1.0f );

    unsigned int destinationNodeId = ( originNodeId_ == _positions.size() - 1 )
                                     ? 0
                                     : originNodeId_ + 1;

    Eigen::Vector3f p1 = _positions[ originNodeId_ ];
    Eigen::Vector3f p2 = _positions[ destinationNodeId  ];

    switch( _interpolationMethod )
    {
      case LERP :
      {
        evaluatedPosition = Eigen::Vector3f
        {
          _lerp( p1.x( ), p2.x( ), t_ ),
          _lerp( p1.y( ), p2.y( ), t_ ),
          _lerp( p1.z( ), p2.z( ), t_ )
        };
      }
      break;

      case CATMULL_ROM :
      {
        unsigned int preOriginNodeId = ( originNodeId_ == 0 )
                                       ?  _positions.size( ) - 1
                                       :  originNodeId_ - 1;

        Eigen::Vector3f p0 = _positions[ preOriginNodeId ];

        unsigned int postDestinationNodeId =
          ( destinationNodeId == _positions.size( ) - 1 )
          ?  0
          :  destinationNodeId + 1;

        Eigen::Vector3f p3 = _positions[ postDestinationNodeId ];

        evaluatedPosition = Eigen::Vector3f
        {
          _catmullRom( p0.x( ), p1.x( ), p2.x( ), p3.x( ), t_ ),
          _catmullRom( p0.y( ), p1.y( ), p2.y( ), p3.y( ), t_ ),
          _catmullRom( p0.z( ), p1.z( ), p2.z( ), p3.z( ), t_ )
        };
      }
      break;

      default :
        std::cerr << "ERROR: No interpolation method selected." << std::endl;
    }

    return evaluatedPosition;
   }

  Eigen::Matrix3f Path::evaluateOrientation( unsigned int originNodeId_,
                                             float t_ )
  {
    unsigned int destinationNodeId = ( originNodeId_ == _positions.size() - 1 )
                                       ? 0
                                       : originNodeId_ + 1;

    // Normalizing. Just in case.

    // Origin node.
    Eigen::Vector3f originLookAtVector = _lookAts[ originNodeId_ ];
    originLookAtVector.normalize( );
    Eigen::Vector3f originUpVector = _ups[ originNodeId_ ];
    originUpVector.normalize( );
    Eigen::Vector3f originTangentVector = originLookAtVector.cross( originUpVector );
    originTangentVector.normalize( );
    originUpVector = originTangentVector.cross( originLookAtVector );

    // Destination node.
    Eigen::Vector3f destinationLookAtVector = _lookAts[ destinationNodeId ];
    destinationLookAtVector.normalize( );
    Eigen::Vector3f destinationUpVector = _ups[ destinationNodeId ];
    destinationUpVector.normalize( );
    Eigen::Vector3f destinationTangentVector = destinationLookAtVector.cross( destinationUpVector );
    destinationTangentVector.normalize( );
    destinationUpVector = destinationTangentVector.cross( destinationLookAtVector );

    // Interpolation method: slerp.

    Eigen::Quaternionf baseQuaternion;
    baseQuaternion = Eigen::Quaternionf::Identity();

    // LookAt quaternion.
    Eigen::Quaternionf lookAtQuaternion;
    lookAtQuaternion.setFromTwoVectors( originLookAtVector, destinationLookAtVector );
    Eigen::Vector3f interpolatedLookAtVector =
     ( baseQuaternion.slerp( t_, lookAtQuaternion ) ) * originLookAtVector;

    // Up quaternion.
    Eigen::Quaternionf upQuaternion;
    upQuaternion.setFromTwoVectors( originUpVector, destinationUpVector );
    Eigen::Vector3f interpolatedUpVector =
      ( baseQuaternion.slerp( t_, upQuaternion ) ) * originUpVector;

    // Tangent quaternion.
    Eigen::Quaternionf tangentQuaternion;
    tangentQuaternion.setFromTwoVectors( originTangentVector, destinationTangentVector );
    Eigen::Vector3f interpolatedTangentVector =
      ( baseQuaternion.slerp( t_, tangentQuaternion ) ) * originTangentVector;

    Eigen::Matrix3f interpolatedOrientation;
    interpolatedOrientation
    << interpolatedTangentVector.x( ), interpolatedUpVector.x( ), -interpolatedLookAtVector.x( ),
       interpolatedTangentVector.y( ), interpolatedUpVector.y( ), -interpolatedLookAtVector.y( ),
       interpolatedTangentVector.z( ), interpolatedUpVector.z( ), -interpolatedLookAtVector.z( );

    return interpolatedOrientation;
  }

  float Path::_lerp( float p1, float p2, float t )
  {
    return ( p1 + t * ( p2 - p1 ) );
  }

  float Path::_catmullRom( float p0, float p1, float p2, float p3, float t )
  {
    float v0 = ( p2 - p0 ) * 0.5f;
    float v1 = ( p3 - p1 ) * 0.5f;
    float t2 = t * t;
    float t3 = t * t2;
    return ( 2.0f * p1 - 2.0f * p2 + v0 + v1 ) * t3 +
           ( -3.0f * p1 + 3.0f * p2 - 2.0f * v0 - v1 ) * t2 + v0 * t + p1;
  }

}
