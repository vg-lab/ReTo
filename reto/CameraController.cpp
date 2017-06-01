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

#include "CameraController.h"

namespace reto
{

  CameraController::CameraController( Camera* camera_,
                                      Path* path_,
                                      TProjection projection_,
                                      TCamera cameraType_,
                                      Eigen::Vector3f pivot_,
                                      float radius_,
                                      float animationDuration_,
                                      float tStep_ )
  : _camera( camera_ )
  , _path( path_ )
  , _projection( projection_ )
  , _cameraType( cameraType_ )
  , _pivot( pivot_ )
  , _radius( radius_ )
  , _animationDuration( animationDuration_ )
  , _tStep( tStep_ )
  {
    Eigen::Matrix4f viewMatrix;
    Eigen::Matrix4f projMatrix;
    Eigen::Matrix4f viewProjMatrix;

    switch( _cameraType )
    {
      case STANDARD :
      {
        viewMatrix = _lookAt( _camera->position( ),
                              _camera->lookAt( ),
                              _camera->up( ) );
      }
      break;

      case ORBITAL :
      {
        float defaultYaw = 0.0f;
        float defaultPitch = 0.0f;

        // Angles to radians.
        float yawRadians = ( defaultYaw * (float) M_PI ) / 180.0f;
        float pitchRadians = ( defaultPitch * (float) M_PI ) / 180.0f;

        Eigen::Matrix3f currentOrientation =
          _yawPitchRoll( 0.0f, yawRadians, pitchRadians );

        viewMatrix = _orbital( currentOrientation,
                               _camera->up( ) );
      }
      break;
    }

    switch( _projection )
    {
      case PERSPECTIVE :
      {
        float defaultFov = _camera->fov();

        defaultFov *= ( (float) M_PI / 360.0f );

        float f = 1.0f / tan( defaultFov );

        float ratio = _camera->width( ) / _camera->height( );

        float inverseOfNearMinusFar = 1.0f / ( _camera->nearPlane( ) - _camera->farPlane( ) );

        projMatrix
        << f / ratio, 0.0f, 0.0f, 0.0f,
           0.0f, f, 0.0f, 0.0f,
           0.0f, 0.0f, ( _camera->farPlane( ) + _camera->nearPlane( )  ) * inverseOfNearMinusFar, ( 2.0f * _camera->farPlane( ) * _camera->nearPlane( ) ) * inverseOfNearMinusFar,
           0.0f, 0.0f, -1.0f, 0.0f;
      }
      break;

      case ORTHOGRAPHIC :
      {
        float right = _camera->width( ) * 0.5f;
        float top = _camera->height( ) * 0.5f;

        projMatrix
        << 1.0f / right, 0.0f, 0.0f, 0.0f,
           0.0f, 1.0f / top, 0.0f, 0.0f,
           0.0f, 0.0f, -2.0f / ( _camera->farPlane( ) - _camera->nearPlane( ) ), -1.0f * ( _camera->farPlane( ) + _camera->nearPlane( ) ) / ( _camera->farPlane( ) - _camera->nearPlane( ) ),
           0.0f, 0.0f, 0.0f, 1.0f;
      }
      break;
    }

    _camera->viewMatrix( viewMatrix );
    _camera->projMatrix( projMatrix );

    viewProjMatrix = projMatrix * viewMatrix;

    _camera->viewProjMatrix( viewProjMatrix );

    // Animation attributes.
    _isAniming = false;
    _animationFirstStep = false;
    _animationSpeed = 0.1f;
    _animationPreviousTime = std::chrono::system_clock::now( );
  }

  CameraController::~CameraController( void )
  {
    delete _camera;
    delete _path;
  }

  Camera* CameraController::camera( void ) const
  {
    return _camera;
  }

  void CameraController::camera( Camera* camera_ )
  {
    _camera = camera_;
  }

  Path* CameraController::path( void ) const
  {
    return _path;
  }

