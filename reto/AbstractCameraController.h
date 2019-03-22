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

#ifndef __RETO_ABSTRACT_CAMERA_CONTROLLER__
#define __RETO_ABSTRACT_CAMERA_CONTROLLER__

#include "Camera.h"
#include "CameraAnimation.h"

#include <reto/api.h>

namespace reto
{
  class AbstractCameraController
  {
  public:

    RETO_API
    AbstractCameraController( Camera* camera_ = nullptr );

    RETO_API
    virtual ~AbstractCameraController( void );

    RETO_API
    Camera* camera( void );

    RETO_API
    void update( void );

    RETO_API
    void anim( float deltaTime_ = 1.0f / 60 );

    RETO_API
    void startAnim( CameraAnimation* cameraAnimation_, bool loop_ = false );

    RETO_API
    void stopAnim( void );

    RETO_API
    bool isAniming( void );

    RETO_API
    void position( const Eigen::Vector3f& position_ );

    RETO_API
    Eigen::Vector3f position( void );

    RETO_API
    void rotation( const Eigen::Matrix3f& rotation_ );

    RETO_API
    void rotation( const Eigen::Vector3f& rotationAngles_ );

    RETO_API
    Eigen::Matrix3f rotation( void );

    RETO_API
    void radius( float radius_ );

    RETO_API
    float radius( void );

    RETO_API
    void translate( const Eigen::Vector3f& translation_ );

    RETO_API
    virtual void localTranslate( const Eigen::Vector3f& translation_ ) = 0;

    RETO_API
    void rotate( const Eigen::Matrix3f& rotation_ );

    RETO_API
    void rotate( const Eigen::Vector3f& rotationAngles_ );

    RETO_API
    void windowSize( int width_, int height_ );

    RETO_API
    Eigen::Matrix3f rotationMatrixFromAngles(
      const Eigen::Vector3f& rotationAngles_ );

  protected:

    virtual void _conformSetViewMatrix( const Eigen::Vector3f& position_,
                                        const Eigen::Matrix3f& rotation_,
                                        float radius_ ) = 0;

    void _setCameraViewMatrix( const Eigen::Matrix4f& viewMatrix_ );

    Camera* _camera;

    Eigen::Vector3f _position;

    Eigen::Matrix3f _rotation;

    float _radius;

    bool _isAniming;

    float _currentAnimTime;

    CameraAnimation* _currentAnim;

    bool _loopAnim;

  };

}


#endif
