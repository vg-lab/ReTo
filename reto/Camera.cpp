/*
 * Copyright (c) 2014-2019 GMRV/URJC.
 *
 * Authors: Juan Jose Garcia Cantero <juanjose.garcia@urjc.es>
 *          Cristian Rodríguez Bernal <cristian.rodriguez@urjc.es>
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
#define _USE_MATH_DEFINES
#include <math.h>

constexpr float HALFDEG2RAD = static_cast<float>(M_PI) / 360.0f;

namespace reto
{
  Camera::Camera( float fov_, float ratio_, float nearPlane_, float farPlane_ )
    : _ratio( ratio_ )
    , _nearPlane( nearPlane_ )
    , _farPlane( farPlane_ )
    , _isProjMatClean( true )
    , _enableZeqConnChanges( true )
    , _zeqConnection( false )
#ifdef RETO_USE_ZEROEQ
    , _zeroeqSession( )
    , _subscriber( nullptr )
    , _subscriberThread( nullptr )
#endif
  {
    _fov = fov_ * HALFDEG2RAD;
    _f = 1.0f / tan( _fov );
    _buildProjectionMatrix( );
    _viewMatrix = Eigen::Matrix4f::Identity( );
    _projectionViewMatrix = _projectionMatrix * _viewMatrix;
  }

  Camera::Camera( const std::string& session_,
#ifdef RETO_USE_ZEROEQ
        std::shared_ptr<zeroeq::Subscriber> subscriber,
#endif
      float fov_, float ratio_, float nearPlane_, float farPlane_ )
  : Camera(fov_, ratio_, nearPlane_, farPlane_)
  {
#ifdef RETO_USE_ZEROEQ
    initializeZeroEQ(session_, subscriber);
#else
    std::cout << "Warning: Could establish connection " << session_
              << " Lexis/ZeroEQ not supported" << std::endl;
#endif
  }

  Camera::~Camera( void )
  {
#ifdef RETO_USE_ZEROEQ
    deinitializeZeroEQ();
#endif
  }

  void Camera::setZeqSession( const std::string& session_
#ifdef RETO_USE_ZEROEQ
        , const std::shared_ptr<zeroeq::Subscriber> subscriber
#endif
      )
  {
#ifdef RETO_USE_ZEROEQ
    initializeZeroEQ( session_, subscriber );
#else
    std::cout << "Warning: Could establish connection " << session_
              << " Lexis/ZeroEQ not supported" << std::endl;
#endif
  }

  float Camera::nearPlane( void ) const
  {
    return _nearPlane;
  }

  void Camera::nearPlane( float nearPlane_ )
  {
    _setNearPlane( nearPlane_ );
  }

  float Camera::farPlane( void ) const
  {
    return _farPlane;
  }

  void Camera::farPlane( float farPlane_ )
  {
    _setFarPlane( farPlane_ );
  }

  float Camera::fieldOfView( void ) const
  {
    return _fov;
  }

  void Camera::fieldOfView( float fov_ )
  {
    _setFov( fov_ );
  }

  float* Camera::projectionMatrix( void )
  {
    if ( !_isProjMatClean )
    {
      _buildProjectionMatrix( );
      _projectionViewMatrix = _projectionMatrix * _viewMatrix;
      _isProjMatClean = true;
    }
    return _projectionMatrix.data( );
  }

  float* Camera::viewMatrix( void )
  {
    return _viewMatrix.data( );
  }

  float* Camera::projectionViewMatrix( void )
  {
    if ( !_isProjMatClean )
    {
      _buildProjectionMatrix( );
      _projectionViewMatrix = _projectionMatrix * _viewMatrix;
      _isProjMatClean = true;
    }
    return _projectionViewMatrix.data( );
  }

  void Camera::_setFov( float fov_ )
  {
    _fov = fov_ * HALFDEG2RAD;
    _f = 1.0f / tan( _fov );
    _isProjMatClean = false;
  }

  void Camera::_setRatio( float ratio_ )
  {
    _ratio = ratio_;
    _isProjMatClean = false;
  }

  void Camera::_setNearPlane( float nearPlane_ )
  {
    _nearPlane = nearPlane_;
    _isProjMatClean = false;
  }

  void Camera::_setFarPlane( float farPlane_ )
  {
    _farPlane = farPlane_;
    _isProjMatClean = false;
  }

  void Camera::_buildProjectionMatrix( void )
  {
    auto nf = 1.0f / ( _nearPlane - _farPlane );

    _projectionMatrix = Eigen::Matrix4f::Identity( );

    _projectionMatrix( 0, 0 ) = _f / _ratio;
    _projectionMatrix( 1, 1 ) = _f;
    _projectionMatrix( 2, 2 ) = ( _farPlane + _nearPlane ) * nf;
    _projectionMatrix( 2, 3 ) = ( 2.0f * _farPlane * _nearPlane ) * nf;
    _projectionMatrix( 3, 2 ) = -1.0f;
    _projectionMatrix( 3, 3 ) = 0.0f;
  }

  void Camera::_setEnableZeqConnChanges( bool enableZeqConnChanges_ )
  {
    _enableZeqConnChanges = enableZeqConnChanges_;
  }

#ifdef RETO_USE_LEXIS
  void Camera::_onCameraEvent( lexis::render::ConstLookOutPtr lookoutPtr_ )
  {
    if ( this->_enableZeqConnChanges )
    {
      const auto& aux = lookoutPtr_->getMatrixVector( );
      std::vector< float > viewMatrixVec( aux.begin( ), aux.end( ));
      _viewMatrix =
          Eigen::Map< Eigen::Matrix< float, 4, 4 > >( viewMatrixVec.data( ));
      _projectionViewMatrix = _projectionMatrix * _viewMatrix;
    }
  }
#endif

#ifdef RETO_USE_ZEROEQ
  void Camera::initializeZeroEQ(const std::string &session,
                                std::shared_ptr<zeroeq::Subscriber> subscriber)
  {
    deinitializeZeroEQ();

    _zeroeqSession = session.empty( ) ? zeroeq::DEFAULT_SESSION : session;
    _zeqConnection = true;

    if(_zeroeqSession.compare(zeroeq::NULL_SESSION) == 0)
    {
      if(subscriber)
      {
        _subscriber = subscriber;
      }
      else
      {
        // Can't get host or port, throw error.
        const auto message = std::string("Invalid subscriber for ZeroEQ initialization. ") + __FILE__ + ":" + std::to_string(__LINE__);
        throw std::runtime_error(message);
      }
    }
    else
    {
      _subscriber = std::make_shared<zeroeq::Subscriber>( _zeroeqSession );
    }

#ifdef RETO_USE_LEXIS
    _subscriber->subscribe(lexis::render::LookOut::ZEROBUF_TYPE_IDENTIFIER( ),
        [ & ]( const void* data, size_t size )
        { _onCameraEvent( lexis::render::LookOut::create( data, size ));});
#endif

    if(_zeroeqSession.compare(zeroeq::NULL_SESSION) != 0)
    {
      _subscriberThread = new std::thread( [&]()
                          { while( true ) _subscriber->receive( 10000 ); });
    }
  }

  void Camera::deinitializeZeroEQ()
  {
    if ( _subscriberThread )
    {
      _subscriberThread->std::thread::~thread( );
      _subscriberThread = nullptr;
    }

    if ( _subscriber )
      _subscriber = nullptr;
  }
#endif

} // end namespace reto
