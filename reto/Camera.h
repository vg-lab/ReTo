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
            Eigen::Vector3f position_ = Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
            Eigen::Vector3f up_ = Eigen::Vector3f( 0.0f, 1.0f, 0.0f ),
            Eigen::Vector3f lookAt_ = Eigen::Vector3f( 0.0f, 0.0f, 1.0f ),
            float fov_ = 45.0f );

    RETO_API
    virtual ~Camera( void );

    /** BEGIN getters & setters **/

    RETO_API
    unsigned int width( void ) const;

    RETO_API
    void width( unsigned int width_ );

    RETO_API
    unsigned int height( void ) const;

    RETO_API
    void height( unsigned int height_ );

    RETO_API
    float nearPlane( void ) const;

    RETO_API
    void nearPlane( float nearPlane_ );

    RETO_API
    float farPlane( void ) const;

    RETO_API
    void farPlane( float farPlane_ );

    RETO_API
    Eigen::Vector3f position( void ) const;

    RETO_API
    void position( Eigen::Vector3f position_ );

    RETO_API
    Eigen::Vector3f up( void ) const;

    RETO_API
    void up( Eigen::Vector3f up_ );

    RETO_API
    Eigen::Vector3f lookAt( void ) const;

    RETO_API
    void lookAt( Eigen::Vector3f lookAt );

    RETO_API
    Eigen::Matrix4f viewMatrix( void ) const;

    RETO_API
    void viewMatrix( Eigen::Matrix4f viewMatrix_ );

    RETO_API
    Eigen::Matrix4f projMatrix( void ) const;

    RETO_API
    void projMatrix( Eigen::Matrix4f projMatrix_ );

    RETO_API
    Eigen::Matrix4f viewProjMatrix( void ) const;

    RETO_API
    void viewProjMatrix( Eigen::Matrix4f viewProjMatrix_ );

    RETO_API
    float fov( void ) const;

    RETO_API
    void fov( float fov_ );

    /** END getters & setters **/

protected:

    /** BEGIN basic attributes **/

    unsigned int _width;
    unsigned int _height;
    float _nearPlane;
    float _farPlane;
    Eigen::Vector3f _position;
    Eigen::Vector3f _up;
    Eigen::Vector3f _lookAt;
    Eigen::Matrix4f _viewMatrix;
    Eigen::Matrix4f _projMatrix;
    Eigen::Matrix4f _viewProjMatrix;
    float _fov;

    /** END basic attributes **/

  };

} //end namespace reto

#endif // __RETO_CAMERA__
