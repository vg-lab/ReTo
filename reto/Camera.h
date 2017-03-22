/*
 * Copyright (c) 2014-2016 GMRV/URJC.
 *
 * Authors: Juan Jose Garcia
 * Authors: Cristian Rodríguez Bernal
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

#ifndef __RETO_CAMERA__
#define __RETO_CAMERA__

// Eigen
#include <Eigen/Dense>

#include <reto/api.h>

namespace reto
{

  class Camera
  {
  public:

    RETO_API
    Camera( unsigned int width_ = 1920, unsigned int height_ = 1080,
            float nearPlane_ = 0.1f, float farPlane_ = 10000.0f,
            Eigen::Vector3f position_ = Eigen::Vector3f( 0.0f ),
            Eigen::Matrix3f orientation_ = Eigen::Matrix3f::Identity( ),
            float fov_ = 45.0f );

    RETO_API
    virtual ~Camera( void );

    /** BEGIN getters & setters **/

    RETO_API
    unsigned int getWidth( void );

    RETO_API
    void setWidth( unsigned int width_ );

    RETO_API
    unsigned int getHeight( void );

    RETO_API
    void setHeight( unsigned int height_ );

    RETO_API
    float getNearPlane( void );

    RETO_API
    void setNearPlane( float nearPlane_ );

    RETO_API
    float getFarPlane( void );

    RETO_API
    void setFarPlane( float farPlane_ );

    RETO_API
    Eigen::Vector3f getPosition( void );

    RETO_API
    void setPosition( Eigen::Vector3f position_ );

    RETO_API
    Eigen::Matrix3f getOrientation( void );

    RETO_API
    void setOrientation( Eigen::Matrix3f orientation_ );

    RETO_API
    Eigen::Matrix4f getViewMatrix( void );

    RETO_API
    void setViewMatrix( Eigen::Matrix4f viewMatrix_ );

    RETO_API
    Eigen::Matrix4f getProjMatrix( void );

    RETO_API
    void setProjMatrix( Eigen::Matrix4f projMatrix_ );

    RETO_API
    Eigen::Matrix4f getViewProjMatrix( void );

    RETO_API
    void setViewProjMatrix( Eigen::Matrix4f viewProjMatrix_ );

    RETO_API
    float getFOV( void );

    RETO_API
    void setFOV( float fov_ );

    /** END getters & setters **/

protected:

    /** BEGIN basic attributes **/

    unsigned int _width;
    unsigned int _height;
    float _nearPlane;
    float _farPlane;
    Eigen::Vector3f _position;
    Eigen::Matrix3f _orientation;
    Eigen::Matrix4f _viewMatrix;
    Eigen::Matrix4f _projMatrix;
    Eigen::Matrix4f _viewProjMatrix;
    float _fov;

    /** END basic attributes **/

  };

} //end namespace reto

#endif // __RETO_CAMERA__
