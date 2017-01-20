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

#ifndef __RETO_PERSPECTIVECAMERA__
#define __RETO_PERSPECTIVECAMERA__

// Eigen
#include <Eigen/Dense>

#include <vector>
#include <iostream>
#include <string>
#include <chrono>

#include <reto/api.h>

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

#include "Camera.h"

namespace reto
{

  class PerspectiveCamera: public Camera
  {
  public:

    RETO_API
    PerspectiveCamera( float width_ = 1920.0f, float height_ = 1080.0f,
                       float nearPlane_ = 0.1f, float farPlane_ = 10000.0f,
                       float yaw_ = 0.0f, float pitch_ = 0.0f,
                       float fov_ = 45.0f,
                       Eigen::Vector3f pivot_ = Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
                       float radius_ = 1000.0f );

    #ifdef RETO_USE_ZEROEQ
    RETO_API
    PerspectiveCamera( const std::string& session_,
                   float width_ = 1920.0f, float height_ = 1080.0f,
                   float nearPlane_ = 0.1f, float farPlane_ = 10000.0f,
                   float yaw_ = 0.0f, float pitch_ = 0.0f,
                   float fov_ = 45.0f,
                   Eigen::Vector3f pivot_ = Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
                   float radius_ = 1000.0f
    #endif

    virtual ~PerspectiveCamera();

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

    /** BEGIN zeroeq_functions **/

    /**
     * Method to if there is a target pivot move the camera to this target pivot
     *@return boolean that is 1 if the camera is actually moving
     */
    RETO_API
    bool anim( void );

    /** END zeroeq_functions **/

    /** BEGIN getters & setters **/

    /**
     * Method to get the field of view of the camera
     * @return float with the field of view of the camera
     */
    RETO_API
    float fov( void );

    /**
     * Method to set the rotation of the camera
     * @param yaw camera rotation of y exe
     * @param pitch camera rotation of x exe
     */
    RETO_API
    void rotation( const float yaw_, const float pitch_ );

    /**
     * Method to get the actual pivot pin of the camera
     * @return Eige::Vector3f with the pivot pin of the camera
     */
    RETO_API
    Eigen::Vector3f pivot( void );

    /**
     * Method to set the camera pivot pin
     * @param pivot new camera pivot pin
     */
    RETO_API
    void pivot( Eigen::Vector3f pivot_ );

    /**
     * Method to get the actual distance between the camera and the pivot pin
     * aka camera radius
     * @return float with the distance between the camera and the pivot pin
     */
    RETO_API
    float radius( void );

    /**
     * Method to set the distance between the camera and the pivot pin
     * @param radius distance between the camera and the pivot pin
     */
    RETO_API
    void radius( const float radius_ );

    /** END getters & setters **/

    /** BEGIN getters & setters (zeroeq) **/

    #ifdef RETO_USE_ZEROEQ
    /**
     * Method to get the pointer to the camera subscriber used to sinchronize
     * the camera with other applications
     * @return pointer to zeroeq::Subscriber
     */
    RETO_API
    zeroeq::Subscriber* subscriber( void );
    #endif

    /**
     * Method to set the target pivot of the camera
     * @param targetPivot camera target pivot pin
     */
    RETO_API
    void targetPivot( Eigen::Vector3f targetPivot_ );

    /**
     * Method to set the target camera distance between the camera and the pivot
     * pin
     * @param targetRadius target camera distance between the camera and the
     * pivot pin
     */
    RETO_API
    void targetRadius( const float targetRadius_ );

    /**
     * Method to set the target pivot pin a nd the target camera distance
     * between the camera and this pivot pin
     * @param targetPivot camera target pivot pin
     * @param targetRadius target camera distance between the camera and the
     * pivot pin
     */
    RETO_API
    void targetPivotRadius( Eigen::Vector3f targetPivot_,
                            const float targetRadius_ );

    /**
     * Method to set the anim duration of the camera movement from actual pivot
     * and radius to the target pivot and radius
     */
    RETO_API
    void animDuration( const float animDuration_ );

    /** END getters & setters (zeroeq) **/

    private:

    RETO_API
    void _BuildViewMatrix( void );

    RETO_API
    void _BuildProjectionMatrix( void );

    RETO_API
    void _BuildViewProjectionMatrix( void );

    /** BEGIN auxiliar **/

    void _PositionVectorized( const std::vector<float>& positionVec_ );

    void _ViewMatrixVectorized( const std::vector<float>& viewVec_ );

    void _Rotation( const Eigen::Matrix3f& rotation_ );

    /** END auxiliar **/

#ifdef RETO_USE_ZEROEQ
    void _OnCameraEvent( lexis::render::ConstLookOutPtr event_ );
#endif

    //! Factor to calculate the camera projection matrix based on the camera
    //! far plane, near plane, field of view and ratio
    float _f;

    //! Camera fild of view
    float _fov;

    //! Camera pivot pin
    Eigen::Vector3f _pivot;

    //! Distance between the camera and the pivot pin
    float _radius;

    /** BEGIN attributes (zeroeq) **/

    //! Camera target pivot pin
    Eigen::Vector3f _targetPivot;

    //! Target distance between the camera an the pivot pin
    float _targetRadius;

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

#ifdef RETO_USE_ZEROEQ

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

    /** END attributes (zeroeq) **/

  };

} /* namespace reto */

#endif /* __RETO_PERSPECTIVECAMERA__ */
