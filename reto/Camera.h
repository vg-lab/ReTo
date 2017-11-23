/*
 * Copyright (c) 2014-2017 GMRV/URJC.
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
     * @param farPlane camera far plane
     * @param pivot camera rotation pivot
     * @param radius camera distance from the camera pivot
     * @param yaw camera rotation of y exe
     * @param pitch camera rotation of x exe
     */
    RETO_API
    Camera( float fov = 45.0f, float ratio = ((float)16)/9,
            float nearPlane = 0.1f, float farPlane = 10000.0f,
            Eigen::Vector3f pivot = Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
            float radius = 1000.0f, float yaw = 0.0f, float pitch = 0.0f );

#ifdef RETO_USE_LEXIS
    /**
     * Camera constructor
     * @param session ZeroEq session to synchronize the camera with other
     * applications
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
    Camera( const std::string& session, float fov = 45.0f,
            float ratio = ((float)16)/9, float nearPlane = 0.1f,
            float farPlane = 10000.0f,
            Eigen::Vector3f pivot = Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
            float radius = 1000.0f, float yaw = 0.0f, float pitch = 0.0f );
#endif

    /**
     * Default destructor
     */
    RETO_API
    ~Camera( void );

    /**
     * Method to translate the camera from the actual position
     * @param increment_ camera translation
     */
    RETO_API
    void localTranslation( Eigen::Vector3f increment_ );

    /**
     * Method to rotate the camera from the actual rotation
     * @param yaw camera rotation of y exe
     * @param pitch camera rotation of x exe
     */
    RETO_API
    void localRotation( const float yaw, const float pitch );

    /**
     * Method to if there is a target pivot move the camera to this target pivot
     *@return boolean that is 1 if the camera is actually moving
     */
    RETO_API
    bool anim( void );

    /**
     * Method to get the field of view of the camera
     * @return float with the field of view of the camera
     */
    RETO_API
    float fov( void );

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
     * Method to get the actual pivot pin of the camera
     * @return Eige::Vector3f with the pivot pin of the camera
     */
    RETO_API
    Eigen::Vector3f pivot( void );

    /**
     * Method to get the actual distance between the camera and the pivot pin
     * aka camera radius
     * @return float with the distance between the camera and the pivot pin
     */
    RETO_API
    float radius( void );

    /**
     * Method to get the pointer to projection matrix of the camera vectorized
     * in columns
     * @return pointer to float whit the column vectorized projection matrix of
     * the camera
     */
    RETO_API
    float* projectionMatrix( void );


    /**
     * Method to get the pointer to ortho-projection matrix of the camera vectorized
     * in columns
     * @return pointer to float whit the column vectorized ortho-projection
     * matrix of the camera
     */
    RETO_API
    float* orthoMatrix( void );

    /**
     * Method to get the pointer to view matrix of the camera vectorized
     * in columns
     * @return pointer to float whit the column vectorized view matrix of
     * the camera
     */
    RETO_API
    float* viewMatrix( void );

    /**
     * Method to get the pointer to projectionview matrix of the camera
     * vectorized in columns
     * @return pointer to float whit the column vectorized projectionview
     * matrix of the camera
     */
    RETO_API
    float* viewProjectionMatrix( void );

    /**
     * Method to get the pointer to the camera position vectorized
     * @return pointer to float whit the vectorized camera position
     */
    RETO_API
    float* position( void );

#ifdef RETO_USE_LEXIS
    /**
     * Method to get the pointer to the camera subscriber used to sinchronize
     * the camera with other applications
     * @return pointer to zeroeq::Subscriber
     */
    RETO_API
    zeroeq::Subscriber* subscriber( void );
