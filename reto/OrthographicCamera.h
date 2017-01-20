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

#ifndef __RETO_ORTHOGRAPHICCAMERA__
#define __RETO_ORTHOGRAPHICCAMERA__

// Eigen
#include <Eigen/Dense>

#include <vector>
#include <iostream>
#include <string>
#include <chrono>

#include <reto/api.h>

#include "Camera.h"

namespace reto
{

  class OrthographicCamera : public Camera
  {
  public:

    RETO_API
    OrthographicCamera( float width_ = 1920.0f, float height_ = 1080.0f,
                        float nearPlane_ = 0.1f, float farPlane_ = 10000.0f,
                        Eigen::Vector3f position_ = Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
                        float yaw_ = 0.0f, float pitch_ = 0.0f );

    /*
    #ifdef RETO_USE_ZEROEQ
    RETO_API
    OrthographicCamera( const std::string& session_,
                        float width_ = 1920.0f, float height_ = 1080.0f,
                        float nearPlane_ = 0.1f, float farPlane_ = 10000.0f,
                        Eigen::Vector3f position_ = Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
                        float yaw_ = 0.0f, float pitch_ = 0.0f );
    #endif
    */

    virtual ~OrthographicCamera();

    /** BEGIN basic_functions **/

    RETO_API
    virtual void center( Eigen::Vector3f centeredPosition_ );

    RETO_API
    virtual void zoom( float increment_ );

    /**
    * Method to translate the camera from the actual position
    * @param increment_ camera translation
    */
    RETO_API
    virtual void localTranslation( Eigen::Vector3f increment_ );

    /**
    * Method to rotate the camera from the actual rotation
    * @param yaw camera rotation of y exe
    * @param pitch camera rotation of x exe
    */
    RETO_API
    virtual void localRotation( float yaw_, float pitch_ );

    /** END basic_functions **/

    /** BEGIN getters & setters **/

    /**
     * Method to set window size
     * @param w: Window width
     * @param h: Window height
     */
    RETO_API
    void setWindowSize( const int width_, const int height_ );

    /**
     * Method to set the camera projection ratio
     * @param ratio camera projection ratio
     */
    RETO_API
    void ratio( float ratio_ );

    /**
     * Method to set the rotation of the camera
     * @param yaw camera rotation of y exe
     * @param pitch camera rotation of x exe
     */
    RETO_API
    void rotation( const float yaw_, const float pitch_ );

    /** END getters & setters **/

  private:

    RETO_API
    void _BuildProjectionMatrix( void );

    RETO_API
    void _BuildViewProjectionMatrix( void );

  };

} /* namespace reto */

#endif /* __RETO_ORTHOGRAPHICCAMERA__ */