  void CameraController::path( Path* path_ )
  {
    _path = path_;
  }

  CameraController::TProjection CameraController::projection( void ) const
  {
    return _projection;
  }

  CameraController::TCamera CameraController::cameraType( void ) const
  {
    return _cameraType;
  }

  Eigen::Vector3f CameraController::pivot( void ) const
  {
    return _pivot;
  }

  void CameraController::pivot( const Eigen::Vector3f& pivot_ )
  {
    _pivot = pivot_;
  }

  float CameraController::radius( void ) const
  {
    return _radius;
  }

  void CameraController::radius( float radius_ )
  {
    _radius = radius_;
  }

  float CameraController::animationDuration( void ) const
  {
    return _animationDuration;
  }

  void CameraController::animationDuration( float animationDuration_ )
  {
    _animationDuration = animationDuration_;
  }

  float CameraController::tStep( void ) const
  {
    return _tStep;
  }

  void CameraController::tStep( float tStep_ )
  {
    _tStep = tStep_;
  }

  void CameraController::center( const Eigen::Vector3f& position_,
                                 const Eigen::Vector3f& up_,
                                 const Eigen::Vector3f& lookAt_,
                                 const Eigen::Vector3f& pivot_,
                                 float radius_, float yaw_, float pitch_ )
  {

    Eigen::Matrix4f newViewMatrix = Eigen::Matrix4f::Identity( );

    switch( _cameraType )
    {
      case STANDARD :
      {
        newViewMatrix = _lookAt( position_, lookAt_, up_ );
      }
      break;

      case ORBITAL :
      {
        pivot( pivot_ );
        radius( radius_ );

        // Angles to radians.
        float yawRadians = ( yaw_ * (float) M_PI ) / 180.0f;
        float pitchRadians = ( pitch_ * (float) M_PI ) / 180.0f;

        Eigen::Matrix3f currentOrientation =
          _yawPitchRoll( 0.0f, yawRadians, pitchRadians );

        newViewMatrix = _orbital( currentOrientation,
                                  up_ );
      }
      break;
    }

    _camera->viewMatrix( newViewMatrix );

    Eigen::Matrix4f newViewProjMatrix = _camera->projMatrix( ) * newViewMatrix;

    _camera->viewProjMatrix( newViewProjMatrix );
  }

  void CameraController::translate( const Eigen::Vector3f& increment_ )
  {
    Eigen::Matrix4f newViewMatrix = _camera->viewMatrix( );

    switch( _cameraType )
    {
      case STANDARD :
      {
        Eigen::Vector3f currentPosition = _camera->position( );

        Eigen::Vector3f newPosition = currentPosition + increment_;

        newViewMatrix = _lookAt( newPosition, _camera->lookAt( ), _camera->up( ) );
      }
      break;

      case ORBITAL :
      {
        std::cerr << "Not implemented yet." << std::endl;
      }
      break;
    }

    _camera->viewMatrix( newViewMatrix );

    Eigen::Matrix4f newViewProjMatrix = _camera->projMatrix( ) * newViewMatrix;

    _camera->viewProjMatrix( newViewProjMatrix );
  }

  void CameraController::translateInLookAtVectorDirection( float increment_ )
  {
    Eigen::Matrix4f newViewMatrix = _camera->viewMatrix( );

    Eigen::Vector3f currentPosition = _camera->position( );
    Eigen::Vector3f currentLookAt = _camera->lookAt( );

    Eigen::Vector3f newPosition = currentPosition + increment_ * currentLookAt;

    switch( _cameraType )
    {
      case STANDARD :
      {
        newViewMatrix = _lookAt( newPosition, currentLookAt, _camera->up( ) );
      }
      break;

      case ORBITAL :
      {
        Eigen::Matrix3f currentOrientation =
          _camera->viewMatrix( ).block( 0, 0, 3, 3 );

        float newRadius = newPosition.norm( );

        // Ensure that radius has a positive minimum.
        if( newRadius < 10.0f ) newRadius = 10.0f;

        radius( newRadius );

        newViewMatrix = _orbital( currentOrientation,
                                  _camera->up( ) );
      }
      break;
    }

    _camera->viewMatrix( newViewMatrix );

    Eigen::Matrix4f newViewProjMatrix = _camera->projMatrix( ) * newViewMatrix;

    _camera->viewProjMatrix( newViewProjMatrix );
  }

