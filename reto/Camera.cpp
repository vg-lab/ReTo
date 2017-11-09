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
    // Initializing matrices.
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

  void Camera::position( const Eigen::Vector3f& position_ )
  {
    _position = position_;
  }

  Eigen::Vector3f Camera::up( void ) const
  {
    return _up;
  }

  void Camera::up( const Eigen::Vector3f& up_ )
  {
    _up = up_;
  }

  Eigen::Vector3f Camera::lookAt( void ) const
  {
    return _lookAt;
  }

  void Camera::lookAt( const Eigen::Vector3f& lookAt_ )
  {
    _lookAt = lookAt_;
  }

  Eigen::Matrix4f Camera::viewMatrix( void ) const
  {
    return _viewMatrix;
  }

  void Camera::viewMatrix( const Eigen::Matrix4f& viewMatrix_ )
  {
    _viewMatrix = viewMatrix_;
  }

  float* Camera::viewMatrixData( void )
  {
    return _viewMatrix.data( );
  }

  Eigen::Matrix4f Camera::projMatrix( void ) const
  {
    return _projMatrix;
  }

  void Camera::projMatrix( const Eigen::Matrix4f& projMatrix_ )
  {
    _projMatrix = projMatrix_;
  }

  float* Camera::projMatrixData( void )
  {
    return _projMatrix.data( );
  }

  Eigen::Matrix4f Camera::viewProjMatrix( void ) const
  {
    return _viewProjMatrix;
  }

  void Camera::viewProjMatrix( const Eigen::Matrix4f& viewProjMatrix_ )
  {
    _viewProjMatrix = viewProjMatrix_;
  }

  float* Camera::viewProjMatrixData( void )
  {
    return _viewProjMatrix.data( );
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
