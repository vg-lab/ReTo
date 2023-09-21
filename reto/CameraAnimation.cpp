/*
 * Copyright (c) 2014-2019 VG-Lab/URJC.
 *
 * Authors: Juan Jose Garcia Cantero <juanjose.garcia@urjc.es>
 *          Cristian Rodríguez Bernal <cristian.rodriguez@urjc.es>
 *
 * This file is part of ReTo <https://gitlab.vg-lab.es/nsviz/ReTo>
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

#include "CameraAnimation.h"

#include <iostream>


namespace reto
{

  KeyCamera::KeyCamera( float time_, const Eigen::Vector3f& position_,
                        const Eigen::Matrix3f& rotation_, float radius_ )
    : _time( time_ )
    , _position( position_ )
    , _rotation( rotation_ )
    , _radius( radius_ )
  {

  }

  KeyCamera::KeyCamera( float time_, const Eigen::Vector3f& position_,
                        const Eigen::Vector3f& rotationAngles_, float radius_ )
    : _time( time_ )
    , _position( position_ )
    , _rotation( _rotMatrixFromAngles( rotationAngles_ ))
    , _radius( radius_ )
  {

  }

  KeyCamera::~KeyCamera( void )
  {

  }

  float KeyCamera::time( void ) const
  {
    return _time;
  }

  Eigen::Vector3f KeyCamera::position( void ) const
  {
    return _position;
  }

  Eigen::Matrix3f KeyCamera::rotation( void ) const
  {
    return _rotation;
  }

  float KeyCamera::radius( void ) const
  {
    return _radius;
  }

  Eigen::Matrix3f KeyCamera::_rotMatrixFromAngles(
    const Eigen::Vector3f& rotationAngles_ )
  {
    Eigen::Matrix3f rot;
    Eigen::Matrix3f rYaw;
    Eigen::Matrix3f rPitch;
    Eigen::Matrix3f rRoll;

    float sinYaw, cosYaw, sinPitch, cosPitch, sinRoll, cosRoll;

    sinYaw = sin( rotationAngles_.x( ));
    cosYaw = cos( rotationAngles_.x( ));
    sinPitch = sin( rotationAngles_.y( ));
    cosPitch = cos( rotationAngles_.y( ));
    sinRoll = sin( rotationAngles_.z( ));
    cosRoll = cos( rotationAngles_.z( ));

    rYaw << cosYaw, 0.0f, sinYaw,
      0.0f, 1.0f, 0.0f,
      -sinYaw, 0.0f, cosYaw;

    rPitch << 1.0f, 0.0f, 0.0f,
      0.0f, cosPitch, -sinPitch,
      0.0f, sinPitch, cosPitch;

    rRoll << 1.0f, 0.0f, 0.0f,
      0.0f, cosRoll, -sinRoll,
      0.0f, sinRoll, cosRoll;

    rot = rRoll * rPitch * rYaw;
    return rot;
  }

  CameraAnimation::CameraAnimation( TInterpolation posInterpolation_,
                                    TInterpolation rotInterpolation_,
                                    TInterpolation radInterpolation_ )
    : _startTime( std::numeric_limits< float >::max( ))
    , _endTime( std::numeric_limits< float >::min( ))
  {
    switch( posInterpolation_ )
    {
    case LINEAR:
      _positionInterFunc = CameraAnimation::_linear;
      break;
    default:
      _positionInterFunc = nullptr;
    }

    switch( rotInterpolation_ )
    {
    case LINEAR:
      _rotationInterFunc = CameraAnimation::_linear;
      break;
    default:
      _rotationInterFunc = nullptr;
    }
    switch( radInterpolation_ )
    {
    case LINEAR:
      _radiusInterFunc = CameraAnimation::_linear;
      break;
    default:
      _radiusInterFunc = nullptr;
    }
  }

  CameraAnimation::~CameraAnimation( void )
  {
    for ( auto cell: _animation )
      delete cell.second;
    _animation.clear( );
  }

  KeyCamera* CameraAnimation::getKeyCamera( float currentTime_ )
  {
    if ( _animation.empty( ))
      return nullptr;
    auto upper_bound = _animation.upper_bound( currentTime_ );
    if ( upper_bound == _animation.end( ))
    {
      upper_bound--;
      return upper_bound->second;
    }
    else if ( upper_bound != _animation.begin( ))
    {
      auto lower_bound = upper_bound;
      lower_bound--;
      float preTime = lower_bound->first;
      KeyCamera* preKey = lower_bound->second;
      float postTime = upper_bound->first;
      KeyCamera* postKey = upper_bound->second;

      float alpha = ( currentTime_ - preTime ) / ( postTime - preTime );
      auto position = preKey->position( );
      if ( _positionInterFunc )
        position = _positionInterFunc( preKey->position( ),
                                       postKey->position( ), alpha );
      auto rotation = preKey->rotation( );
      if ( _rotationInterFunc )
        rotation = _rotationInterFunc( preKey->rotation( ),
                                       postKey->rotation( ), alpha );
      auto radius = preKey->radius( );
      if ( _radiusInterFunc )
        radius = _radiusInterFunc( preKey->radius( ),
                                       postKey->radius( ), alpha );
      return new KeyCamera( currentTime_, position, rotation, radius );
    }
    return _animation.begin( )->second;
  }

  bool CameraAnimation::addKeyCamera( KeyCamera* keyCamera_ )
  {
    if ( !keyCamera_ )
      return false;
    float time = keyCamera_->time( );
    std::pair< std::map< float , KeyCamera* >::iterator, bool > ret;
    ret = _animation.insert( std::pair< float, KeyCamera* >(
                               time, keyCamera_ ));
    if ( ret.second )
    {
      _startTime = std::min( _startTime, time );
      _endTime = std::max( _endTime, time );
    }
    return ret.second;
  }

  float CameraAnimation::startTime( void ) const
  {
    return _startTime;
  }

  float CameraAnimation::endTime( void ) const
  {
    return _endTime;
  }

  bool CameraAnimation::isPosAnimated( void ) const
  {
    return _positionInterFunc;
  }

  bool CameraAnimation::isRotAnimated( void ) const
  {
    return _rotationInterFunc;
  }

  bool CameraAnimation::isRadAnimated( void ) const
  {
    return _radiusInterFunc;
  }


  Eigen::Vector3f CameraAnimation::_linear(
    Eigen::Vector3f start_, Eigen::Vector3f end_, float alpha_ )
  {
    return ( 1.0f - alpha_ ) * start_ + alpha_ * end_;
  }

  Eigen::Matrix3f CameraAnimation::_linear(
    Eigen::Matrix3f start_, Eigen::Matrix3f end_, float alpha_ )
  {
    auto qStart = Eigen::Quaternionf( start_ );
    auto qEnd = Eigen::Quaternionf( end_ );
    return qStart.slerp( alpha_, qEnd ).toRotationMatrix( );
  }

  float CameraAnimation::_linear(
    float start_, float end_, float alpha_ )
  {
    return ( 1.0f - alpha_ ) * start_ + alpha_ * end_;
  }
}