  void CameraController::translateInUpVectorDirection( float increment_ )
  {
    Eigen::Matrix4f newViewMatrix = _camera->viewMatrix( );

    Eigen::Vector3f currentPosition = _camera->position( );
    Eigen::Vector3f currentUp = _camera->up( );

    Eigen::Vector3f newPosition = currentPosition + increment_ * currentUp;

    switch( _cameraType )
    {
      case STANDARD :
      {
        newViewMatrix = _lookAt( newPosition, _camera->lookAt( ), currentUp );
      }
      break;

      case ORBITAL :
      {
        std::cerr << "Not implemented yet." << std::endl;
      }
      break;
    }

    _camera->viewMatrix( newViewMatrix );

    Eigen::Matrix4f newViewProjMatrix = _camera->projMatrix( ) * newViewMatrix;

    _camera->viewProjMatrix( newViewProjMatrix );
  }

  void CameraController::translateInRightVectorDirection( float increment_ )
  {
    Eigen::Matrix4f newViewMatrix = _camera->viewMatrix( );

    Eigen::Vector3f currentPosition = _camera->position( );
    Eigen::Vector3f currentLookAt = _camera->lookAt( );
    Eigen::Vector3f currentUp = _camera->up( );
    Eigen::Vector3f currentRight = currentLookAt.cross( currentUp );
    currentRight.normalize( );

    Eigen::Vector3f newPosition = currentPosition + increment_ * currentRight;

    switch( _cameraType )
    {
      case STANDARD :
      {
        newViewMatrix = _lookAt( newPosition, currentLookAt, currentUp );
      }
      break;

      case ORBITAL :
      {
        std::cerr << "Not implemented yet." << std::endl;
      }
      break;
    }

    _camera->viewMatrix( newViewMatrix );

    Eigen::Matrix4f newViewProjMatrix = _camera->projMatrix( ) * newViewMatrix;

    _camera->viewProjMatrix( newViewProjMatrix );
  }

  void CameraController::localOrientation( float yaw_, float pitch_ )
  {
    Eigen::Matrix4f newViewMatrix = _camera->viewMatrix( );

    // Angles to radians.
    float yawRadians = ( yaw_ * (float) M_PI ) / 180.0f;
    float pitchRadians = ( pitch_ * (float) M_PI ) / 180.0f;

    switch( _cameraType )
    {
      case STANDARD :
      {
        // Calculating new look-at vector directly.
        Eigen::Vector3f newLookAt;
        newLookAt( 0 ) = cos( pitchRadians ) * cos( yawRadians );
        newLookAt( 1 ) = sin( pitchRadians );
        newLookAt( 2 ) = cos( pitchRadians ) * sin( yawRadians );
        newLookAt.normalize( );

        newViewMatrix = _lookAt( _camera->position( ), newLookAt, _camera->up( ) );
      }
      break;

      case ORBITAL :
      {
        Eigen::Matrix3f currentOrientation =
          _yawPitchRoll( 0.0f, yawRadians, pitchRadians );

        newViewMatrix = _orbital( currentOrientation,
                                  _camera->up( ) );
      }
      break;
    }

    _camera->viewMatrix( newViewMatrix );

    Eigen::Matrix4f newViewProjMatrix = _camera->projMatrix( ) * newViewMatrix;

    _camera->viewProjMatrix( newViewProjMatrix );
  }

