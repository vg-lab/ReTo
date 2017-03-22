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

    Eigen::Vector3f defaultPosition = _camera->getPosition( );
    Eigen::Matrix3f defaultOrientation = _camera->getOrientation( );

    float defaultNearPlane = _camera->getNearPlane( );
    float defaultFarPlane = _camera->getFarPlane( );

    Eigen::Matrix4f viewMatrix;
    Eigen::Matrix4f projMatrix;
    Eigen::Matrix4f viewProjMatrix;

    switch( _cameraType )
    {
      case STANDARD :
      {
        viewMatrix << defaultOrientation( 0, 0 ), defaultOrientation( 0, 1 ), defaultOrientation( 0, 2 ), defaultPosition( 0 ),
                      defaultOrientation( 1, 0 ), defaultOrientation( 1, 1 ), defaultOrientation( 1, 2 ), defaultPosition( 1 ),
                      defaultOrientation( 2, 0 ), defaultOrientation( 2, 1 ), defaultOrientation( 2, 2 ), defaultPosition( 2 ),
                      0.0f, 0.0f, 0.0f, 1.0f;
      }
      break;

      case ORBITAL :
      {
        viewMatrix = Eigen::Matrix4f::Identity( );

        std::cerr << "Not implemented yet" << std::endl;
      }
      break;
    }

    _camera->setViewMatrix( viewMatrix );

    switch( _projection )
    {
      case PERSPECTIVE :
      {
        float defaultFov = _camera->getFOV();

        defaultFov *= ( M_PI / 360.0f );

        float f = 1.0f / tan( defaultFov );

        float ratio = _camera->getWidth( ) / _camera->getHeight( );

        float inverseOfNearMinusFar = 1.0f / ( defaultNearPlane - defaultFarPlane );

        projMatrix
        << f / ratio, 0.0f, 0.0f, 0.0f,
           0.0f, f, 0.0f, 0.0f,
           0.0f, 0.0f, ( defaultFarPlane + defaultNearPlane  ) * inverseOfNearMinusFar, ( 2.0f * defaultFarPlane * defaultNearPlane ) * inverseOfNearMinusFar,
           0.0f, 0.0f, -1.0f, 0.0f;

        _camera->setProjMatrix( projMatrix );
      }
      break;

      case ORTHOGRAPHIC :
      {
        float right = _camera->getWidth( ) * 0.5f;
        float top = _camera->getHeight( ) * 0.5f;

        projMatrix
        << 1.0f / right, 0.0f, 0.0f, 0.0f,
           0.0f, 1.0f / top, 0.0f, 0.0f,
           0.0f, 0.0f, -2.0f / ( defaultFarPlane - defaultNearPlane ), -1.0f * ( defaultFarPlane + defaultNearPlane ) / ( defaultFarPlane - defaultNearPlane ),
           0.0f, 0.0f, 0.0f, 1.0f;
      }
      break;
    }

    _camera->setProjMatrix( projMatrix );

    viewProjMatrix = projMatrix * viewMatrix;

    _camera->setViewProjMatrix( viewProjMatrix );
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
        _camera->setPosition( centeredPosition_ );
        _camera->setOrientation( Eigen::Matrix3f::Identity( ) );

        Eigen::Vector3f currentPosition = _camera->getPosition( );
        Eigen::Matrix3f currentOrientation = _camera->getOrientation( );

        newViewMatrix << currentOrientation( 0, 0 ), currentOrientation( 0, 1 ), currentOrientation( 0, 2 ), currentPosition( 0 ),
                         currentOrientation( 1, 0 ), currentOrientation( 1, 1 ), currentOrientation( 1, 2 ), currentPosition( 1 ),
                         currentOrientation( 2, 0 ), currentOrientation( 2, 1 ), currentOrientation( 2, 2 ), currentPosition( 2 ),
                         0.0f, 0.0f, 0.0f, 1.0f;
      }
      break;

      case ORBITAL :
      {
        newViewMatrix = Eigen::Matrix4f::Identity( );

        std::cerr << "Not implemented yet" << std::endl;
      }
      break;
    }

    _camera->setViewMatrix( newViewMatrix );

    Eigen::Matrix4f newViewProjMatrix = _camera->getProjMatrix( ) * newViewMatrix;

    _camera->setViewProjMatrix( newViewProjMatrix );
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
        float currentRatio = _camera->getWidth( ) / _camera->getHeight( );
        float currentFov = _camera->getFOV( );

        float newFov = currentFov * scale;

        _camera->setFOV( newFov );

        newFov *= ( M_PI / 360.0f );

        float newF = 1.0f / tan( newFov );

        newProjMatrix = _camera->getProjMatrix( );
        newProjMatrix( 0, 0 ) = newF / currentRatio;
        newProjMatrix( 1, 1 ) = newF;
      }
      break;

      case ORTHOGRAPHIC :
      {
        float currentWidth = _camera->getWidth( );
        float currentHeight = _camera->getHeight( );

        float newWidth = currentWidth * scale;
        float newHeight = currentHeight * scale;

        _camera->setWidth( newWidth );
        _camera->setHeight( newHeight );

        float newRight = newWidth * 0.5f;
        float newTop = newHeight * 0.5f;

        newProjMatrix = _camera->getProjMatrix( );
        newProjMatrix( 0, 0 ) = 1.0f / newRight;
        newProjMatrix( 1, 1 ) = 1.0f / newTop;
      }
      break;
    }

    _camera->setProjMatrix( newProjMatrix );

    Eigen::Matrix4f newViewProjMatrix = newProjMatrix * _camera->getViewMatrix( );

    _camera->setViewProjMatrix( newViewProjMatrix );
  }

  void CameraController::resize( float width_, float height_ )
  {

    Eigen::Matrix4f newProjMatrix = Eigen::Matrix4f::Identity( );

    // Setting new width and height.
    _camera->setWidth( width_ );
    _camera->setHeight( height_ );

    switch( _projection )
    {
      case PERSPECTIVE :
      {
        float newRatio = width_ / height_;

        float currentFov = _camera->getFOV();
        currentFov *= ( M_PI / 360.0f );
        float f = 1.0f / tan( currentFov );

        newProjMatrix = _camera->getProjMatrix( );
        newProjMatrix( 0, 0 ) = f / newRatio;
      }
      break;

      case ORTHOGRAPHIC :
      {
        float newRight = width_ * 0.5f;
        float newTop = height_ * 0.5f;

        newProjMatrix = _camera->getProjMatrix( );
        newProjMatrix( 0, 0 ) = 1.0f / newRight;
        newProjMatrix( 1, 1 ) = 1.0f / newTop;
      }
      break;
    }

    _camera->setProjMatrix( newProjMatrix );

    Eigen::Matrix4f newViewProjMatrix = newProjMatrix * _camera->getViewMatrix( );

    _camera->setViewProjMatrix( newViewProjMatrix );
  }

  void CameraController::localTranslation( Eigen::Vector3f increment_ )
  {
    Eigen::Matrix4f newViewMatrix = Eigen::Matrix4f::Identity( );

    switch( _cameraType )
    {
      case STANDARD :
      {
        Eigen::Vector3f currentPosition = _camera->getPosition( );

        Eigen::Vector3f newPosition = currentPosition + increment_;

        _camera->setPosition( newPosition );

        newViewMatrix = _camera->getViewMatrix( );
        newViewMatrix( 0, 3 ) = newPosition( 0 );
        newViewMatrix( 1, 3 ) = newPosition( 1 );
        newViewMatrix( 2, 3 ) = newPosition( 2 );
      }
      break;

      case ORBITAL :
      {
        newViewMatrix = Eigen::Matrix4f::Identity( );

        std::cerr << "Not implemented yet" << std::endl;
      }
      break;
    }

    _camera->setViewMatrix( newViewMatrix );

    Eigen::Matrix4f newViewProjMatrix = _camera->getProjMatrix( ) * newViewMatrix;

    _camera->setViewProjMatrix( newViewProjMatrix );
  }

  void CameraController::localRotation( float yaw_, float pitch_ )
  {
    Eigen::Matrix4f newViewMatrix = Eigen::Matrix4f::Identity( );

    switch( _cameraType )
    {
      case STANDARD :
      {
        Eigen::Matrix3f currentOrientation = _camera->getOrientation( );

        Eigen::Matrix3f rotationMatrix = generateRotationMatrix( yaw_, pitch_ );

        Eigen::Matrix3f newOrientation = rotationMatrix * currentOrientation;

        _camera->setOrientation( newOrientation );

        newViewMatrix = _camera->getViewMatrix( );
        newViewMatrix.block( 0, 0, 3, 3 ) = newOrientation;
      }
      break;

      case ORBITAL :
      {
        newViewMatrix = Eigen::Matrix4f::Identity( );

        std::cerr << "Not implemented yet" << std::endl;
      }
      break;
    }

    _camera->setViewMatrix( newViewMatrix );

    Eigen::Matrix4f newViewProjMatrix = _camera->getProjMatrix( ) * newViewMatrix;

    _camera->setViewProjMatrix( newViewProjMatrix );
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

}
