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

#ifndef __RETO_CAMERACONTROLLER__
#define __RETO_CAMERACONTROLLER__

// Eigen
#include <Eigen/Dense>

#include <iostream>
#include <chrono>
#include <ctime>

#include "Camera.h"
#include "Path.h"

#include <reto/api.h>

namespace reto
{

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
                      Path* path_ = new Path( Path::TInterpolationMethod::LERP ) );

    RETO_API
    virtual ~CameraController( void );

    /** BEGIN wrappers **/

    RETO_API
    void path( Path* path_ );

    /** END wrappers **/

    /** BEGIN basic functions **/

    RETO_API
    void center( Eigen::Vector3f centeredPosition_ = Eigen::Vector3f( 0.0f ) );

    RETO_API
    void zoom( float increment_ );

    RETO_API
    void resize( float width_, float height_ );

    RETO_API
    void localTranslation( Eigen::Vector3f increment_ );

    RETO_API
    void moveUsingLookAtVector( float increment_ );

    RETO_API
    void moveUsingTangentVector( float increment_ );

    RETO_API
    void localRotation( float yaw_, float pitch_ );

    /** END basic functions **/

    RETO_API
    void triggerAnimation( void );

    RETO_API
    bool animate( void );

    // Temporary.
    Camera* _camera;

  private:

    /** BEGIN auxiliar functions **/

    RETO_API
    Eigen::Matrix3f generateRotationMatrix( float yaw_, float pitch_ );

    RETO_API
    Eigen::Matrix4f lookAt( Eigen::Vector3f position_,
                            Eigen::Vector3f lookAt_,
                            Eigen::Vector3f up_ );

    /** END auxiliar functions **/

    /** BEGIN basic attributes **/

    TProjection _projection;
    TCamera _cameraType;
    Path* _path;

    /** END basic attributes **/

    /** BEGIN animation attributes **/

    bool _isAniming;
    bool _animationFirstStep;
    float _animationSpeed;
    float _animationDuration;
    std::chrono::time_point< std::chrono::system_clock > _animationPreviousTime;

    unsigned int _currentNodeId;
    float _currentT;
    float _tStep;

    /** END animation attributes **/

  };

}

#endif