  void CameraController::localRotation( float yaw_, float pitch_ )
  {
    Eigen::Matrix4f newViewMatrix = _camera->viewMatrix( );

    // Angles to radians.
    float yawRadians = ( yaw_ * (float) M_PI ) / 180.0f;
    float pitchRadians = ( pitch_ * (float) M_PI ) / 180.0f;

    switch( _cameraType )
    {
      case STANDARD :
      {
        /**
        Eigen::Matrix3f rotationMatrix =
          _yawPitchRoll( 0.0f, yawRadians, pitchRadians );

        Eigen::Vector3f newLookAt =
          rotationMatrix * _camera->lookAt( );
        newLookAt.normalize( );

        newViewMatrix = _lookAt( _camera->position( ), newLookAt, _camera->up( ) );
        **/
        std::cerr << "Not implemented yet." << std::endl;
      }
      break;

      case ORBITAL :
      {
        Eigen::Matrix3f rotationMatrix =
          _yawPitchRoll( 0.0f, yawRadians, pitchRadians );

        Eigen::Matrix3f currentOrientation =
          _camera->viewMatrix( ).block( 0, 0, 3, 3 );

        // This is key for orbital camera to work.
        currentOrientation = rotationMatrix * currentOrientation;

        newViewMatrix = _orbital( currentOrientation,
                                  _camera->up( ) );
      }
      break;
    }

    _camera->viewMatrix( newViewMatrix );

    Eigen::Matrix4f newViewProjMatrix = _camera->projMatrix( ) * newViewMatrix;

    _camera->viewProjMatrix( newViewProjMatrix );
  }

  void CameraController::zoom( float increment_ )
  {

    Eigen::Matrix4f newProjMatrix = _camera->projMatrix( );

    float scale = 1.0f;
    scale += increment_;

    switch( _projection )
    {
      case PERSPECTIVE :
      {
        float currentRatio = (float) _camera->width( ) / (float) _camera->height( );

        float currentFov = _camera->fov( );
        float newFov = currentFov * scale;
        _camera->fov( newFov );
        newFov *= ( (float) M_PI / 360.0f );
        float newF = 1.0f / tan( newFov );

        newProjMatrix( 0, 0 ) = newF / currentRatio;
        newProjMatrix( 1, 1 ) = newF;
      }
      break;

      case ORTHOGRAPHIC :
      {
        float currentWidth = _camera->width( );
        float currentHeight = _camera->height( );

        float newWidth = currentWidth * scale;
        float newHeight = currentHeight * scale;

        _camera->width( newWidth );
        _camera->height( newHeight );

        float newRight = newWidth * 0.5f;
        float newTop = newHeight * 0.5f;

        newProjMatrix( 0, 0 ) = 1.0f / newRight;
        newProjMatrix( 1, 1 ) = 1.0f / newTop;
      }
      break;
    }

    _camera->projMatrix( newProjMatrix );

    Eigen::Matrix4f newViewProjMatrix = newProjMatrix * _camera->viewMatrix( );

    _camera->viewProjMatrix( newViewProjMatrix );
  }

  void CameraController::resize( float width_, float height_ )
  {

    Eigen::Matrix4f newProjMatrix = _camera->projMatrix( );

    // Setting new width and height.
    _camera->width( width_ );
    _camera->height( height_ );

    switch( _projection )
    {
      case PERSPECTIVE :
      {
        float newRatio = width_ / height_;

        float currentFov = _camera->fov();
        currentFov *= ( (float) M_PI / 360.0f );
        float f = 1.0f / tan( currentFov );

        newProjMatrix( 0, 0 ) = f / newRatio;
      }
      break;

      case ORTHOGRAPHIC :
      {
        float newRight = width_ * 0.5f;
        float newTop = height_ * 0.5f;

        newProjMatrix( 0, 0 ) = 1.0f / newRight;
        newProjMatrix( 1, 1 ) = 1.0f / newTop;
      }
      break;
    }

    _camera->projMatrix( newProjMatrix );

    Eigen::Matrix4f newViewProjMatrix = newProjMatrix * _camera->viewMatrix( );

    _camera->viewProjMatrix( newViewProjMatrix );
  }

