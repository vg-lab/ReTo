/*
 * Copyright (c) 2014-2016 GMRV/URJC.
 *
 * Authors: Cristian Rodriguez Bernal (crodriguez)
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

#include "Spline.h"
#include <stdexcept>
#include <iostream>
#include <math.h>

namespace reto
{
  Spline::Spline( const std::vector<Eigen::Vector3f>& points )
  {
    this->_points = points;
    this->_olddt = this->_currentdt = 0.0;
  }

  Eigen::Vector3f Spline::evaluate( float dt )
  {
    // Code based on
    //https://github.com/maldicion069/monkeybrush.js/blob/master/lib/library/maths/Spline.ts
    /*if (dt > 1.0f || dt < 0.0f)
    {
      throw std::invalid_argument( "received value not in range [0, 1]" );
    }*/
    if ( dt <= 0 )
    {
	    return this->_points.front( );
  	}
  	else if ( dt >= 1 )
  	{
      return this->_points.back( );
  	}

    this->_olddt = this->_currentdt;
    this->_currentdt = dt;

    float point = ( this->_points.size( ) - 1 ) * dt;
    unsigned int iniPoint = floor( point );
    float w = point - iniPoint;

    Eigen::Vector3f p0 = this->_points[ iniPoint == 0 ? iniPoint : iniPoint - 1 ];
    Eigen::Vector3f p1 = this->_points[ iniPoint ];
    Eigen::Vector3f p2 = this->_points[ iniPoint > this->_points.size( ) - 2 ?
                                    this->_points.size( ) - 1 : iniPoint + 1 ];
    Eigen::Vector3f p3 = this->_points[ iniPoint > this->_points.size( ) - 3 ?
                                    this->_points.size( ) - 1 : iniPoint + 2 ];

    return Eigen::Vector3f {
      this->_catmullRom(
        p0( 0 ), p1( 0 ), p2( 0 ), p3( 0 ), w
      ),
      this->_catmullRom(
        p0( 1 ), p1( 1 ), p2( 1 ), p3( 1 ), w
      ),
      this->_catmullRom(
        p0( 2 ), p1( 2 ), p2( 2 ), p3( 2 ), w
      )
    };
  }

  Eigen::Vector3f Spline::getTangent( void )
  {
    return this->getTangent( this->_olddt, this->_currentdt );
  }
  Eigen::Vector3f Spline::getTangent( float dt0, float dt1 )
  {
    Eigen::Vector3f p1 = this->evaluate( dt0 );
    Eigen::Vector3f p2 = this->evaluate( dt1 );
    Eigen::Vector3f rem = p2 - p1;
    rem.normalize( );
    return rem;
  }
  float Spline::angleBetweenPoints( void )
  {
    return this->angleBetweenPoints( this->_olddt, this->_currentdt );
  }

  float Spline::angleBetweenPoints( float dt0, float dt1 )
  {
    Eigen::Vector3f p1 = this->evaluate( dt0 );
    Eigen::Vector3f p2 = this->evaluate( dt1 );
    float angle = atan2( p2( 2 ) - p1( 2 ), p2( 0 ) - p1( 0 ) );
    return angle * M_PI / 180.0;
  }

  float Spline::_catmullRom( float p0, float p1, float p2, float p3, float t )
  {
      float
          v0 = ( p2 - p0 ) * 0.5,
          v1 = ( p3 - p1 ) * 0.5,
          t2 = t * t,
          t3 = t * t2;
      return ( 2.0 * p1 - 2.0 * p2 + v0 + v1 ) * t3 +
          ( -3.0 * p1 + 3.0 * p2 - 2.0 * v0 - v1 ) * t2 + v0 * t + p1;
  };
}
