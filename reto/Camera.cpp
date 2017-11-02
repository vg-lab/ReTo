/*
 * Copyright (c) 2014-2017 GMRV/URJC.
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

namespace reto
{

  Camera::Camera( float fov_, float ratio_, float nearPlane_, float farPlane_,
                  Eigen::Vector3f pivot_, float radius_, float yaw_,
                  float pitch_ )
    : _ratio( ratio_ )
    , _nearPlane( nearPlane_ )
    , _farPlane( farPlane_ )
    , _pivot( pivot_ )
    , _radius( radius_ )
#ifdef RETO_USE_LEXIS
    , _zeqConnection( false )
#endif
    , _isAniming( false )
    , _firstStep( false )
    , _speedPivot( 1.0f / 60.0f )
    , _speedRadius( 1.0f / 60.0f )
    , _animDuration( 2.0f )
  {
    _fov = fov_ * ( M_PI / 360.0f );
    _f = 1.0f / tan( _fov );

    _previusTime = std::chrono::system_clock::now( );

    _Rotation( _RotationFromPY( pitch_, yaw_ ));
    _BuildProjectionMatrix( );
    _BuildOrthoProjectionMatrix( );
    _BuildViewMatrix( );
    _BuildViewProjectionMatrix( );
  }

#ifdef RETO_USE_LEXIS
  Camera::Camera( const std::string& session_, float fov_, float ratio_,
                  float nearPlane_, float farPlane_, Eigen::Vector3f pivot_,
                  float radius_, float yaw_, float pitch_ )
    : _ratio( ratio_ )
    , _nearPlane( nearPlane_ )
    , _farPlane( farPlane_ )
    , _pivot( pivot_ )
    , _radius( radius_ )
    , _zeqConnection( true )
    , _isAniming( false )
    , _firstStep( false )
    , _speedPivot( 0.0f )
    , _speedRadius( 0.0f )
    , _animDuration( 2.0f )
  {
    _fov = fov_ * ( M_PI / 360.0f );
    _f = 1.0f / tan( _fov );

    _zeroeqSession = session_.empty( ) ? zeroeq::DEFAULT_SESSION : session_;

    _previusTime = std::chrono::system_clock::now( );

    _Rotation( _RotationFromPY( pitch_, yaw_ ));

    _publisher = new zeroeq::Publisher( _zeroeqSession );
    _subscriber = new zeroeq::Subscriber( _zeroeqSession );

    _subscriber->subscribe(
        lexis::render::LookOut::ZEROBUF_TYPE_IDENTIFIER( ),
        [ & ]( const void* data, size_t size )
        { _OnCameraEvent( lexis::render::LookOut::create( data, size ));});

    _subscriberThread =
        new std::thread( [&]() { while( true ) _subscriber->receive( 10000 );});

    _BuildProjectionMatrix( );
    _BuildOrthoProjectionMatrix( );
    _BuildViewMatrix( );
    _BuildViewProjectionMatrix( );
  }
#endif

  Camera::~Camera( void )
  {
  }

  // PUBLIC

  void Camera::localTranslation( Eigen::Vector3f increment_ )
  {
    _pivot += _rotation.transpose( ) * increment_;
    _BuildViewMatrix( );
    _BuildViewProjectionMatrix( );
  }

  void Camera::localRotation( const float yaw_, const float pitch_ )
  {
    _Rotation( _RotationFromPY( yaw_, pitch_) * _rotation );
    _BuildViewMatrix( );
    _BuildViewProjectionMatrix( );
  }

  bool Camera::anim( void )
  {
    std::chrono::time_point< std::chrono::system_clock > actualTime =
      std::chrono::system_clock::now( );

    if ( _isAniming )
    {
      auto duration = std::chrono::duration_cast< std::chrono::milliseconds >
        ( actualTime - _previusTime );
      float dt = (( float ) duration.count( )) * 0.001f;

      Eigen::Vector3f actualPivot = _pivot;
      Eigen::Vector3f diffPivot = _targetPivot - actualPivot;

      float actualRadius = _radius;
      float diffRadius = _targetRadius - actualRadius;

      if ( _firstStep )
      {
        _speedPivot = diffPivot.norm( ) / _animDuration;
        _speedRadius = fabs( diffRadius ) / _animDuration;
        _firstStep = false;
      }

      float distancePivot = dt * _speedPivot;
      float distanceRadius = dt * _speedRadius;

      bool pivotInPlace = false;
      bool radiusInPlace = false;

      if (( pivotInPlace = ( diffPivot.norm() <= distancePivot )))
        _pivot = _targetPivot;
      else
        _pivot = actualPivot + diffPivot.normalized() * distancePivot;

      _BuildViewMatrix( );
      _BuildViewProjectionMatrix( );

      if (( radiusInPlace = ( fabs( diffRadius ) <= distanceRadius )))
        _radius = _targetRadius;
      else
        _radius = actualRadius + diffRadius / fabs( diffRadius ) *
            distanceRadius;

      _BuildViewMatrix( );
      _BuildViewProjectionMatrix( );

      _isAniming = !( pivotInPlace && radiusInPlace );
      _previusTime = actualTime;
      return true;
    }
    _previusTime = actualTime;
    return false;
  }

  // GETTERS

  float Camera::fov( void )
  {
    return _fov;
  }

  Eigen::Vector3f Camera::pivot( void )
  {
    return _pivot;
  }

  float Camera::radius( void )
  {
    return _radius;
  }

  float* Camera::projectionMatrix( void )
  {
    return _projVec.data( );
  }

  float* Camera::orthoMatrix( void )
  {
    return _orthoVec.data( );
  }

  float* Camera::viewMatrix( void )
  {
    return _viewVec.data( );
  }

  float* Camera::viewProjectionMatrix( void )
  {
    return _viewProjVec.data( );
  }

  float* Camera::position( void )
  {
    return _positionVec.data( );
  }

#ifdef RETO_USE_LEXIS
  zeroeq::Subscriber* Camera::subscriber( void )
  {
    return _subscriber;
  }
#endif


  // SETTERS

  void Camera::ratio( const float ratio_ )
  {
    _ratio = ratio_;
    _BuildProjectionMatrix( );
    _BuildOrthoProjectionMatrix( );
    _BuildViewProjectionMatrix( );
  }

  void Camera::pivot( Eigen::Vector3f pivot_ )
  {
    if ( !_isAniming )
    {
      _pivot = pivot_;
      _BuildViewMatrix( );
      _BuildViewProjectionMatrix( );
    }
  }

  void Camera::radius( const float radius_ )
  {
    if ( !_isAniming )
    {
      _radius = radius_;
      _BuildViewMatrix( );
      _BuildViewProjectionMatrix( );
    }
  }

  void Camera::rotation( const float yaw_, const float pitch_ )
  {
    if ( !_isAniming )
    {
      _Rotation( _RotationFromPY( yaw_, pitch_ ));
      _BuildViewMatrix( );
      _BuildViewProjectionMatrix( );
    }
  }

  void Camera::targetPivot( Eigen::Vector3f targetPivot_ )
  {
    _isAniming = true;
    _firstStep = true;
    _targetPivot = targetPivot_;
  }

  void Camera::targetRadius( const float targetRadius_ )
  {
    _isAniming = true;
    _firstStep = true;
    _targetRadius = targetRadius_;
  }

  void Camera::targetPivotRadius( Eigen::Vector3f targetPivot_,
      float targetRadius_ )
  {
    targetPivot( targetPivot_ );
    targetRadius( targetRadius_ );
  }

  void Camera::animDuration( const float animDuration_ )
  {
    _animDuration = animDuration_;
  }

  void Camera::_BuildProjectionMatrix( void )
  {
    _projVec.resize( 16 );

    auto nf = 1.0f / ( _nearPlane - _farPlane );

    // row 1
    _projVec[0] = _f / _ratio;
    _projVec[1] = .0f;
    _projVec[2] = .0f;
    _projVec[3] = .0f;
    // row 2
    _projVec[4] = .0f;
    _projVec[5] = _f;
    _projVec[6] = .0f;
    _projVec[7] = .0f;
    // row 3
    _projVec[8] = .0f;
    _projVec[9] = .0f;
    _projVec[10] = ( _farPlane + _nearPlane ) * nf;
    _projVec[11] = -1.0f;
    // row 4
    _projVec[12] = .0f;
    _projVec[13] = .0f;
    _projVec[14] = ( 2.0f * _farPlane * _nearPlane ) * nf;
    _projVec[15] = .0f;
  }

  void Camera::_BuildOrthoProjectionMatrix( void )
  {
    _orthoVec.resize( 16 );

    float left = 0.0f;
    float right = this->width( ) * 2.5f;
    float top = this->height( ) * 2.5f;
    float bottom = 0.0f;
    float near = _nearPlane;
    float far = _farPlane;

    auto lr = 1.0f / (left - right),
      bt = 1.0f / (bottom - top),
      nf = 1.0f / (near - far);

    // row 1
    _orthoVec[0] = -2.0f * lr;
    _orthoVec[1] = 0.0f;
    _orthoVec[2] = 0.0f;
    _orthoVec[3] = 0.0f;
    // row 2
    _orthoVec[4] = 0.0f;
    _orthoVec[5] = -2.0f * bt;
    _orthoVec[6] = 0.0f;
    _orthoVec[7] = 0.0f;
    // row 3
    _orthoVec[8] = 0.0f;
    _orthoVec[9] = 0.0f;
    _orthoVec[10] = 2.0f * nf;
    _orthoVec[11] = 0.0f;
    // row 4
    _orthoVec[12] = (left + right) * lr;
    _orthoVec[13] = (top + bottom) * bt;
    _orthoVec[14] = (far + near) * nf;
    _orthoVec[15] = 1.0f;
  }

  void Camera::_BuildViewMatrix( void )
  {
    Eigen::Vector3f pos = _rotation.transpose( ) *
    Eigen::Vector3f( 0.0f, 0.0f, 1.0f ) * _radius + _pivot;

    Eigen::Vector3f pv = _rotation * _pivot;

    std::vector<float> positionVec;
    positionVec.resize( 3 );
    positionVec[ 0 ] =  pos.x( );
    positionVec[ 1 ] =  pos.y( );
    positionVec[ 2 ] =  pos.z( );

    _PositionVectorized( positionVec );

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
    viewVec[12] = - pv.x( );
    viewVec[13] = - pv.y( );
    viewVec[14] = - pv.z( ) - _radius;
    viewVec[15] = 1.0f;

    _ViewMatrixVectorized( viewVec );

#ifdef RETO_USE_LEXIS
    if ( _zeqConnection )
    {
      std::vector< double > viewm ( viewVec.begin( ), viewVec.end( ));

      lexis::render::LookOut lookout;
      lookout.setMatrix( viewm );
      _publisher->publish( lookout );
    }
#endif

  }

  void Camera::_BuildViewProjectionMatrix( void )
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

#ifdef RETO_USE_LEXIS
  void Camera::_OnCameraEvent( lexis::render::ConstLookOutPtr lookoutPtr_ )
  {
    const auto& aux = lookoutPtr_->getMatrixVector( );
    std::vector<float> viewMatrixVec( aux.begin( ), aux.end( ));
    _ViewMatrixVectorized( viewMatrixVec );

    Eigen::Matrix3f rot;
    rot << viewMatrixVec[0], viewMatrixVec[4], viewMatrixVec[8],
           viewMatrixVec[1], viewMatrixVec[5], viewMatrixVec[9],
           viewMatrixVec[2], viewMatrixVec[6], viewMatrixVec[10];

    Eigen::Vector3f pos = - rot.inverse() * Eigen::Vector3f( viewMatrixVec[12],
                          viewMatrixVec[13], viewMatrixVec[14] );

    std::vector< float > posVec( 3 );
    posVec[ 0 ] = pos.x( );
    posVec[ 1 ] = pos.y( );
    posVec[ 2 ] = pos.z( );

    _PositionVectorized( posVec );
  }

#endif

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

    // PRIVATE

#ifndef RETO_USE_LEXIS

  void Camera::_PositionVectorized( const std::vector<float>& positionVec_ )
  {
    _positionVec = positionVec_;
  }

  void Camera::_Rotation( const Eigen::Matrix3f& rotation_ )
  {
    _rotation = rotation_;
  }

  void Camera::_ViewMatrixVectorized( const std::vector<float>& viewVec_ )
  {
    _viewVec = viewVec_;
  }

#else

  void Camera::_PositionVectorized( const std::vector<float>& positionVec_ )
  {
    _positionMutex.lock( );
    _positionVec = positionVec_;
    _positionMutex.unlock( );
  }

  void Camera::_Rotation( const Eigen::Matrix3f& rotation_ )
  {
    _rotationMutex.lock( );
    _rotation = rotation_;
    _rotationMutex.unlock( );
  }

  void Camera::_ViewMatrixVectorized( const std::vector<float>& viewVec_ )
  {
    _viewMatrixMutex.lock( );
    _viewVec = viewVec_;
    _viewMatrixMutex.unlock( );
  }

#endif
  void Camera::setWindowSize( const int width_, const int height_ )
  {
    this->_width = width_;
    this->_height = height_;
  }

  int Camera::width( void )
  {
    return this->_width;
  }

  int Camera::height( void )
  {
    return this->_height;
  }


} // end namespace reto
