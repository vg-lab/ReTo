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

  CameraController::CameraController( TProjection projection_,
                                      TCamera cameraType_,
                                      Path* path_,
                                      float animationDuration_,
                                      float tStep_ )
  {
    // Creating camera with default values.
    _camera = new Camera( );
    _projection = projection_;
    _cameraType = cameraType_;

    Eigen::Vector3f defaultPosition = _camera->position( );
    Eigen::Vector3f defaultUp = _camera->up( );
    Eigen::Vector3f defaultLookAt = _camera->lookAt( );

    float defaultNearPlane = _camera->nearPlane( );
    float defaultFarPlane = _camera->farPlane( );

    Eigen::Matrix4f viewMatrix;
    Eigen::Matrix4f projMatrix;
    Eigen::Matrix4f viewProjMatrix;

    switch( _cameraType )
    {
      case STANDARD :
      {
        viewMatrix = _lookAt( defaultPosition, defaultLookAt, defaultUp );
        //std::cout << "firstViewMatrix: " << viewMatrix << std::endl;
      }
      break;

      case ORBITAL :
      {
        viewMatrix = Eigen::Matrix4f::Identity( );

        std::cerr << "Not implemented yet" << std::endl;
      }
      break;
    }

    switch( _projection )
    {
      case PERSPECTIVE :
      {
        float defaultFov = _camera->fov();

        defaultFov *= ( M_PI / 360.0f );

        float f = 1.0f / tan( defaultFov );

        float ratio = _camera->width( ) / _camera->height( );

        float inverseOfNearMinusFar = 1.0f / ( defaultNearPlane - defaultFarPlane );

        projMatrix
        << f / ratio, 0.0f, 0.0f, 0.0f,
           0.0f, f, 0.0f, 0.0f,
           0.0f, 0.0f, ( defaultFarPlane + defaultNearPlane  ) * inverseOfNearMinusFar, ( 2.0f * defaultFarPlane * defaultNearPlane ) * inverseOfNearMinusFar,
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
           0.0f, 0.0f, -2.0f / ( defaultFarPlane - defaultNearPlane ), -1.0f * ( defaultFarPlane + defaultNearPlane ) / ( defaultFarPlane - defaultNearPlane ),
           0.0f, 0.0f, 0.0f, 1.0f;
      }
      break;
    }

    _camera->viewMatrix( viewMatrix );
    _camera->projMatrix( projMatrix );

    viewProjMatrix = projMatrix * viewMatrix;

    _camera->viewProjMatrix( viewProjMatrix );

    // Assigning path.
    _path = path_;

    // Animation attributes.
    _isAniming = false;
    _animationFirstStep = false;
    _animationSpeed = 0.1f;
    _animationDuration = animationDuration_;
    _animationPreviousTime = std::chrono::system_clock::now( );
    _tStep = tStep_;
  }

  CameraController::~CameraController( void )
  {
    delete _camera;
    delete _path;
  }

  Path* CameraController::path( void )
  {
    return _path;
  }

  void CameraController::path( Path* path_ )
  {
    _path = path_;
  }

  void CameraController::center( Eigen::Vector3f position_,
                                 Eigen::Vector3f up_,
                                 Eigen::Vector3f lookAt_ )
  {

    Eigen::Matrix4f newViewMatrix = Eigen::Matrix4f::Identity( );

    switch( _cameraType )
    {
      case STANDARD :
      {
        newViewMatrix = _lookAt( position_, lookAt_, up_ );
        //std::cout << "centeredViewMatrix: " << newViewMatrix << std::endl;
      }
      break;

      case ORBITAL :
      {
        newViewMatrix = Eigen::Matrix4f::Identity( );

        std::cerr << "Not implemented yet" << std::endl;
      }
      break;
    }

    _camera->viewMatrix( newViewMatrix );

    Eigen::Matrix4f newViewProjMatrix = _camera->projMatrix( ) * newViewMatrix;

    _camera->viewProjMatrix( newViewProjMatrix );
  }

  void CameraController::zoom( float increment_ )
  {

    Eigen::Matrix4f newProjMatrix = Eigen::Matrix4f::Identity( );

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
        newFov *= ( M_PI / 360.0f );
        float newF = 1.0f / tan( newFov );

        newProjMatrix = _camera->projMatrix( );
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

        newProjMatrix = _camera->projMatrix( );
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

    Eigen::Matrix4f newProjMatrix = Eigen::Matrix4f::Identity( );

    // Setting new width and height.
    _camera->width( width_ );
    _camera->height( height_ );

    switch( _projection )
    {
      case PERSPECTIVE :
      {
        float newRatio = width_ / height_;

        float currentFov = _camera->fov();
        currentFov *= ( M_PI / 360.0f );
        float f = 1.0f / tan( currentFov );

        newProjMatrix = _camera->projMatrix( );
        newProjMatrix( 0, 0 ) = f / newRatio;
      }
      break;

      case ORTHOGRAPHIC :
      {
        float newRight = width_ * 0.5f;
        float newTop = height_ * 0.5f;

        newProjMatrix = _camera->projMatrix( );
        newProjMatrix( 0, 0 ) = 1.0f / newRight;
        newProjMatrix( 1, 1 ) = 1.0f / newTop;
      }
      break;
    }

    _camera->projMatrix( newProjMatrix );

    Eigen::Matrix4f newViewProjMatrix = newProjMatrix * _camera->viewMatrix( );

    _camera->viewProjMatrix( newViewProjMatrix );
  }

  void CameraController::translate( Eigen::Vector3f increment_ )
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
        newViewMatrix = Eigen::Matrix4f::Identity( );

        std::cerr << "Not implemented yet" << std::endl;
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

    switch( _cameraType )
    {
      case STANDARD :
      {

        Eigen::Vector3f currentPosition = _camera->position( );
        Eigen::Vector3f currentLookAt = _camera->lookAt( );

        Eigen::Vector3f newPosition = currentPosition + increment_ * currentLookAt;

        newViewMatrix = _lookAt( newPosition, currentLookAt, _camera->up( ) );
      }
      break;

      case ORBITAL :
      {
        newViewMatrix = Eigen::Matrix4f::Identity( );

        std::cerr << "Not implemented yet" << std::endl;
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

    switch( _cameraType )
    {
      case STANDARD :
      {

        Eigen::Vector3f currentPosition = _camera->position( );
        Eigen::Vector3f currentUp = _camera->up( );

        Eigen::Vector3f newPosition = currentPosition + increment_ * currentUp;

        newViewMatrix = _lookAt( newPosition, _camera->lookAt( ), currentUp );
      }
      break;

      case ORBITAL :
      {
        newViewMatrix = Eigen::Matrix4f::Identity( );

        std::cerr << "Not implemented yet" << std::endl;
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

    switch( _cameraType )
    {
      case STANDARD :
      {
        Eigen::Vector3f currentPosition = _camera->position( );
        Eigen::Vector3f currentLookAt = _camera->lookAt( );
        Eigen::Vector3f currentUp = _camera->up( );
        Eigen::Vector3f currentRight = currentLookAt.cross( currentUp );
        currentRight.normalize( );

        Eigen::Vector3f newPosition = currentPosition + increment_ * currentRight;

        newViewMatrix = _lookAt( newPosition, currentLookAt, currentUp );
      }
      break;

      case ORBITAL :
      {
        newViewMatrix = Eigen::Matrix4f::Identity( );

        std::cerr << "Not implemented yet" << std::endl;
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

    switch( _cameraType )
    {
      case STANDARD :
      {
        // Angles to radians.
        float yawRadians = ( yaw_ * M_PI ) / 180.0f;
        float pitchRadians = ( pitch_ * M_PI ) / 180.0f;

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
        newViewMatrix = Eigen::Matrix4f::Identity( );

        std::cerr << "Not implemented yet" << std::endl;
      }
      break;
    }

    _camera->viewMatrix( newViewMatrix );

    Eigen::Matrix4f newViewProjMatrix = _camera->projMatrix( ) * newViewMatrix;

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

  Eigen::Matrix4f CameraController::_lookAt( Eigen::Vector3f position_,
                                             Eigen::Vector3f lookAt_,
                                             Eigen::Vector3f up_ )
  {
    // Updating vectors.
    Eigen::Vector3f eyeToCenter = lookAt_.normalized( );
    Eigen::Vector3f upVector = up_ - up_.dot( eyeToCenter ) * eyeToCenter;
    upVector.normalize( );
    Eigen::Vector3f rightVector = eyeToCenter.cross( upVector );
    rightVector.normalize( );

    // Setting vectors.
    _camera->position( position_ );
    //std::cout << "position:" << position_ << std::endl;
    _camera->lookAt( eyeToCenter );
    std::cout << "lookAt:" << eyeToCenter << std::endl;
    _camera->up( upVector );
    std::cout << "up:" << upVector << std::endl;
    std::cout << std::endl;

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

  Eigen::Matrix3f CameraController::_generateRotationMatrix( float yaw_, float pitch_ )
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

} // end namespace reto
