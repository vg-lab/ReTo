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
                                      Path* path_ )
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
        viewMatrix = lookAt( defaultPosition, defaultLookAt, defaultUp );
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
    _animationDuration = 10.0f;
    _animationPreviousTime = std::chrono::system_clock::now( );
    _tStep = 0.05f;
  }

  CameraController::~CameraController( void )
  {
    delete _camera;
    delete _path;
  }

  void CameraController::path( Path* path_ )
  {
    _path = path_;
  }

  void CameraController::center( Eigen::Vector3f centeredPosition_ )
  {

    Eigen::Matrix4f newViewMatrix = Eigen::Matrix4f::Identity( );

    switch( _cameraType )
    {
      case STANDARD :
      {
        _camera->position( centeredPosition_ );
        _camera->up( Eigen::Vector3f( 0.0f, 1.0f, 0.0f ) );
        _camera->lookAt( Eigen::Vector3f( 0.0f, 0.0f, 1.0f ) );
        newViewMatrix = lookAt( _camera->position( ), _camera->lookAt( ), _camera->up( ) );
        std::cout << "VIEW-C: " << newViewMatrix << std::endl;
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
        float currentRatio = _camera->width( ) / _camera->height( );
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

  void CameraController::localTranslation( Eigen::Vector3f increment_ )
  {
    Eigen::Matrix4f newViewMatrix = _camera->viewMatrix( );

    switch( _cameraType )
    {
      case STANDARD :
      {
        Eigen::Vector3f currentPosition = _camera->position( );

        Eigen::Vector3f newPosition = currentPosition + increment_;

        newViewMatrix = lookAt( newPosition, _camera->lookAt( ), _camera->up( ) );
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

  void CameraController::moveUsingLookAtVector( float increment_ )
  {
    Eigen::Matrix4f newViewMatrix = _camera->viewMatrix( );

    switch( _cameraType )
    {
      case STANDARD :
      {

        Eigen::Vector3f currentPosition = _camera->position( );
        Eigen::Vector3f currentLookAt = _camera->lookAt( );

        Eigen::Vector3f newPosition = currentPosition + increment_ * currentLookAt;

        newViewMatrix = lookAt( newPosition, currentLookAt, _camera->up( ) );
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

  void CameraController::moveUsingTangentVector( float increment_ )
  {
    Eigen::Matrix4f newViewMatrix = _camera->viewMatrix( );

    switch( _cameraType )
    {
      case STANDARD :
      {
        Eigen::Vector3f currentPosition = _camera->position( );
        Eigen::Vector3f currentLookAt = _camera->lookAt( );
        Eigen::Vector3f currentUp = _camera->up( );
        Eigen::Vector3f currentTangent = currentLookAt.cross( currentUp );
        currentTangent.normalize( );

        Eigen::Vector3f newPosition = currentPosition + increment_ * currentTangent;

        newViewMatrix = lookAt( newPosition, currentLookAt, currentUp );
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

  void CameraController::localRotation( float yaw_, float pitch_ )
  {
    Eigen::Matrix4f newViewMatrix = _camera->viewMatrix( );

    switch( _cameraType )
    {
      case STANDARD :
      {
        // To radians.
        float yawRadians = ( yaw_ * M_PI ) / 180.0f;
        float pitchRadians = ( pitch_ * M_PI ) / 180.0f;

        Eigen::Vector3f newLookAt;
        newLookAt( 0 ) = cos( pitchRadians ) * cos( yawRadians );
        newLookAt( 1 ) = sin( pitchRadians );
        newLookAt( 2 ) = cos( pitchRadians ) * sin( yawRadians );
        newLookAt.normalize( );

        newViewMatrix = lookAt( _camera->position( ), newLookAt, _camera->up( ) );
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

  void CameraController::triggerAnimation( void )
  {
    _isAniming = true;
    _animationFirstStep = true;
    _currentT = _tStep;
    _currentNodeId = 0;
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

     float dt = ( ( float ) duration.count( ) ) * 0.001f;

     Eigen::Vector3f currentPosition = _camera->position( );

     Eigen::Vector3f targetPosition = _path->evaluatePosition( _currentNodeId,
                                                               _currentT );

     /**
     std::cout << "targetPosition: (" << targetPosition.x() << ", "
                                      << targetPosition.y() << ", "
                                      << targetPosition.z() << ")" << std::endl;
     **/

     Eigen::Vector3f diffPosition = targetPosition - currentPosition;

     if ( _animationFirstStep )
     {
       _animationSpeed = diffPosition.norm( ) / _animationDuration;
       _animationFirstStep = false;
     }

     // Position-in-place checking.
     float distance = dt * _animationSpeed;
     bool positionInPlace = false;
     Eigen::Vector3f nextPosition = currentPosition;
     if ( ( positionInPlace = ( diffPosition.norm() <= distance ) ) )
       nextPosition = targetPosition;
     else
       nextPosition = currentPosition + diffPosition.normalized() * distance;

     //Eigen::Vector3f increment = nextPosition - currentPosition;

     Eigen::Matrix3f targetOrientation =
       _path->evaluateOrientation( _currentNodeId,
                                   _currentT );

     /*
     std::cout << "targetOrientation: " << std::endl;
     std::cout << "("  << targetOrientation(0,0) << ", " << targetOrientation(1,0) << ", "  << targetOrientation(2,0) << ")" << std::endl;
     std::cout << "("  << targetOrientation(0,1) << ", " << targetOrientation(1,1) << ", "  << targetOrientation(2,1) << ")" << std::endl;
     std::cout << "("  << targetOrientation(0,2) << ", " << targetOrientation(1,2) << ", "  << targetOrientation(2,2) << ")" << std::endl;
     std::cout << std::endl;
     */

     // Applying rotation and translation.
     Eigen::Matrix4f newViewMatrix = _camera->viewMatrix( );
     newViewMatrix.block( 0, 0, 3, 3 ) = targetOrientation;
     newViewMatrix( 0, 3 ) = nextPosition.x( );
     newViewMatrix( 1, 3 ) = nextPosition.y( );
     newViewMatrix( 2, 3 ) = nextPosition.z( );

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
       /**if( positionInPlace )**/ _currentT += _tStep;
     }

     _animationPreviousTime = currentTime;
     return true;
    }

    _animationPreviousTime = currentTime;
    return false;
  }

  Eigen::Matrix3f CameraController::generateRotationMatrix( float yaw_, float pitch_ )
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

  Eigen::Matrix4f CameraController::lookAt( Eigen::Vector3f position_,
                                            Eigen::Vector3f lookAt_,
                                            Eigen::Vector3f up_ )
  {
    // Updating vectors.
    Eigen::Vector3f eyeToCenter = lookAt_.normalized( );
    Eigen::Vector3f upVector = up_ - up_.dot( eyeToCenter ) * eyeToCenter;
    upVector.normalize( );
    Eigen::Vector3f tangentVector = eyeToCenter.cross( upVector );
    tangentVector.normalize( );

    // Setting vectors.
    _camera->position( position_ );
    _camera->lookAt( eyeToCenter );
    _camera->up( upVector );

    // Relative position.
    float x = -tangentVector.dot( position_ );
    float y = -upVector.dot( position_ );
    float z = eyeToCenter.dot( position_ );

    Eigen::Matrix4f toReturn;
    toReturn << tangentVector.x( ), tangentVector.y( ), tangentVector.z( ), x,
                upVector.x( ), upVector.y( ), upVector.z( ), y,
                -eyeToCenter.x( ), -eyeToCenter.y( ), -eyeToCenter.z( ), z,
                0.0f, 0.0f, 0.0f, 1.0f;

    return toReturn;
  }

}