  bool CameraController::animate( void )
  {
    if( _path->empty( ) )
    {
      std::cerr << "A path has not been built." << std::endl;
      return false;
    }

    std::chrono::time_point< std::chrono::system_clock > currentTime =
      std::chrono::system_clock::now( );

    if ( _isAniming )
    {
      auto duration = std::chrono::duration_cast< std::chrono::milliseconds >
        ( currentTime - _animationPreviousTime );

     float dt = ( ( float ) duration.count( ) ) * 0.1f;

     Eigen::Vector3f currentPosition = _camera->position( );
     std::cout << "currentPosition: " << currentPosition << std::endl;

     Eigen::Vector3f targetPosition = _path->evaluatePosition( _currentNodeId,
                                                               _currentT );

     Eigen::Vector3f diffPosition = targetPosition - currentPosition;

     if ( _animationFirstStep )
     {
       _animationSpeed = diffPosition.norm( ) / _animationDuration;
       _animationFirstStep = false;
     }

     // Position-in-place checking.
     float distance = dt * _animationSpeed;
     bool positionInPlace = false;
     float tForOrientation = _currentT - _tStep;
     Eigen::Vector3f nextPosition = currentPosition;
     if ( ( positionInPlace = ( diffPosition.norm() <= distance ) ) )
     {
       nextPosition = targetPosition;
     }
     else
     {
       Eigen::Vector3f displacement = diffPosition.normalized() * distance;
       nextPosition = currentPosition + displacement;
       tForOrientation += _tStep * ( displacement.norm() / diffPosition.norm() );
     }

     std::cout << "nextPosition:" << nextPosition << std::endl;
     std::cout << "targetPosition: " << targetPosition << std::endl;

     Eigen::Vector3f nextLookAt = _path->evaluateLookAt( _currentNodeId,
                                                         tForOrientation );

     Eigen::Vector3f nextUp = _path->evaluateUp( _currentNodeId,
                                                 tForOrientation );

     Eigen::Matrix4f newViewMatrix =
       _lookAt( nextPosition, nextLookAt, nextUp );

     // Updating matrices.
     _camera->viewMatrix( newViewMatrix );
     Eigen::Matrix4f newViewProjMatrix = _camera->projMatrix( ) * newViewMatrix;
     _camera->viewProjMatrix( newViewProjMatrix );

     // For a strange reason ( x == 1.0f ) does not work.
     bool finishedT = ( _currentT >= 1.0f );
     bool finishedNodes = ( _currentNodeId >=  _path->nodesSize() - 1 );
     //bool finishCondition = ( _currentTargetPositionId == _splineTargetPositions.size()-1 );

     // Path state.
     /**
     std::cout << "currentT: " << _currentT << std::endl;
     std::cout << "T: " << finishedT << std::endl;
     std::cout << "currentNode: " << _currentNodeId << std::endl;
     std::cout << "NODE: " << finishedNodes << std::endl;
     std::cout << "INPLACE: " << positionInPlace << std::endl;
     std::cout << std::endl;
     **/

     _isAniming = !( /*positionInPlace &&*/ finishedT && finishedNodes );

     // Spline state.
     /**/
     std::cout << "Animating camera: " << _isAniming << std::endl;
     std::cout << std::endl;
     /**/

     if( finishedT )
     {
       _currentT = 0.0f;
       _currentNodeId += 1;
     }
     else
     {
       /*if( positionInPlace )*/ _currentT += _tStep;
     }

     _animationPreviousTime = currentTime;
     return true;
    }

    _animationPreviousTime = currentTime;
    return false;
  }

  void CameraController::triggerAnimation( void )
  {
    _isAniming = true;
    _animationFirstStep = true;
    _currentT = _tStep;
    _currentNodeId = 0;
  }