#endif

    /**
     * Method to set the camera projection ratio
     * @param ratio camera projection ratio
     */
    RETO_API
    void ratio( float ratio );

    /**
     * Method to set the camera pivot pin
     * @param pivot new camera pivot pin
     */
    RETO_API
    void pivot( Eigen::Vector3f pivot );

    /**
     * Method to set the distance between the camera and the pivot pin
     * @param radius distance between the camera and the pivot pin
     */
    RETO_API
    void radius( const float radius );

    /**
     * Method to set the rotation of the camera
     * @param yaw camera rotation of y exe
     * @param pitch camera rotation of x exe
     */
    RETO_API
    void rotation( const float yaw, const float pitch );

    /**
     * Method to set the target pivot of the camera
     * @param targetPivot camera target pivot pin
     */
    RETO_API
    void targetPivot( Eigen::Vector3f targetPivot );

    /**
     * Method to set the target camera distance between the camera and the pivot
     * pin
     * @param targetRadius target camera distance between the camera and the
     * pivot pin
     */
    RETO_API
    void targetRadius( const float targetRadius );

    /**
     * Method to set the target pivot pin a nd the target camera distance
     * between the camera and this pivot pin
     * @param targetPivot camera target pivot pin
     * @param targetRadius target camera distance between the camera and the
     * pivot pin
     */
    RETO_API
    void targetPivotRadius( Eigen::Vector3f targetPivot,
                            const float targetRadius );

    /**
     * Method to set the anim duration of the camera movement from actual pivot
     * and radius to the target pivot and radius
     */
    RETO_API
    void animDuration( const float animDuration );

    /**
     * Method to set window size
     * @param width: Window width
     * @param height: Window height
     */
    RETO_API
    void setWindowSize( const int width, const int height );

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

private:
    void _PositionVectorized( const std::vector<float>& positionVec_ );

    void _Rotation( const Eigen::Matrix3f& rotation_ );

    void _ViewMatrixVectorized( const std::vector<float>& viewVec_ );

    void _BuildProjectionMatrix( void );

    void _BuildOrthoProjectionMatrix( void );

    void _BuildViewMatrix( void );

    void _BuildViewProjectionMatrix( void );

#ifdef RETO_USE_LEXIS
    void _OnCameraEvent( lexis::render::ConstLookOutPtr event_ );
#endif

    Eigen::Matrix3f _RotationFromPY( float yaw_, float pitch_ );

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

    //! Camera pivot pin
    Eigen::Vector3f _pivot;

    //! Distance between the camera and the pivot pin
    float _radius;

    //! Camera rotation matrix
    Eigen::Matrix3f _rotation;

    //! Vectorized camera position
    std::vector<float> _positionVec;

    //! Vectorized camera projection matrix
    std::vector<float> _projVec;

    //! Vectorized camera orthoprojection matrix
    std::vector<float> _orthoVec;

    //! Vectorized camera view matrix
    std::vector<float> _viewVec;

    //! Vectorized camera viewprojection matrix
    std::vector<float> _viewProjVec;

    //! Camera target pivot pin
    Eigen::Vector3f _targetPivot;

    //! Target distance between the camera an the pivot pin
    float _targetRadius;

#ifdef RETO_USE_LEXIS

    //! State of the zeq connection: 1 activated, 0 desactivated
    bool _zeqConnection;

    //! ZeroEQ session to sinchronize the camera with other apps
    std::string _zeroeqSession;

    //! ZeroEQ publisher
    zeroeq::Publisher* _publisher;

    //! ZeroEQ subscriber
    zeroeq::Subscriber* _subscriber;

    //! Mutex to access to the camera position
    std::mutex _positionMutex;

    //! Mutex to access to the camera rotation matrix
    std::mutex _rotationMutex;

    //! Mutex to accessto the camera view matrix
    std::mutex _viewMatrixMutex;

    //! Thread that runs the ZeroEQ subscriber
    std::thread* _subscriberThread;
#endif

    //! State of the camera animation
    bool _isAniming;

    //! Bool to check if the camera animation is in the first step
    bool _firstStep;

    //! Camera pivot animation speed
    float _speedPivot;

    //! Camera radius animation speed
    float _speedRadius;

    //! Camera animation duration in seconds
    float _animDuration;

    //! Register of the previus time in the animation loop
    std::chrono::time_point< std::chrono::system_clock > _previusTime;

    //! Window width
    int _width;

    //! Window height
    int _height;

  };
} //end namespace reto

#endif // __RETO_CAMERA__
