/*
 * Copyright (c) 2014-2016 GMRV/URJC.
 *
 * Authors: TBD
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

#ifdef RETO_USE_ZEROEQ
#include <zeroeq/zeroeq.h>
#include <servus/uri.h>

#include <mutex>
#include <boost/bind.hpp>

#ifdef RETO_USE_LEXIS
#include <lexis/lexis.h>
#endif

#ifdef RETO_USE_GMRVLEX
#include <gmrvlex/gmrvlex.h>
#endif

#include <thread>

#endif

namespace reto
{

  class Camera
  {

  public:

    RETO_API
    Camera( float fov_ = 45.0f, float ratio_ = ((float)16)/9,
            float nearPlane_ = 0.1f, float farPlane_ = 10000.0f,
            Eigen::Vector3f pivot_ = Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
            float radius_ = 1000.0f, float yaw_ = 0.0f, float pitch_ = 0.0f );

#ifdef RETO_USE_ZEROEQ
    RETO_API
    Camera( const std::string& session_, float fov_ = 45.0f,
            float ratio_ = ((float)16)/9, float nearPlane_ = 0.1f,
            float farPlane_ = 10000.0f,
            Eigen::Vector3f pivot_ = Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
            float radius_ = 1000.0f, float yaw_ = 0.0f, float pitch_ = 0.0f );
#endif

    RETO_API
    ~Camera(void);

    RETO_API
    void LocalRotation( float yaw_, float pitch_ );

    RETO_API
    bool Anim( void );

      // GETTERS

    RETO_API
    float Fov( void );

    RETO_API
    float& FarPlane( void )
    {
      return _farPlane;
    }

    RETO_API
    Eigen::Vector3f Pivot( void );

    RETO_API
    float Radius( void );

    RETO_API
    float* ProjectionMatrix( void );

    RETO_API
    float* ViewMatrix( void );

    RETO_API
    float* ViewProjectionMatrix( void );

    RETO_API
    float* Position( void );

#ifdef RETO_USE_ZEROEQ
    RETO_API
    zeroeq::Subscriber* Subscriber( void );
#endif

      // SETTERS
    RETO_API
    void Ratio( float ratio_ );

    RETO_API
    void Pivot( Eigen::Vector3f pivot_ );

    RETO_API
    void Radius( float radius_ );

    RETO_API
    void Rotation( float yaw_, float pitch_ );

    RETO_API
    void TargetPivot( Eigen::Vector3f targetPivot_ );

    RETO_API
    void TargetRadius( float targetRadius_ );

    RETO_API
    void TargetPivotRadius( Eigen::Vector3f targetPivot_,
                            float targetRadius_ );

    RETO_API
    void AnimDuration( float animDuration_ );

    private:
// TODO (Cristian): Harcoded
    void _PositionVectorized( const std::vector<float>& positionVec_ ) {
    	positionVec_.size();
    }
    void _Rotation( const Eigen::Matrix3f& rotation_ ) {
    	rotation_.size();
    }
    void _ViewMatrixVectorized( const std::vector<float>& viewVec_ ) {
    	viewVec_.size();
    }

    void _BuildProjectionMatrix( void );
    void _BuildViewMatrix( void );
    void _BuildViewProjectionMatrix( void );

#ifdef RETO_USE_ZEROEQ
    void _OnCameraEvent( lexis::render::ConstLookOutPtr event_ );
#endif

    Eigen::Matrix3f _RotationFromPY( float yaw_, float pitch_ );

    float _f;
    float _fov;
    float _ratio;
    float _nearPlane;
    float _farPlane;

    Eigen::Vector3f _pivot;
    float _radius;

    Eigen::Matrix3f _rotation;

    std::vector<float> _positionVec;
    std::vector<float> _projVec;
    std::vector<float> _viewVec;
    std::vector<float> _viewProjVec;

    Eigen::Vector3f _targetPivot;
    float _targetRadius;



#ifdef RETO_USE_ZEROEQ
    bool _zeqConnection;

    std::string _zeroeqSession;
    zeroeq::Publisher* _publisher;
    zeroeq::Subscriber* _subscriber;

    std::mutex _positionMutex;
    std::mutex _rotationMutex;
    std::mutex _viewMatrixMutex;

    std::thread* _subscriberThread;
#endif

    bool _isAniming;
    bool _firstStep;
    float _speedPivot;
    float _speedRadius;
    float _animDuration;

    std::chrono::time_point< std::chrono::system_clock > _previusTime;

  };


} //end namespace nlrender

#endif // __RETO_CAMERA__