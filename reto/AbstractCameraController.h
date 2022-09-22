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

#include <reto/api.h>
#include <string>

#ifdef _MSC_VER
#pragma warning(disable: 4251)
#endif

#include <Eigen/Dense>

#ifdef RETO_USE_ZEROEQ
#include <memory>
#include <zeroeq/zeroeq.h>
#endif

namespace reto
{
  class Camera;
  class CameraAnimation;

  class RETO_API AbstractCameraController
  {
  public:
    AbstractCameraController( Camera* camera_ = nullptr, const std::string zeqSession = std::string()
#ifdef RETO_USE_ZEROEQ
        , std::shared_ptr<zeroeq::Subscriber> subscriber = nullptr
#endif
        );

    virtual ~AbstractCameraController( void )
    {};

    Camera* camera( void );

    void update( void );

    void anim( const float deltaTime_ = 1.0f / 60 );

    void startAnim( CameraAnimation* cameraAnimation_, bool loop_ = false );

    void stopAnim( void );

    bool isAniming( void ) const;

    virtual void position( const Eigen::Vector3f& position_ );

    virtual Eigen::Vector3f position( void ) const;

    void rotation( const Eigen::Matrix3f& rotation_ );

    void rotation( const Eigen::Vector3f& rotationAngles_ );

    Eigen::Matrix3f rotation( void ) const;

    void radius( float radius_ );

    float radius( void ) const;

    void translate( const Eigen::Vector3f& translation_ );

    virtual void localTranslate( const Eigen::Vector3f& translation_ ) = 0;

    void rotate( const Eigen::Matrix3f& rotation_ );

    void rotate( const Eigen::Vector3f& rotationAngles_ );

    void windowSize( int width_, int height_ );

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
