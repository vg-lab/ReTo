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

#include <vector>
#include <iostream>
#include <string>
#include <chrono>

#include <reto/api.h>

namespace reto
{

  //! Class to manage camera
  /*!
    This class manage the camera: allows to move, rotate, make zoom, anim and
    automatically synchronize the camera with other applications using of ZeroEQ
   */
  class Camera
  {
  public:

    /**
     * Camera constructor
     * @param fov camera field of view
     * @param ratio camera ratio
     * @param nearPlane camera near plane
     * @param farPlan camera far plane
     * @param pivot camera rotation pivot
     * @param radius camera distance from the camera pivot
     * @param yaw camera rotation of y exe
     * @param pitch camera rotation of x exe
     */
    RETO_API
    Camera( float width_ = 1920.0f, float height_ = 1080.0f,
            float nearPlane_ = 0.1f, float farPlane_ = 10000.0f,
            Eigen::Vector3f position_ = Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
            float yaw_ = 0.0f, float pitch_ = 0.0f );

    /**
     * Default destructor
     */
    RETO_API
    virtual ~Camera( void );

    /** BEGIN basic_functions **/

    RETO_API
    virtual void center( Eigen::Vector3f centeredPosition_ ) = 0;

    RETO_API
    virtual void zoom( float increment_ ) = 0;

    /**
     * Method to translate the camera from the actual position
     * @param increment_ camera translation
     */
    RETO_API
    virtual void localTranslation( Eigen::Vector3f increment_ ) = 0;

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
     * Method to get window width size
     * @return window width size
     */
    RETO_API
    int width( void );

    /**
     * Method to get window height size
     * @return window height size
     */
    RETO_API
    int height( void );

    /**
     * Method to set window size
     * @param w: Window width
     * @param h: Window height
     */
    RETO_API
    virtual void setWindowSize( const int width_, const int height_ );

    /**
     * Method to set the camera projection ratio
     * @param ratio camera projection ratio
     */
    RETO_API
    virtual void ratio( float ratio_ );

    /**
     * Method to get and set the near plane of the camera
     * @return the reference to a float with the near plane attrib of the camera
     */
    RETO_API
    float& nearPlane( void )
    {
      return _nearPlane;
    }

    /**
     * Method to get and set the far plane of the camera
     * @return the reference to a float with the far plane attrib of the camera
     */
    RETO_API
    float& farPlane( void )
    {
      return _farPlane;
    }

    /**
     * Method to get the pointer to the camera position vectorized
     * @return pointer to float whit the vectorized camera position
     */
    RETO_API
    float* position( void );

    /**
     * Method to get the pointer to view matrix of the camera vectorized
     * in columns
     * @return pointer to float whit the column vectorized view matrix of
     * the camera
     */
    RETO_API
    float* viewMatrix( void );

    /**
     * Method to get the pointer to projection matrix of the camera vectorized
     * in columns
     * @return pointer to float whit the column vectorized projection matrix of
     * the camera
     */
    RETO_API
    float* projectionMatrix( void );

    /**
     * Method to get the pointer to projectionview matrix of the camera
     * vectorized in columns
     * @return pointer to float whit the column vectorized projectionview
     * matrix of the camera
     */
    RETO_API
    float* viewProjectionMatrix( void );

    /** END getters & setters **/

protected:

    RETO_API
    Eigen::Matrix3f _RotationFromPY( float yaw_, float pitch_ );

    RETO_API
    virtual void _BuildViewMatrix( void );

    RETO_API
    virtual void _BuildProjectionMatrix( void ) = 0;

    RETO_API
    virtual void _BuildViewProjectionMatrix( void ) = 0;

    /** BEGIN auxiliar **/

    virtual void _PositionVectorized( const std::vector<float>& positionVec_ );

    virtual void _ViewMatrixVectorized( const std::vector<float>& viewVec_ );

    virtual void _Rotation( const Eigen::Matrix3f& rotation_ );

    /** END auxiliar **/

    /** BEGIN basic_attributes **/

    //! Window width
    int _width;

    //! Window height
    int _height;

    //! Camera ratio
    float _ratio;

    //! Camera near plane
    float _nearPlane;

    //! Camera far plane
    float _farPlane;

    //! Vectorized camera position
    std::vector<float> _positionVec;

    //! Camera rotation matrix
    Eigen::Matrix3f _rotation;

    //! Vectorized camera view matrix
    std::vector<float> _viewVec;

    //! Vectorized camera projection matrix
    std::vector<float> _projVec;

    //! Vectorized camera viewprojection matrix
    std::vector<float> _viewProjVec;

    /** END basic_attributes **/

  };
} //end namespace reto

#endif // __RETO_CAMERA__
