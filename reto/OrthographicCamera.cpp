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

#include "OrthographicCamera.h"

namespace reto
{

  OrthographicCamera::OrthographicCamera( float width_, float height_,
                                          float nearPlane_, float farPlane_,
                                          Eigen::Vector3f position_,
                                          float yaw_, float pitch_ )
  :  Camera( width_, height_, nearPlane_, farPlane_,
             position_, yaw_, pitch_ )
  {
    _BuildProjectionMatrix( );
    _BuildViewProjectionMatrix( );
  }

  OrthographicCamera::~OrthographicCamera()
  {
  }

  void OrthographicCamera::center( Eigen::Vector3f centeredPosition_ )
  {
    std::vector<float> positionVec;
    positionVec.resize( 3 );
    positionVec[ 0 ] = centeredPosition_( 0 );
    positionVec[ 1 ] = centeredPosition_( 1 );
    positionVec[ 2 ] = centeredPosition_( 2 );
    _PositionVectorized( positionVec );
    _Rotation( Eigen::Matrix3f::Identity( ) );
    _width = 1920.0f;
    _height = 1080.0f;
    _BuildViewMatrix( );
    _BuildProjectionMatrix( );
    _BuildViewProjectionMatrix( );
  }

  void OrthographicCamera::zoom( float increment_ )
  {
    float scale = 1.0f;
    scale += increment_;
    _width *= scale;
    _height *= scale;
    _BuildProjectionMatrix();
    _BuildViewProjectionMatrix();
  }

  void OrthographicCamera::localTranslation( Eigen::Vector3f increment_ )
  {
    std::vector<float> positionVec;
    positionVec.resize( 3 );
    positionVec[ 0 ] = _positionVec[ 0 ] - increment_( 0 );
    positionVec[ 1 ] = _positionVec[ 1 ] - increment_( 1 );
    positionVec[ 2 ] = _positionVec[ 2 ];
    _PositionVectorized( positionVec );
    _BuildViewMatrix();
    _BuildViewProjectionMatrix();
  }

  void OrthographicCamera::localRotation( const float yaw_, const float pitch_ )
  {
    Camera::localRotation( yaw_, pitch_ );
    _BuildViewProjectionMatrix( );
  }

  void OrthographicCamera::setWindowSize( const int width_, const int height_ )
  {
    Camera::setWindowSize( width_, height_ );
    _BuildProjectionMatrix( );
    _BuildViewProjectionMatrix( );
  }

  void OrthographicCamera::ratio( float ratio_ )
  {
    Camera::ratio( ratio_ );
    _BuildProjectionMatrix( );
    _BuildViewProjectionMatrix( );
  }

  void OrthographicCamera::rotation( const float yaw_, const float pitch_ )
  {
    //if( !_isAniming )
    //{
      _Rotation( _RotationFromPY( yaw_, pitch_ ));
      _BuildViewMatrix( );
      _BuildViewProjectionMatrix( );
    //}
  }

  void OrthographicCamera::_BuildProjectionMatrix( void )
  {
    _projVec.resize( 16 );

    float right = _width * 0.5f;
    float top = _height * 0.5f;
    // Row 1.
    _projVec.at(0) = 1.0f / right;
    _projVec.at(1) = .0f;
    _projVec.at(2) = .0f;
    _projVec.at(3) = .0f;
    // Row 2.
    _projVec.at(4) = .0f;
    _projVec.at(5) = 1.0f / top;
    _projVec.at(6) = .0f;
    _projVec.at(7) = .0f;
    // Row 3.
    _projVec.at(8) = .0f;
    _projVec.at(9) = .0f;
    _projVec.at(10) = -2.0f / ( _farPlane - _nearPlane );
    _projVec.at(11) = .0f;
    // Row 4.
    _projVec.at(12) = .0f;
    _projVec.at(13) = .0f;
    _projVec.at(14) = -1.0f * (_nearPlane + _farPlane ) / ( _farPlane - _nearPlane );
    _projVec.at(15) = 1.0f;
  }

  void OrthographicCamera::_BuildViewProjectionMatrix( void )
  {
    Eigen::Matrix4f v, p, vp;
    v << _viewVec[0], _viewVec[4], _viewVec[8], _viewVec[12],
         _viewVec[1], _viewVec[5], _viewVec[9], _viewVec[13],
         _viewVec[2], _viewVec[6], _viewVec[10], _viewVec[14],
         _viewVec[3], _viewVec[7], _viewVec[11], _viewVec[15];

    p << _projVec[0], _projVec[4], _projVec[8], _projVec[12],
         _projVec[1], _projVec[5], _projVec[9], _projVec[13],
         _projVec[2], _projVec[6], _projVec[10], _projVec[14],
         _projVec[3], _projVec[7], _projVec[11], _projVec[15];

    vp = p * v;

    _viewProjVec.resize( 16 );

    _viewProjVec[0] = vp( 0, 0 );
    _viewProjVec[1] = vp( 1, 0 );
    _viewProjVec[2] = vp( 2, 0 );
    _viewProjVec[3] = vp( 3, 0 );

    _viewProjVec[4] = vp( 0, 1 );
    _viewProjVec[5] = vp( 1, 1 );
    _viewProjVec[6] = vp( 2, 1 );
    _viewProjVec[7] = vp( 3, 1 );

    _viewProjVec[8] = vp( 0, 2 );
    _viewProjVec[9] = vp( 1, 2 );
    _viewProjVec[10] = vp( 2, 2 );
    _viewProjVec[11] = vp( 3, 2 );

    _viewProjVec[12] = vp( 0, 3 );
    _viewProjVec[13] = vp( 1, 3 );
    _viewProjVec[14] = vp( 2, 3 );
    _viewProjVec[15] = vp( 3, 3 );
  }

} /* namespace reto */
