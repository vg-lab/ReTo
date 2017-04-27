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

#ifdef RETO_USE_ZEROEQ
  Camera::Camera( const std::string& zeqSession_,
                  unsigned int width_, unsigned int height_,
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
  , _zeqConnection( true )
  {
    // ZeroEQ section.
    _zeroeqSession = zeqSession_.empty( ) ? zeroeq::DEFAULT_SESSION : zeqSession_;

    _publisher = new zeroeq::Publisher( _zeroeqSession );

    _subscriber = new zeroeq::Subscriber( _zeroeqSession );
    _subscriber->subscribe(
      lexis::render::LookOut::ZEROBUF_TYPE_IDENTIFIER( ),
      [ & ]( const void* data, size_t size )
      {
        _OnCameraEvent( lexis::render::LookOut::create( data, size ));
      } );

    _subscriberThread =
      new std::thread(
        [&]( )
        {
          while( true ) _subscriber->receive( 10000 );
        } );

    // Initializing matrices.
    _viewMatrix = Eigen::Matrix4f::Identity( );
    _projMatrix = Eigen::Matrix4f::Identity( );
    _viewProjMatrix = Eigen::Matrix4f::Identity( );
  }
#endif

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

  void Camera::viewMatrix( const Eigen::Matrix4f& viewMatrix_ )
  {
    _viewMatrix( 0, 0 ) = viewMatrix_( 0, 0 ); _viewMatrix( 1, 0 ) = viewMatrix_( 1, 0 );
    _viewMatrix( 2, 0 ) = viewMatrix_( 2, 0 ); _viewMatrix( 3, 0 ) = viewMatrix_( 3, 0 );
    _viewMatrix( 0, 1 ) = viewMatrix_( 0, 1 ); _viewMatrix( 1, 1 ) = viewMatrix_( 1, 1 );
    _viewMatrix( 2, 1 ) = viewMatrix_( 2, 1 ); _viewMatrix( 3, 1 ) = viewMatrix_( 3, 1 );
    _viewMatrix( 0, 2 ) = viewMatrix_( 0, 2 ); _viewMatrix( 1, 2 ) = viewMatrix_( 1, 2 );
    _viewMatrix( 2, 2 ) = viewMatrix_( 2, 2 ); _viewMatrix( 3, 2 ) = viewMatrix_( 3, 2 );
    _viewMatrix( 0, 3 ) = viewMatrix_( 0, 3 ); _viewMatrix( 1, 3 ) = viewMatrix_( 1, 3 );
    _viewMatrix( 2, 3 ) = viewMatrix_( 2, 3 ); _viewMatrix( 3, 3 ) = viewMatrix_( 3, 3 );

#ifdef RETO_USE_ZEROEQ
    // Each time view matrix is changed then it is published.
    std::vector< float > currentViewMatrixAsVector
      = _matrix4fToVector( viewMatrix_ );

    viewMatrixAsVector( currentViewMatrixAsVector );

    if ( _zeqConnection )
    {
      std::vector< double > viewm( _viewMatrixAsVector.begin( ),
                                   _viewMatrixAsVector.end( ) );
      lexis::render::LookOut lookout;
      lookout.setMatrix( viewm );
      _publisher->publish( lookout );
    }
#endif
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

  void Camera::projMatrix( const Eigen::Matrix4f& projMatrix_ )
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

  void Camera::viewProjMatrix( const Eigen::Matrix4f& viewProjMatrix_ )
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

#ifdef RETO_USE_ZEROEQ
  zeroeq::Subscriber* Camera::subscriber( void )
  {
    return _subscriber;
  }

  void Camera::viewMatrixAsVector( const std::vector<float>& viewMatrixAsVector_ )
  {
    _viewMatrixMutex.lock( );
    _viewMatrixAsVector = viewMatrixAsVector_;
    _viewMatrixMutex.unlock( );
  }

  std::vector< float > Camera::_matrix4fToVector( const Eigen::Matrix4f& matrix_ )
  {
    std::vector< float > toReturn( 16 );

    // Column 0.
    toReturn[0] = matrix_( 0, 0 );
    toReturn[1] = matrix_( 1, 0 );
    toReturn[2] = matrix_( 2, 0 );
    toReturn[3] = matrix_( 3, 0 );

    // Column 1.
    toReturn[4] = matrix_( 0, 1 );
    toReturn[5] = matrix_( 1, 1 );
    toReturn[6] = matrix_( 2, 1 );
    toReturn[7] = matrix_( 3, 1 );

    // Column 2.
    toReturn[8] = matrix_( 0, 2 );
    toReturn[9] = matrix_( 1, 2 );
    toReturn[10] = matrix_( 2, 2 );
    toReturn[11] = matrix_( 3, 2 );

    // Column 3.
    toReturn[12] = matrix_( 0, 3 );
    toReturn[13] = matrix_( 1, 3 );
    toReturn[14] = matrix_( 2, 3 );
    toReturn[15] = matrix_( 3, 3 );

    return toReturn;
  }

  Eigen::Matrix4f Camera::_vectorToMatrix4f( const std::vector<float>& vector_  )
  {
    Eigen::Matrix4f toReturn = Eigen::Matrix4f::Identity( );

    // Column 0.
    toReturn( 0, 0 ) = vector_[0];
    toReturn( 1, 0 ) = vector_[1];
    toReturn( 2, 0 ) = vector_[2];
    toReturn( 3, 0 ) = vector_[3];

    // Column 1.
    toReturn( 0, 1 ) = vector_[4];
    toReturn( 1, 1 ) = vector_[5];
    toReturn( 2, 1 ) = vector_[6];
    toReturn( 3, 1 ) = vector_[7];

    // Column 2.
    toReturn( 0, 2 ) = vector_[8];
    toReturn( 1, 2 ) = vector_[9];
    toReturn( 2, 2 ) = vector_[10];
    toReturn( 3, 2 ) = vector_[11];

    // Column 3.
    toReturn( 0, 3 ) = vector_[12];
    toReturn( 1, 3 ) = vector_[13];
    toReturn( 2, 3 ) = vector_[14];
    toReturn( 3, 3 ) = vector_[15];

    return toReturn;
  }

  void Camera::_OnCameraEvent( lexis::render::ConstLookOutPtr lookoutPtr_ )
  {
    // A new view matrix is received and the local one is updated.
    std::vector< double > aux = std::move( lookoutPtr_->getMatrixVector( ) );

    std::vector< float > currentViewMatrixAsVector( aux.begin( ),
                                                    aux.end( ) );

    viewMatrixAsVector( currentViewMatrixAsVector );

    _viewMatrix = _vectorToMatrix4f( _viewMatrixAsVector );
  }
#endif

} // end namespace reto
