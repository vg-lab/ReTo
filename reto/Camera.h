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


#ifndef __RETO_CAMERA__
#define __RETO_CAMERA__

// Eigen
#include <Eigen/Dense>

#include <vector>
#include <iostream>
#include <string>
#include <chrono>

#include <reto/api.h>

#ifdef RETO_USE_LEXIS
#include <zeroeq/zeroeq.h>
#include <servus/uri.h>

#include <mutex>
#include <boost/bind.hpp>

#ifdef RETO_USE_LEXIS
#include <lexis/lexis.h>
#endif

#include <thread>

#endif

namespace reto
{
  class AbstractCameraController;

  //! Class to manage camera
  /*!
   * This class manage the camera: allows synchronize the camera with other
   * applications using of ZeroEQ
   */
  class Camera
  {
    friend class AbstractCameraController;
    public:

    /**
     * Camera constructor
     * @param fov camera field of view
     * @param ratio camera ratio
     * @param nearPlane camera near plane
     * @param farPlane camera far plane
     */
    RETO_API
    Camera( float fov_ = 45.0f, float ratio_ = 16.0f/9,
            float nearPlane_ = 0.1f, float farPlane_ = 10000.0f );

    /**
     * Camera constructor
     * @param session ZeroEq session to synchronize the camera with other
     * applications
     * @param fov camera field of view
     * @param ratio camera ratio
     * @param nearPlane camera near plane
     * @param farPlan camera far plane
     */
    RETO_API
    Camera( const std::string& session_, float fov_ = 45.0f,
            float ratio_ = 16.0f/9, float nearPlane_ = 0.1f,
            float farPlane_ = 10000.0f );

    /**
     * Default destructor
     */
    RETO_API
    ~Camera( void );


    /**
     * Method to start or change the camera zeqSession
     @param session ZeroEq session to synchronize the camera with other
     * applications
     */
    RETO_API
    void setZeqSession( const std::string& session_ );

    /**
     * Method to obtain the near plane distance.
     * @return camera near plane distance.
     */
    RETO_API
    float nearPlane( void ) const;

    /**
     * Method to establish the near plane distance.
     @param nearPlane_ new value to be assigned as near plane distance.
     */
    RETO_API
    void nearPlane( float nearPlane_ );

    /**
     * Method to obtain the far plane distance.
     * @return camera far plane distance.
     */
    RETO_API
    float farPlane( void ) const;

    /**
     * Method to establish the far plane distance.
     @param farPlane_ new value to be assigned as far plane distance.
     */
    RETO_API
    void farPlane( float farPlane_ );

    /**
     * Method to obtain the field of view angle value.
     * @return field of view angle.
     */
    RETO_API
    float fieldOfView( void ) const;

    /**
     * Method to establish field of view angle.
     @param fov_ new value to be assigned as field of view.
     */
    RETO_API
    void fieldOfView( float fov_ );

    /**
     * Method to get the pointer to projection matrix of the camera vectorized
     * in columns
     * @return pointer to float whit the column vectorized projection matrix of
     * the camera
     */
    RETO_API
    float* projectionMatrix( void );

    /**
     * Method to get the pointer to view matrix of the camera vectorized
     * in columns
     * @return pointer to float with the column vectorized view matrix of
     * the camera
     */
    RETO_API
    float* viewMatrix( void );

    /**
     * Method to get the pointer to view-projection matrix of the camera
     * vectorized in columns
     * @return pointer to float with the column vectorized view-projection
     * matrix of the camera
     */
    RETO_API
    float* projectionViewMatrix( void );

  protected:

    void _setFov( float fov_ );

    void _setRatio( float ratio_ );

    void _setNearPlane( float nearPlane_ );

    void _setFarPlane( float farPlane_ );

    void _buildProjectionMatrix( void );

    void _setEnableZeqConnChanges( bool enableZeqConnChanges_ );

#ifdef RETO_USE_LEXIS
    void _onCameraEvent( lexis::render::ConstLookOutPtr event_ );
#endif

    //! Factor to calculate the camera projection matrix based on the camera
    //! far plane, near plane, field of view and ratio
    float _f;

    //! Camera fild of view
    float _fov;

    //! Camera ratio
    float _ratio;

    //! Camera near plane
    float _nearPlane;

    //! Camera far plane
    float _farPlane;

    //! Projection matrix
    Eigen::Matrix4f _projectionMatrix;

    //! Bool to check the clean state of projection matrix
    bool _isProjMatClean;

    //! View matrix
    Eigen::Matrix4f _viewMatrix;

    //! Projection view matrix
    Eigen::Matrix4f _projectionViewMatrix;

    //! Bool to enable zeq changes in view matrix
    bool _enableZeqConnChanges;

    //! State of the zeq connection: 1 activated, 0 desactivated
    bool _zeqConnection;

#ifdef RETO_USE_LEXIS

    //! ZeroEQ session to sinchronize the camera with other apps
    std::string _zeroeqSession;

    //! ZeroEQ publisher
    zeroeq::Publisher* _publisher;

    //! ZeroEQ subscriber
    zeroeq::Subscriber* _subscriber;

    //! Thread that runs the ZeroEQ subscriber
    std::thread* _subscriberThread;
#endif

  };

} //end namespace reto

#endif // __RETO_CAMERA__
