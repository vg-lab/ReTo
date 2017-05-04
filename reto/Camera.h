/*
 * Copyright (c) 2014-2017 GMRV/URJC.
 *
 * Authors: Juan Guerrero Martín
 * Authors: Cristian Rodríguez Bernal
 * Authors: Juan Jose Garcia
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

#include <reto/api.h>
#include <Eigen/Dense>

#ifdef RETO_USE_ZEROEQ
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

  //! Dummy camera
  /*!
    This class represents a basic camera.
   */
  class Camera
  {
  public:

    /**
     * Camera constructor
     * @param width frustum width
     * @param height frustum height
     * @param nearPlane camera near plane
     * @param farPlane camera far plane
     * @param position camera position in global coordinates
     * @param up camera up vector
     * @param lookAt vector from camera position to target
     * @param fov field of view for a perspective camera
     */
    RETO_API
    Camera( unsigned int width_ = 1920, unsigned int height_ = 1080,
            float nearPlane_ = 0.1f, float farPlane_ = 10000.0f,
            Eigen::Vector3f position_ = Eigen::Vector3f( 0.0f, 0.0f, -500.0f ),
            Eigen::Vector3f up_ = Eigen::Vector3f( 0.0f, 1.0f, 0.0f ),
            Eigen::Vector3f lookAt_ = Eigen::Vector3f( 0.0f, 0.0f, 1.0f ),
            float fov_ = 45.0f );

#ifdef RETO_USE_ZEROEQ
    /**
     * Camera constructor
     * @param session ZeroEq session to synchronize the camera with other
     * applications
     * @param width frustum width
     * @param height frustum height
     * @param nearPlane camera near plane
     * @param farPlane camera far plane
     * @param position camera position in global coordinates
     * @param up camera up vector
     * @param lookAt vector from camera position to target
     * @param fov field of view for a perspective camera
     */
    RETO_API
    Camera( const std::string& zeqSession_,
            unsigned int width_ = 1920, unsigned int height_ = 1080,
            float nearPlane_ = 0.1f, float farPlane_ = 10000.0f,
            Eigen::Vector3f position_ = Eigen::Vector3f( 0.0f, 0.0f, -500.0f ),
            Eigen::Vector3f up_ = Eigen::Vector3f( 0.0f, 1.0f, 0.0f ),
            Eigen::Vector3f lookAt_ = Eigen::Vector3f( 0.0f, 0.0f, 1.0f ),
            float fov_ = 45.0f );
#endif

    /**
     * Default destructor
     */
    RETO_API
    virtual ~Camera( void );

    /**
     * Method to get frustum width
     * @return unsigned integer with frustum width
     */
    RETO_API
    unsigned int width( void ) const;

    /**
     * Method to set frustum width
     * @param unsigned integer with the new frustum width
     */
    RETO_API
    void width( unsigned int width_ );

    /**
     * Method to get frustum height
     * @return unsigned integer with frustum height
     */
    RETO_API
    unsigned int height( void ) const;

    /**
     * Method to set frustum height
     * @param unsigned integer with the new frustum height
     */
    RETO_API
    void height( unsigned int height_ );

    /**
     * Method to get near plane (its z coordinate)
     * @return float with near plane z coordinate
     */
    RETO_API
    float nearPlane( void ) const;

    /**
     * Method to set near plane (its z coordinate)
     * @param float with the new near plane z coordinate
     */
    RETO_API
    void nearPlane( float nearPlane_ );

    /**
     * Method to get far plane (its z coordinate)
     * @return float with far plane z coordinate
     */
    RETO_API
    float farPlane( void ) const;

    /**
     * Method to set far plane (its z coordinate)
     * @param float with the new far plane z coordinate
     */
    RETO_API
    void farPlane( float farPlane_ );

    /**
     * Method to get camera position in global coordinates
     * @return Eigen::Vector3f with camera position
     */
    RETO_API
    Eigen::Vector3f position( void ) const;

    /**
     * Method to set camera position in global coordinates
     * @param Eigen::Vector3f with the new camera position
     */
    RETO_API
    void position( const Eigen::Vector3f& position_ );

    /**
     * Method to get camera up vector
     * @return Eigen::Vector3f with camera up vector
     */
    RETO_API
    Eigen::Vector3f up( void ) const;

    /**
     * Method to set camera up vector
     * @param Eigen::Vector3f with the new camera up vector
     */
    RETO_API
    void up( const Eigen::Vector3f& up_ );

    /**
     * Method to get camera look-at vector
     * @return Eigen::Vector3f with camera look-at vector
     */
    RETO_API
    Eigen::Vector3f lookAt( void ) const;

    /**
     * Method to set camera look-at vector
     * @param Eigen::Vector3f with the new camera look-at vector
     */
    RETO_API
    void lookAt( const Eigen::Vector3f& lookAt_ );

    /**
     * Method to get camera view matrix
     * @return Eigen::Matrix4f with camera view matrix
     */
    RETO_API
    Eigen::Matrix4f viewMatrix( void ) const;

    /**
     * Method to set camera view matrix
     * @param Eigen::Matrix4f with the new camera view matrix
     */
    RETO_API
    void viewMatrix( const Eigen::Matrix4f& viewMatrix_ );

    /**
     * Method to get camera projection matrix
     * @return Eigen::Matrix4f with camera projection matrix
     */
    RETO_API
    Eigen::Matrix4f projMatrix( void ) const;

    /**
     * Method to set camera projection matrix
     * @param Eigen::Matrix4f with the new camera projection matrix
     */
    RETO_API
    void projMatrix( const Eigen::Matrix4f& projMatrix_ );

    /**
     * Method to get camera view projection matrix: projMatrix * viewMatrix
     * @return Eigen::Matrix4f with camera view projection matrix
     */
    RETO_API
    Eigen::Matrix4f viewProjMatrix( void ) const;

    /**
     * Method to set camera view projection matrix: projMatrix * viewMatrix
     * @param Eigen::Matrix4f with the new camera view projection matrix
     */
    RETO_API
    void viewProjMatrix( const Eigen::Matrix4f& viewProjMatrix_ );

    /**
     * Method to get camera field of view (only if working with a perspective
     * one)
     * @return float with camera field of view
     */
    RETO_API
    float fov( void ) const;

    /**
     * Method to set camera field of view (only if working with a perspective
     * one)
     * @param float with the new camera field of view
     */
    RETO_API
    void fov( float fov_ );

#ifdef RETO_USE_ZEROEQ
    /**
     * Method to get the pointer to the camera subscriber used to synchronize
     * the camera with other applications
     * @return pointer to zeroeq::Subscriber
     */
    RETO_API
    zeroeq::Subscriber* subscriber( void );

    /**
     * Method to set camera view matrix represented as a vector of floats
     * @param std::vector< float > with the new camera view matrix represented as a vector of floats
     */
    void viewMatrixAsVector( const std::vector< float >& viewMatrixAsVector_ );

#endif

protected:

    //! Frustum width
    unsigned int _width;

    //! Frustum height
    unsigned int _height;

    //! Near plane z coordinate
    float _nearPlane;

    //! Far plane z coordinate
    float _farPlane;

    //! Position in global coordinates
    Eigen::Vector3f _position;

    //! Up vector
    Eigen::Vector3f _up;

    //! Look-at vector (from position to target)
    Eigen::Vector3f _lookAt;

    //! View matrix
    Eigen::Matrix4f _viewMatrix;

    //! Projection matrix
    Eigen::Matrix4f _projMatrix;

    //! View projection matrix
    Eigen::Matrix4f _viewProjMatrix;

    //! Field of view (only if working with a perspective camera)
    float _fov;

#ifdef RETO_USE_ZEROEQ
    //! State of the ZeroEQ connection: 1 activated, 0 deactivated
    bool _zeqConnection;

    //! ZeroEQ session to synchronize the camera with other applications
    std::string _zeroeqSession;

    //! ZeroEQ publisher
    zeroeq::Publisher* _publisher;

    //! ZeroEQ subscriber
    zeroeq::Subscriber* _subscriber;

    //! Thread that runs the ZeroEQ subscriber
    std::thread* _subscriberThread;

    //! View matrix represented as a vector of floats
    std::vector< float > _viewMatrixAsVector;

    //! Mutex to access to the camera view matrix
    std::mutex _viewMatrixMutex;

private:

    /**
     * Auxiliary method to transform a Eigen::Matrix4f into a vector of floats
     * @param Eigen::Matrix4f as the input matrix
     * @return std::vector< float > as the output vector of floats
     */
    std::vector< float > _matrix4fToVector( const Eigen::Matrix4f& matrix_ );

    /**
     * Auxiliary method to transform a vector of floats into a Eigen::Matrix4f
     * @param std::vector< float > as the input vector of floats
     * @return Eigen::Matrix4f as the output matrix
     */
    Eigen::Matrix4f _vectorToMatrix4f( const std::vector<float>& vector_  );

    /**
     * Method that triggers when a camera event occurs
     */
    void _OnCameraEvent( lexis::render::ConstLookOutPtr event_ );

#endif

  };

} // end namespace reto

#endif // __RETO_CAMERA__
