/*
 * Copyright (c) 2014-2017 GMRV/URJC.
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

#ifndef __RETO_CAMERACONTROLLER__
#define __RETO_CAMERACONTROLLER__

#include "Camera.h"
#include "Path.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <Eigen/Dense>
#include <reto/api.h>

namespace reto
{

  //! Camera controller
  /*!
    This class manages the camera. Basically it updates camera view and
    projection matrices. With this class the user can do the following
    actions with the camera: centering, zooming, resizing, translating,
    rotating and animating.
   */
  class CameraController
  {
  public:

    RETO_API
    enum TProjection
    {
      PERSPECTIVE = 0,
      ORTHOGRAPHIC
    };

    RETO_API
    enum TCamera
    {
      STANDARD = 0,
      ORBITAL
    };

    RETO_API
    CameraController( TProjection projection_,
                      TCamera cameraType_,
                      Path* path_ = new Path( Path::TInterpolationMethod::LERP ),
                      float animationDuration_ = 5.0f,
                      float tStep_ = 0.01f );

    RETO_API
    virtual ~CameraController( void );

    RETO_API
    Path* path( void );

    RETO_API
    void path( Path* path_ );

    RETO_API
    void center( const Eigen::Vector3f& position_ = Eigen::Vector3f( 0.0f, 0.0f, -500.0f ),
                 const Eigen::Vector3f& up_ = Eigen::Vector3f( 0.0f, 1.0f, 0.0f ),
                 const Eigen::Vector3f& lookAt_ = Eigen::Vector3f( 0.0f, 0.0f, 1.0f ) );

    RETO_API
    void zoom( float increment_ );

    RETO_API
    void resize( float width_, float height_ );

    RETO_API
    void translate( const Eigen::Vector3f& increment_ );

    RETO_API
    void translateInLookAtVectorDirection( float increment_ );

    RETO_API
    void translateInUpVectorDirection( float increment_ );

    RETO_API
    void translateInRightVectorDirection( float increment_ );

    RETO_API
    void localOrientation( float yaw_, float pitch_ );

    RETO_API
    bool animate( void );

    RETO_API
    void triggerAnimation( void );

    // Temporary.
    Camera* _camera;

  private:

    RETO_API
    Eigen::Matrix4f _lookAt( const Eigen::Vector3f& position_,
                             const Eigen::Vector3f& lookAt_,
                             const Eigen::Vector3f& up_ );

    RETO_API
    Eigen::Matrix3f _generateRotationMatrix( float yaw_, float pitch_ );

    TProjection _projection;
    TCamera _cameraType;
    Path* _path;

    bool _isAniming;
    bool _animationFirstStep;
    float _animationSpeed;
    float _animationDuration;
    std::chrono::time_point< std::chrono::system_clock > _animationPreviousTime;

    unsigned int _currentNodeId;
    float _currentT;
    float _tStep;

  };

} // end namespace reto

#endif // __RETO_CAMERA__
