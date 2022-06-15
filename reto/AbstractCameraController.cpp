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

#include "AbstractCameraController.h"

template<class T> void ignore( const T& ) { }

namespace reto
{
  AbstractCameraController::AbstractCameraController( Camera* camera_, const std::string session
#ifdef RETO_USE_ZEROEQ
      , std::shared_ptr<zeroeq::Subscriber> subscriber
#endif
      )
  : _camera( camera_ )
  , _position( 0.0f, 0.0f, 0.0f )
  , _rotation( Eigen::Matrix3f::Identity( ))
  , _radius( 1.0f )
  , _isAniming( false )
  , _currentAnimTime( 0.0f )
  , _currentAnim( nullptr )
  , _loopAnim( false )
  {
    if ( !_camera )
      _camera = new Camera( );

#ifdef RETO_USE_ZEROEQ
    const auto zeqSession = session.empty() ? zeroeq::DEFAULT_SESSION : session;
    _camera->initializeZeroEQ( zeqSession, subscriber);
#else
    ignore(session);
#endif
  }

  Camera* AbstractCameraController::camera( void )
  {
    return _camera;
  }

  void AbstractCameraController::update( void )
  {
    _conformSetViewMatrix( _position, _rotation, _radius );
  }

  void AbstractCameraController::anim( const float deltaTime_ )
  {
    if ( _isAniming && _currentAnim )
    {
      _currentAnimTime += deltaTime_;
      if ( _currentAnimTime > _currentAnim->endTime( ))
      {
        if ( _loopAnim )
          _currentAnimTime = _currentAnim->startTime( );
        else
        {
          _isAniming = false;
          _camera->_setEnableZeqConnChanges( true );
        }
      }
      auto keyCamera = _currentAnim->getKeyCamera( _currentAnimTime );
      if ( _currentAnim->isPosAnimated( ))
        _position = keyCamera->position( );
      if ( _currentAnim->isRotAnimated( ))
        _rotation = keyCamera->rotation( );
      if ( _currentAnim->isRadAnimated( ))
        _radius = keyCamera->radius( );
      _conformSetViewMatrix( _position, _rotation,
                             _radius );
    }
  }

  void AbstractCameraController::startAnim( CameraAnimation* cameraAnimation_,
                                            bool loop_ )
  {
    if ( !_isAniming && cameraAnimation_ )
    {
      _currentAnim = cameraAnimation_;
      _loopAnim = loop_;
      _isAniming = true;
      _currentAnimTime = _currentAnim->startTime( );
      _camera->_setEnableZeqConnChanges( false );
    }
  }

  void AbstractCameraController::stopAnim( void )
  {
    _isAniming = false;
    _loopAnim = false;
    _camera->_setEnableZeqConnChanges( true );
  }

  bool AbstractCameraController::isAniming( void ) const
  {
    return _isAniming;
  }

  void AbstractCameraController::position( const Eigen::Vector3f& position_ )
  {
    if ( !_isAniming )
    {
      _position = position_;
      _conformSetViewMatrix( _position, _rotation, _radius );
    }
  }

  Eigen::Vector3f AbstractCameraController::position( void ) const
  {
    return _position;
  }


  void AbstractCameraController::rotation( const Eigen::Matrix3f& rotation_ )
  {
    if ( !_isAniming )
    {
      _rotation = rotation_;
      _conformSetViewMatrix( _position, _rotation, _radius );
    }
  }

  void AbstractCameraController::rotation(
    const Eigen::Vector3f& rotationAngles_ )
  {
    _rotation = rotationMatrixFromAngles( rotationAngles_ );
    _conformSetViewMatrix( _position, _rotation, _radius );
  }

  Eigen::Matrix3f AbstractCameraController::rotation( void ) const
  {
    return _rotation;
  }

  void AbstractCameraController::radius( float radius_ )
  {
    if ( !_isAniming )
    {
      _radius = radius_;
      _conformSetViewMatrix( _position, _rotation, _radius );
    }
  }

  float AbstractCameraController::radius( void ) const
  {
    return _radius;
  }

  void AbstractCameraController::translate(
    const Eigen::Vector3f& translation_ )
  {
    position( _position + translation_ );
  }

  void AbstractCameraController::rotate( const Eigen::Matrix3f& rotation_ )
  {
    Eigen::Matrix3f rot = rotation_ * _rotation;
    rotation( rot );
  }

  void AbstractCameraController::rotate(
    const Eigen::Vector3f& rotationAngles_ )
  {
    Eigen::Matrix3f rot = rotationMatrixFromAngles( rotationAngles_ ) *
      _rotation;
    rotation( rot );
  }

  void AbstractCameraController::windowSize( int width_, int height_ )
  {
    _camera->_setRatio(static_cast<float>(width_) / height_ );
  }

  Eigen::Matrix3f AbstractCameraController::rotationMatrixFromAngles(
    const Eigen::Vector3f& rotationAngles_ )
  {
    Eigen::Matrix3f rot;
    Eigen::Matrix3f rYaw;
    Eigen::Matrix3f rPitch;
    Eigen::Matrix3f rRoll;

    float sinYaw = sin( rotationAngles_.x( ));
    float cosYaw = cos( rotationAngles_.x( ));
    float sinPitch = sin( rotationAngles_.y( ));
    float cosPitch = cos( rotationAngles_.y( ));
    float sinRoll = sin( rotationAngles_.z( ));
    float cosRoll = cos( rotationAngles_.z( ));

    rYaw << cosYaw, 0.0f, sinYaw,
      0.0f, 1.0f, 0.0f,
      -sinYaw, 0.0f, cosYaw;

    rPitch << 1.0f, 0.0f, 0.0f,
      0.0f, cosPitch, -sinPitch,
      0.0f, sinPitch, cosPitch;

    rRoll << 1.0f, 0.0f, 0.0f,
      0.0f, cosRoll, -sinRoll,
      0.0f, sinRoll, cosRoll;

    rot = rYaw * rPitch * rRoll;
    return rot;
  }

  void AbstractCameraController::_setCameraViewMatrix(
    const Eigen::Matrix4f& viewMatrix_ )
  {
    _camera->_viewMatrix = viewMatrix_;
    _camera->_isProjMatClean = false;
  }
}