  Eigen::Matrix4f CameraController::_lookAt( const Eigen::Vector3f& position_,
                                             const Eigen::Vector3f& lookAt_,
                                             const Eigen::Vector3f& up_ )
  {
    /**
     * Updating vectors.
     */
    Eigen::Vector3f eyeToCenter = lookAt_.normalized( );
    // Up vector orthogonal with look-at vector.
    Eigen::Vector3f upVector = up_ - up_.dot( eyeToCenter ) * eyeToCenter;
    upVector.normalize( );
    Eigen::Vector3f rightVector = eyeToCenter.cross( upVector );
    rightVector.normalize( );

    // Setting vectors.
    _camera->position( position_ );
    _camera->lookAt( eyeToCenter );
    _camera->up( upVector );

    // Feedback.
    /**
    std::cout << "position: ("
              << position_.x( ) << ", "
              << position_.y( ) << ", "
              << position_.z( ) << ")" << std::endl;
    std::cout << "lookAt: ("
              << eyeToCenter.x( ) << ", "
              << eyeToCenter.y( ) << ", "
              << eyeToCenter.z( ) << ")" << std::endl;
    std::cout << "up: ("
              << upVector.x( ) << ", "
              << upVector.y( ) << ", "
              << upVector.z( ) << ")" << std::endl;
    std::cout << std::endl;
    **/

    // Relative position.
    float x = -rightVector.dot( position_ );
    float y = -upVector.dot( position_ );
    float z = eyeToCenter.dot( position_ );

    Eigen::Matrix4f toReturn;
    toReturn << rightVector.x( ), rightVector.y( ), rightVector.z( ), x,
                upVector.x( ), upVector.y( ), upVector.z( ), y,
                -eyeToCenter.x( ), -eyeToCenter.y( ), -eyeToCenter.z( ), z,
                0.0f, 0.0f, 0.0f, 1.0f;

    return toReturn;
  }

  Eigen::Matrix4f CameraController::_orbital( const Eigen::Matrix3f& orientation_,
                                              const Eigen::Vector3f& up_ )
  {
    Eigen::Vector3f newPosition = _pivot +
      orientation_.transpose( ) * Eigen::Vector3f( 0.0f, 0.0f, 1.0f ) * _radius;
    Eigen::Vector3f newLookAt = _pivot - newPosition;
    // Up vector will be orthogonalized in _lookAt function.

    // Feedback.
    /**
    std::cout << "pivot: ("
              << _pivot.x( ) << ", "
              << _pivot.y( ) << ", "
              << _pivot.z( ) << ")" << std::endl;
    std::cout << "radius: " << _radius << std::endl;
    **/

    Eigen::Matrix4f toReturn = _lookAt( newPosition,
                                        newLookAt,
                                        up_ );

    return toReturn;
  }

  Eigen::Matrix3f CameraController::_yawPitchRoll( float yaw_, float pitch_, float roll_ )
  {
    /**
     * Reference: http://planning.cs.uiuc.edu/node102.html.
     */

    Eigen::Matrix3f rRoll; // Rotation about the x-axis.
    Eigen::Matrix3f rPitch; // Rotation about the y-axis.
    Eigen::Matrix3f rYaw; // Rotation about the z-axis.

    rRoll << 1.0f, 0.0f, 0.0f,
             0.0f, cos( roll_ ), -sin( roll_ ),
             0.0f, sin( roll_ ), cos( roll_ );

    rPitch << cos( pitch_ ), 0.0f, sin( pitch_ ),
              0.0f, 1.0f, 0.0f,
              -sin( pitch_ ), 0.0f, cos( pitch_ );

    rYaw << cos( yaw_ ), -sin( yaw_ ), 0.0f,
            sin( yaw_ ), cos( yaw_ ), 0.0f,
            0.0f, 0.0f, 1.0f;

    // Order should not be changed.
    Eigen::Matrix3f rot = rYaw * rPitch * rRoll;

    return rot;
  }

} // end namespace reto
