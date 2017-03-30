/*
 * Copyright (c) 2014-2017 GMRV/URJC.
 *
 * Authors: Juan Guerrero Martín
 * Authors: Cristian Rodríguez Bernal
 * Authors: Juan Jose Garcia
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

namespace reto
{

  Camera::Camera( unsigned int width_, unsigned int height_,
                  float nearPlane_, float farPlane_,
                  Eigen::Vector3f position_,
                  Eigen::Vector3f up_,
                  Eigen::Vector3f lookAt_,
                  float fov_ )
  : _width( width_ )
  , _height( height_ )
  , _nearPlane( nearPlane_ )
  , _farPlane( farPlane_ )
  , _position( position_ )
  , _up( up_ )
  , _lookAt( lookAt_ )
  , _fov( fov_ )
  {
    _viewMatrix = Eigen::Matrix4f::Identity( );
    _projMatrix = Eigen::Matrix4f::Identity( );
    _viewProjMatrix = Eigen::Matrix4f::Identity( );
  }

  Camera::~Camera( void )
  {
  }

  unsigned int Camera::width( void ) const
  {
    return _width;
  }

  void Camera::width( unsigned int width_ )
  {
    _width = width_;
  }

  unsigned int Camera::height( void ) const
  {
    return _height;
  }

  void Camera::height( unsigned int height_ )
  {
    _height = height_;
  }

  float Camera::nearPlane( void ) const
  {
    return _nearPlane;
  }

  void Camera::nearPlane( float nearPlane_ )
  {
    _nearPlane = nearPlane_;
  }

  float Camera::farPlane( void ) const
  {
    return _farPlane;
  }

  void Camera::farPlane( float farPlane_ )
  {
    _farPlane = farPlane_;
  }

  Eigen::Vector3f Camera::position( void ) const
  {
    return _position;
  }

  void Camera::position( Eigen::Vector3f position_ )
  {
    _position = position_;
  }

  Eigen::Vector3f Camera::up( void ) const
  {
    return _up;
  }

  void Camera::up( Eigen::Vector3f up_ )
  {
    _up = up_;
  }

  Eigen::Vector3f Camera::lookAt( void ) const
  {
    return _lookAt;
  }

  void Camera::lookAt( Eigen::Vector3f lookAt_ )
  {
    _lookAt = lookAt_;
  }

  Eigen::Matrix4f Camera::viewMatrix( void ) const
  {
    Eigen::Matrix4f viewMatrix_ = Eigen::Matrix4f::Identity( );
    viewMatrix_( 0, 0 ) = _viewMatrix( 0, 0 ); viewMatrix_( 1, 0 ) = _viewMatrix( 1, 0 );
    viewMatrix_( 2, 0 ) = _viewMatrix( 2, 0 ); viewMatrix_( 3, 0 ) = _viewMatrix( 3, 0 );
    viewMatrix_( 0, 1 ) = _viewMatrix( 0, 1 ); viewMatrix_( 1, 1 ) = _viewMatrix( 1, 1 );
    viewMatrix_( 2, 1 ) = _viewMatrix( 2, 1 ); viewMatrix_( 3, 1 ) = _viewMatrix( 3, 1 );
    viewMatrix_( 0, 2 ) = _viewMatrix( 0, 2 ); viewMatrix_( 1, 2 ) = _viewMatrix( 1, 2 );
    viewMatrix_( 2, 2 ) = _viewMatrix( 2, 2 ); viewMatrix_( 3, 2 ) = _viewMatrix( 3, 2 );
    viewMatrix_( 0, 3 ) = _viewMatrix( 0, 3 ); viewMatrix_( 1, 3 ) = _viewMatrix( 1, 3 );
    viewMatrix_( 2, 3 ) = _viewMatrix( 2, 3 ); viewMatrix_( 3, 3 ) = _viewMatrix( 3, 3 );
    return viewMatrix_;
  }

  void Camera::viewMatrix( Eigen::Matrix4f viewMatrix_ )
  {
    _viewMatrix( 0, 0 ) = viewMatrix_( 0, 0 ); _viewMatrix( 1, 0 ) = viewMatrix_( 1, 0 );
    _viewMatrix( 2, 0 ) = viewMatrix_( 2, 0 ); _viewMatrix( 3, 0 ) = viewMatrix_( 3, 0 );
    _viewMatrix( 0, 1 ) = viewMatrix_( 0, 1 ); _viewMatrix( 1, 1 ) = viewMatrix_( 1, 1 );
    _viewMatrix( 2, 1 ) = viewMatrix_( 2, 1 ); _viewMatrix( 3, 1 ) = viewMatrix_( 3, 1 );
    _viewMatrix( 0, 2 ) = viewMatrix_( 0, 2 ); _viewMatrix( 1, 2 ) = viewMatrix_( 1, 2 );
    _viewMatrix( 2, 2 ) = viewMatrix_( 2, 2 ); _viewMatrix( 3, 2 ) = viewMatrix_( 3, 2 );
    _viewMatrix( 0, 3 ) = viewMatrix_( 0, 3 ); _viewMatrix( 1, 3 ) = viewMatrix_( 1, 3 );
    _viewMatrix( 2, 3 ) = viewMatrix_( 2, 3 ); _viewMatrix( 3, 3 ) = viewMatrix_( 3, 3 );
  }

  Eigen::Matrix4f Camera::projMatrix( void ) const
  {
    Eigen::Matrix4f projMatrix_ = Eigen::Matrix4f::Identity( );
    projMatrix_( 0, 0 ) = _projMatrix( 0, 0 ); projMatrix_( 1, 0 ) = _projMatrix( 1, 0 );
    projMatrix_( 2, 0 ) = _projMatrix( 2, 0 ); projMatrix_( 3, 0 ) = _projMatrix( 3, 0 );
    projMatrix_( 0, 1 ) = _projMatrix( 0, 1 ); projMatrix_( 1, 1 ) = _projMatrix( 1, 1 );
    projMatrix_( 2, 1 ) = _projMatrix( 2, 1 ); projMatrix_( 3, 1 ) = _projMatrix( 3, 1 );
    projMatrix_( 0, 2 ) = _projMatrix( 0, 2 ); projMatrix_( 1, 2 ) = _projMatrix( 1, 2 );
    projMatrix_( 2, 2 ) = _projMatrix( 2, 2 ); projMatrix_( 3, 2 ) = _projMatrix( 3, 2 );
    projMatrix_( 0, 3 ) = _projMatrix( 0, 3 ); projMatrix_( 1, 3 ) = _projMatrix( 1, 3 );
    projMatrix_( 2, 3 ) = _projMatrix( 2, 3 ); projMatrix_( 3, 3 ) = _projMatrix( 3, 3 );
    return projMatrix_;
  }

  void Camera::projMatrix( Eigen::Matrix4f projMatrix_ )
  {
    _projMatrix( 0, 0 ) = projMatrix_( 0, 0 ); _projMatrix( 1, 0 ) = projMatrix_( 1, 0 );
    _projMatrix( 2, 0 ) = projMatrix_( 2, 0 ); _projMatrix( 3, 0 ) = projMatrix_( 3, 0 );
    _projMatrix( 0, 1 ) = projMatrix_( 0, 1 ); _projMatrix( 1, 1 ) = projMatrix_( 1, 1 );
    _projMatrix( 2, 1 ) = projMatrix_( 2, 1 ); _projMatrix( 3, 1 ) = projMatrix_( 3, 1 );
    _projMatrix( 0, 2 ) = projMatrix_( 0, 2 ); _projMatrix( 1, 2 ) = projMatrix_( 1, 2 );
    _projMatrix( 2, 2 ) = projMatrix_( 2, 2 ); _projMatrix( 3, 2 ) = projMatrix_( 3, 2 );
    _projMatrix( 0, 3 ) = projMatrix_( 0, 3 ); _projMatrix( 1, 3 ) = projMatrix_( 1, 3 );
    _projMatrix( 2, 3 ) = projMatrix_( 2, 3 ); _projMatrix( 3, 3 ) = projMatrix_( 3, 3 );
  }

  Eigen::Matrix4f Camera::viewProjMatrix( void ) const
  {
    Eigen::Matrix4f viewProjMatrix_ = Eigen::Matrix4f::Identity( );
    viewProjMatrix_( 0, 0 ) = _viewProjMatrix( 0, 0 ); viewProjMatrix_( 1, 0 ) = _viewProjMatrix( 1, 0 );
    viewProjMatrix_( 2, 0 ) = _viewProjMatrix( 2, 0 ); viewProjMatrix_( 3, 0 ) = _viewProjMatrix( 3, 0 );
    viewProjMatrix_( 0, 1 ) = _viewProjMatrix( 0, 1 ); viewProjMatrix_( 1, 1 ) = _viewProjMatrix( 1, 1 );
    viewProjMatrix_( 2, 1 ) = _viewProjMatrix( 2, 1 ); viewProjMatrix_( 3, 1 ) = _viewProjMatrix( 3, 1 );
    viewProjMatrix_( 0, 2 ) = _viewProjMatrix( 0, 2 ); viewProjMatrix_( 1, 2 ) = _viewProjMatrix( 1, 2 );
    viewProjMatrix_( 2, 2 ) = _viewProjMatrix( 2, 2 ); viewProjMatrix_( 3, 2 ) = _viewProjMatrix( 3, 2 );
    viewProjMatrix_( 0, 3 ) = _viewProjMatrix( 0, 3 ); viewProjMatrix_( 1, 3 ) = _viewProjMatrix( 1, 3 );
    viewProjMatrix_( 2, 3 ) = _viewProjMatrix( 2, 3 ); viewProjMatrix_( 3, 3 ) = _viewProjMatrix( 3, 3 );
    return viewProjMatrix_;
  }

  void Camera::viewProjMatrix( Eigen::Matrix4f viewProjMatrix_ )
  {
    _viewProjMatrix( 0, 0 ) = viewProjMatrix_( 0, 0 ); _viewProjMatrix( 1, 0 ) = viewProjMatrix_( 1, 0 );
    _viewProjMatrix( 2, 0 ) = viewProjMatrix_( 2, 0 ); _viewProjMatrix( 3, 0 ) = viewProjMatrix_( 3, 0 );
    _viewProjMatrix( 0, 1 ) = viewProjMatrix_( 0, 1 ); _viewProjMatrix( 1, 1 ) = viewProjMatrix_( 1, 1 );
    _viewProjMatrix( 2, 1 ) = viewProjMatrix_( 2, 1 ); _viewProjMatrix( 3, 1 ) = viewProjMatrix_( 3, 1 );
    _viewProjMatrix( 0, 2 ) = viewProjMatrix_( 0, 2 ); _viewProjMatrix( 1, 2 ) = viewProjMatrix_( 1, 2 );
    _viewProjMatrix( 2, 2 ) = viewProjMatrix_( 2, 2 ); _viewProjMatrix( 3, 2 ) = viewProjMatrix_( 3, 2 );
    _viewProjMatrix( 0, 3 ) = viewProjMatrix_( 0, 3 ); _viewProjMatrix( 1, 3 ) = viewProjMatrix_( 1, 3 );
    _viewProjMatrix( 2, 3 ) = viewProjMatrix_( 2, 3 ); _viewProjMatrix( 3, 3 ) = viewProjMatrix_( 3, 3 );
  }

  float Camera::fov( void ) const
  {
    return _fov;
  }

  void Camera::fov( float fov_ )
  {
    _fov = fov_;
  }

} // end namespace reto
