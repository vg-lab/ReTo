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

#ifndef __RETO_CAMERA_ANIMATION__
#define __RETO_CAMERA_ANIMATION__

#include <Eigen/Dense>
#include <map>

#include <reto/api.h>

namespace reto
{
  class KeyCamera
  {

  public:
    RETO_API
    KeyCamera( float time_, const Eigen::Vector3f& position_,
               const Eigen::Matrix3f& rotation_,
               float radius_ = 0.0f );

    RETO_API
    KeyCamera( float time_, const Eigen::Vector3f& position_,
               const Eigen::Vector3f& rotationAngles_, float radius_ = 0.0f );

    RETO_API
    ~KeyCamera( void );

    RETO_API
    float time( void ) const;

    RETO_API
    Eigen::Vector3f position( void ) const;

    RETO_API
    Eigen::Matrix3f rotation( void ) const;

    RETO_API
    float radius( void ) const;

  protected:

    Eigen::Matrix3f _rotMatrixFromAngles(
      const Eigen::Vector3f& rotationAngles_ );

    float _time;

    Eigen::Vector3f _position;

    Eigen::Matrix3f _rotation;

    float _radius;

  };

  class CameraAnimation
  {

  public:

    typedef enum{
      NONE = 0,
      LINEAR
    } TInterpolation;

    RETO_API
    CameraAnimation( TInterpolation posInterpolation_ = LINEAR,
                     TInterpolation rotInterpolation_ = NONE,
                     TInterpolation radInterpolation_ = NONE );

    RETO_API
    ~CameraAnimation( void );

    RETO_API
    KeyCamera* getKeyCamera( float currentTime_ );

    RETO_API
    bool addKeyCamera( KeyCamera* keyCamera_ );

    RETO_API
    float startTime( void ) const;

    RETO_API
    float endTime( void ) const;

    RETO_API
    bool isPosAnimated( void ) const;

    RETO_API
    bool isRotAnimated( void ) const;

    RETO_API
    bool isRadAnimated( void ) const;

  protected:

    typedef Eigen::Vector3f
    (*TPosInterFunc)( Eigen::Vector3f, Eigen::Vector3f, float );

    typedef Eigen::Matrix3f
    (*TRotInterFunc)( Eigen::Matrix3f, Eigen::Matrix3f, float );

    typedef float (*TRadInterFunc)( float, float, float );

    static Eigen::Vector3f _linear( Eigen::Vector3f start_,
                                         Eigen::Vector3f end_,
                                         float alpha_ );

    static Eigen::Matrix3f _linear( Eigen::Matrix3f start_,
                                         Eigen::Matrix3f end_,
                                         float alpha_ );

    static float _linear( float start_, float end_, float alpha_ );

    std::map< float, KeyCamera* > _animation;

    float _startTime;

    float _endTime;

    TPosInterFunc _positionInterFunc;

    TRotInterFunc _rotationInterFunc;

    TRadInterFunc _radiusInterFunc;

  };
}

#endif
