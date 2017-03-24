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
                                      TCamera cameraType_ )
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
  }

  CameraController::~CameraController( void )
  {
    delete _camera;
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

        _camera->position( newPosition );

        // Updating only position.
        newViewMatrix( 0, 3 ) = _camera->position().x( );
        newViewMatrix( 1, 3 ) = _camera->position().y( );
        newViewMatrix( 2, 3 ) = _camera->position().z( );

        //newViewMatrix = lookAt( _camera->position( ), _camera->lookAt( ), _camera->up( ) );
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

        _camera->position( newPosition );

        // Updating only position.
        newViewMatrix( 0, 3 ) = _camera->position().x( );
        newViewMatrix( 1, 3 ) = _camera->position().y( );
        newViewMatrix( 2, 3 ) = _camera->position().z( );

        //newViewMatrix = lookAt( _camera->position( ), currentLookAt, _camera->up( ) );
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

        _camera->position( newPosition );

        // Updating only position.
        newViewMatrix( 0, 3 ) = _camera->position().x( );
        newViewMatrix( 1, 3 ) = _camera->position().y( );
        newViewMatrix( 2, 3 ) = _camera->position().z( );

        //newViewMatrix = lookAt( _camera->position( ), currentLookAt, currentUp );
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
        /**
        Eigen::Vector3f newLookAt;
        newLookAt( 0 ) = cos( yaw_ ) * cos( pitch_ );
        newLookAt( 1 ) = sin( yaw_ ) * cos( pitch_ );
        newLookAt( 2 ) = sin( pitch_ );
        newLookAt.normalize( );

        _camera->lookAt( newLookAt );

         newViewMatrix = lookAt( _camera->position( ), _camera->lookAt( ), _camera->up( ) );
        **/

        /**/
        Eigen::Matrix3f currentOrientation =
          newViewMatrix.block( 0, 0, 3, 3 );
        Eigen::Matrix3f rotationMatrix = generateRotationMatrix( yaw_, pitch_ );
        Eigen::Matrix3f newOrientation = rotationMatrix * currentOrientation;
        newViewMatrix.block( 0, 0, 3, 3 ) = newOrientation;
        /**/
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
    // Orientation.
    Eigen::Vector3f eyeToCenter = lookAt_.normalized( );
    //Eigen::Vector3f eyeToCenter = center_ - eye_;
    //eyeToCenter.normalize( );
    Eigen::Vector3f upVector = up_.normalized( );
    Eigen::Vector3f tangentVector = eyeToCenter.cross( upVector );
    tangentVector.normalize( );
    upVector = tangentVector.cross( eyeToCenter );

    // IMPORTANT: Updating up vector.
    _camera->up( upVector );

    // Position.
    float x = position_.x( );
    //float x = -tangentVector.dot( position_ );
    float y = position_.y( );
    //float y = -upVector.dot( position_ );
    float z = position_.z( );
    //float z = eyeToCenter.dot( position_ );

    _camera->position( Eigen::Vector3f( x, y, z ) );

    Eigen::Matrix4f toReturn;
    toReturn << tangentVector.x( ), upVector.x( ), -eyeToCenter.x( ), x,
                tangentVector.y( ), upVector.y( ), -eyeToCenter.y( ), y,
                tangentVector.z( ), upVector.z( ), -eyeToCenter.z( ), z,
                0.0f, 0.0f, 0.0f, 1.0f;

    return toReturn;
  }

}
