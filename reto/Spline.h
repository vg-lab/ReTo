/*
 * Copyright (c) 2014-2017 GMRV/URJC.
 *
 * Authors: Cristian Rodríguez Bernal <cristian.rodriguez@urjc.es>
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

#ifndef __RETO__SPLINE__
#define __RETO__SPLINE__

#include <reto/api.h>
#include <vector>
#include <Eigen/Dense>

namespace reto
{
  /**
   * Class to manage spline structs
   * @class Spline
   */
  class Spline
  {
    public:
      /**
       * Create a new Spline object.
       * @param points Point list who define the spline.
       */
      RETO_API
      Spline( const std::vector< Eigen::Vector3f >& points );
      
      /**
       * Evaluate the spline in a specific time.
       * @param dt Time to evaluate. Must be in [0, 1] interval.
       * @return Interpolated position in dt time.
       */
      RETO_API
      Eigen::Vector3f evaluate( float dt );
      
      /**
       * Returns tangent vector in last time evaluate called.
       * @return Intepolated tangent in dt time.
       */
      RETO_API
      Eigen::Vector3f getTangent( void );
      
      /**
       * Returns tangent vector between between two times given.
       * @param dt0 Last time
       * @param dt1 Current time
       * @return Interpolated tangent in position.
       */
      RETO_API
      Eigen::Vector3f getTangent( float dt0, float dt1 );
      
      /**
       * Returns interpolated angle between last and current time.
       * @return angle (in radians).
       */
      RETO_API
      float angleBetweenPoints( void );
      
      /**
       * Returns interpolated angle between last and current time.
       * @param dt0 Last time
       * @param dt1 Current time
       * @return angle (in radians).
       */
      RETO_API
      float angleBetweenPoints( float dt0, float dt1 );
    protected:
      /**
       * Evaluate catmull rom spline with 4 given values and time
       * @param p0 First value
       * @param p1 Second value
       * @param p2 Third value
       * @param p3 Fourth value
       * @param t Time to evaluate
       * @return Interpolated value
       */
      float _catmullRom( float p0, float p1, float p2, float p3, float t );

      //! List of points who defineds the spline
      std::vector< Eigen::Vector3f > _points;
      //!Current time used (default = 0.0f)
      float _currentdt;
      //! Last time used (default = 0.0f)
      float _olddt;
  }; // class Spline
}; // namespace reto

#endif // __RETO__SPLINE__
