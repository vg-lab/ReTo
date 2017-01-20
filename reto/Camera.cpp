/*
 * Copyright (c) 2014-2016 GMRV/URJC.
 *
 * Authors: Juan Jose Garcia
 * Authors: Cristian Rodríguez Bernal
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
#include "Camera.h"

#include <cmath>

namespace reto
{

  Camera::Camera( float width_, float height_,
                  float nearPlane_, float farPlane_,
                  Eigen::Vector3f position_,
                  float yaw_, float pitch_ )
  : _width( width_ )
  , _height( height_ )
  , _nearPlane( nearPlane_ )
  , _farPlane( farPlane_ )
  {
    _ratio = width_ / height_;
    std::vector<float> positionVec;
    positionVec.resize( 3 );
    positionVec[ 0 ] = position_( 0 );
    positionVec[ 1 ] = position_( 1 );
    positionVec[ 2 ] = position_( 2 );
    _PositionVectorized( positionVec );
    _Rotation( _RotationFromPY( pitch_, yaw_ ) );
    _BuildViewMatrix( );
  }

  Camera::~Camera( void )
  {
  }

  // BASIC FUNCTIONS

  void Camera::localRotation( float yaw_, float pitch_ )
  {
    _Rotation( _RotationFromPY( yaw_, pitch_) * _rotation );
    _BuildViewMatrix( );
  }

  // GETTERS & SETTERS

  int Camera::width( void )
  {
    return this->_width;
  }

  int Camera::height( void )
  {
    return this->_height;
  }

  void Camera::setWindowSize( const int width_, const int height_ )
  {
    this->_width = width_;
    this->_height = height_;
  }

  void Camera::ratio( const float ratio_ )
  {
    _ratio = ratio_;
  }

  float* Camera::position( void )
  {
    return _positionVec.data( );
  }

  float* Camera::viewMatrix( void )
  {
    return _viewVec.data( );
  }

  float* Camera::projectionMatrix( void )
  {
    return _projVec.data( );
  }

  float* Camera::viewProjectionMatrix( void )
  {
    return _viewProjVec.data( );
  }

  // PROTECTED

  Eigen::Matrix3f Camera::_RotationFromPY( const float yaw_, const float pitch_ )
  {
    Eigen::Matrix3f rot;
    Eigen::Matrix3f rYaw;
    Eigen::Matrix3f rPitch;

    float sinYaw, cosYaw, sinPitch, cosPitch;

    sinYaw = sin( yaw_ );
    cosYaw = cos( yaw_ );
    sinPitch = sin( pitch_ );
    cosPitch = cos( pitch_ );

    rYaw << cosYaw, 0.0f, sinYaw,
            0.0f,   1.0f, 0.0f,
            -sinYaw, 0.0f, cosYaw;

    rPitch << 1.0f, 0.0f,     0.0f,
              0.0f, cosPitch, -sinPitch,
              0.0f, sinPitch, cosPitch;

    rot = rPitch * rYaw;
    return rot;
  }

  void Camera::_BuildViewMatrix( void )
  {
    std::vector<float> viewVec(16);

    // row 1
    viewVec[0] = _rotation( 0, 0 );
    viewVec[1] = _rotation( 1, 0 );
    viewVec[2] = _rotation( 2, 0 );
    viewVec[3] = .0f;
    // row 2
    viewVec[4] = _rotation( 0, 1 );
    viewVec[5] = _rotation( 1, 1 );
    viewVec[6] = _rotation( 2, 1 );
    viewVec[7] = .0f;
    // row 3
    viewVec[8] = _rotation( 0, 2 );
    viewVec[9] = _rotation( 1, 2 );
    viewVec[10] = _rotation( 2, 2 );
    viewVec[11] = .0f;
    // row 4
    viewVec[12] = _positionVec[ 0 ];
    viewVec[13] = _positionVec[ 1 ];
    viewVec[14] = _positionVec[ 2 ];
    viewVec[15] = 1.0f;

    _ViewMatrixVectorized( viewVec );
  }

  void Camera::_PositionVectorized( const std::vector<float>& positionVec_ )
  {
    _positionVec = positionVec_;
  }

  void Camera::_ViewMatrixVectorized( const std::vector<float>& viewVec_ )
  {
    _viewVec = viewVec_;
  }

  void Camera::_Rotation( const Eigen::Matrix3f& rotation_ )
  {
    _rotation = rotation_;
  }

} // end namespace reto
