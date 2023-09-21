/*
 * Copyright (c) 2014-2019 VG-Lab/URJC.
 *
 * Authors: Juan Jose Garcia Cantero <juanjose.garcia@urjc.es>
 *          Cristian Rodríguez Bernal <cristian.rodriguez@urjc.es>
 *
 * This file is part of ReTo <https://gitlab.vg-lab.es/nsviz/ReTo>
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

#include <reto/api.h>

#ifdef _MSC_VER
#pragma warning(disable: 4251)
#endif

// Eigen
#include <Eigen/Dense>

#include <vector>
#include <iostream>
#include <string>
#include <chrono>
#include <memory>

#ifdef RETO_USE_ZEROEQ
#include <zeroeq/zeroeq.h>
#include <servus/uri.h>

#include <mutex>
#include <boost/bind/bind.hpp>

#include <thread>

#ifdef RETO_USE_LEXIS
#include <lexis/lexis.h>
#endif

#endif

namespace reto
{
  //! Class to manage camera
  /*!
   * This class manage the camera: allows synchronize the camera with other
   * applications using of ZeroEQ
   */
  class RETO_API Camera
  {
    friend class AbstractCameraController;

  public:
    static const std::string NO_ZEROEQ; /** use as session name to disable ZEROEQ */

    /**
     * Camera constructor
     * @param fov camera field of view
     * @param ratio camera ratio
     * @param nearPlane camera near plane
     * @param farPlane camera far plane
     */
    Camera( float fov_ = 45.0f, float ratio_ = 16.0f/9,
            float nearPlane_ = 10.0f, float farPlane_ = 10000.0f );

    /**
     * Camera constructor
     * @param session ZeroEq session to synchronize the camera with other
     * applications
     * @param fov camera field of view
     * @param ratio camera ratio
     * @param nearPlane camera near plane
     * @param farPlan camera far plane
     */
    Camera( const std::string& session_,
#ifdef RETO_USE_ZEROEQ
      std::shared_ptr<zeroeq::Subscriber> subscriber,
#endif
            float fov_ = 45.0f,
            float ratio_ = 16.0f/9, float nearPlane_ = 10.0f,
            float farPlane_ = 10000.0f );

    /**
     * Default destructor
     */
    ~Camera( void );

    /**
     * Method to start or change the camera zeqSession
     @param session ZeroEq session to synchronize the camera with other
     * applications
     */
    void setZeqSession( const std::string& session_ = std::string()
#ifdef RETO_USE_ZEROEQ
      , const std::shared_ptr<zeroeq::Subscriber> subscriber = nullptr
#endif
                      );

    /**
     * Method to obtain the near plane distance.
     * @return camera near plane distance.
     */
    float nearPlane( void ) const;

    /**
     * Method to establish the near plane distance.
     @param nearPlane_ new value to be assigned as near plane distance.
     */
    void nearPlane( float nearPlane_ );

    /**
     * Method to obtain the far plane distance.
     * @return camera far plane distance.
     */
    float farPlane( void ) const;

    /**
     * Method to establish the far plane distance.
     @param farPlane_ new value to be assigned as far plane distance.
     */
    void farPlane( float farPlane_ );

    /**
     * Method to obtain the field of view angle value.
     * @return field of view angle.
     */
    float fieldOfView( void ) const;

    /**
     * Method to establish field of view angle.
     @param fov_ new value to be assigned as field of view.
     */
    void fieldOfView( float fov_ );

    /**
     * Method to get the pointer to projection matrix of the camera vectorized
     * in columns
     * @return pointer to float whit the column vectorized projection matrix of
     * the camera
     */
    float* projectionMatrix( void );

    /**
     * Method to get the pointer to view matrix of the camera vectorized
     * in columns
     * @return pointer to float with the column vectorized view matrix of
     * the camera
     */
    float* viewMatrix( void );

    /**
     * Method to get the pointer to view-projection matrix of the camera
     * vectorized in columns
     * @return pointer to float with the column vectorized view-projection
     * matrix of the camera
     */
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
    float _fovTan;

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

#ifdef RETO_USE_ZEROEQ
    //! ZeroEQ session to synchronize the camera with other apps
    std::string _zeroeqSession;

    //! ZeroEQ subscriber
    std::shared_ptr<zeroeq::Subscriber> _subscriber;

    //! Thread that runs the ZeroEQ subscriber
    std::thread* _subscriberThread;
  private:
    void initializeZeroEQ(const std::string &session = std::string(),
                          std::shared_ptr<zeroeq::Subscriber> subscriber = nullptr);

    void deinitializeZeroEQ();
#endif

  };
} //end namespace reto

#endif // __RETO_CAMERA__
