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

#ifndef __RETO_PATH__
#define __RETO_PATH__

// Eigen
#include <Eigen/Dense>

#include <iostream>
#include <vector>

#include <reto/api.h>

namespace reto
{

  class Path
  {
  public:

    RETO_API
    enum TInterpolationMethod
    {
      LERP = 0,
      CATMULL_ROM
    };

    RETO_API
    Path( TInterpolationMethod interpolationMethod_ );

    RETO_API
    Path( const std::vector< Eigen::Vector3f >& positions_,
          const std::vector< Eigen::Vector3f >& lookAts_,
          const std::vector< Eigen::Vector3f >& ups_,
          TInterpolationMethod interpolationMethod_ );

    RETO_API
    virtual ~Path( void );

    RETO_API
    std::vector< Eigen::Vector3f > positions( void ) const;

    RETO_API
    std::vector< Eigen::Vector3f > lookAts( void ) const;

    RETO_API
    std::vector< Eigen::Vector3f > ups( void ) const;

    RETO_API
    bool empty( void );

    RETO_API
    unsigned int nodesSize( void );

    RETO_API
    void clearNodes( void );

    RETO_API
    void addNode( const Eigen::Vector3f& position_,
                  const Eigen::Vector3f& lookAt_,
                  const Eigen::Vector3f& up_ );

    RETO_API
    Eigen::Vector3f evaluatePosition( unsigned int originNodeId_,
                                      float t_ );

    RETO_API
    Eigen::Vector3f evaluateLookAt( unsigned int originNodeId_,
                                    float t_ );

    RETO_API
    Eigen::Vector3f evaluateUp( unsigned int originNodeId_,
                                float t_ );

    //RETO_API
    //Eigen::Matrix3f evaluateOrientation( unsigned int originNodeId_,
    //                                     float t_ );

  private:

    // Interpolation methods.
    float _lerp( float p1, float p2, float t );
    float _catmullRom( float p0, float p1, float p2, float p3, float t );

    std::vector< Eigen::Vector3f > _positions;
    std::vector< Eigen::Vector3f > _lookAts;
    std::vector< Eigen::Vector3f > _ups;
    TInterpolationMethod _interpolationMethod;

  };

}

#endif
