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

#include <reto/api.h>
#include <Eigen/Dense>
#include <vector>
#include <iostream>

namespace reto
{

  //! Path
  /*!
    This class represents a path made of nodes. Each node
    is defined by a position, a look-at vector and an up
    vector.
   */
  class Path
  {
  public:

    /**
     * Enumerator with different position interpolation methods
     * LERP: Linear interpolation
     * CATMULL_ROM: Interpolation based on a Catmull-Rom spline
     */
    RETO_API
    enum TInterpolationMethod
    {
      LERP = 0,
      CATMULL_ROM
    };

    /**
     * Path default constructor
     * @param interpolationMethod the one that will be applied to
     * interpolate between two positions
     */
    RETO_API
    Path( TInterpolationMethod interpolationMethod_ = LERP );

    /**
     * Path constructor
     * @param positions collection of positions that form the path
     * @param lookAts collection of look-at vectors
     * @param ups collection of up vectors
     * @param interpolationMethod the one that will be applied to
     * interpolate between two positions
     */
    RETO_API
    Path( const std::vector< Eigen::Vector3f >& positions_,
          const std::vector< Eigen::Vector3f >& lookAts_,
          const std::vector< Eigen::Vector3f >& ups_,
          TInterpolationMethod interpolationMethod_ = LERP );

    /**
     * Default destructor
     */
    RETO_API
    virtual ~Path( void );

    /**
     * Method to get the collection of positions
     * @return std::vector< Eigen::Vector3f > with all the positions belonging to the path
     */
    RETO_API
    std::vector< Eigen::Vector3f > positions( void ) const;

    /**
     * Method to get the collection of look-at vectors
     * @return std::vector< Eigen::Vector3f > with all the look-at vectors
     */
    RETO_API
    std::vector< Eigen::Vector3f > lookAts( void ) const;

    /**
     * Method to get the collection of up vectors
     * @return std::vector< Eigen::Vector3f > with all the up vectors
     */
    RETO_API
    std::vector< Eigen::Vector3f > ups( void ) const;

    /**
     * Method to get the position interpolation method used
     * @return TInterpolationMethod with the interpolation method
     */
    RETO_API
    TInterpolationMethod interpolationMethod( void ) const;

    /**
     * Method to set the position interpolation method
     * @param TInterpolationMethod with the new interpolation method
     */
    RETO_API
    void interpolationMethod( TInterpolationMethod interpolationMethod_ );

    /**
     * Method that checks if all collections are empty
     * @return bool true if all collections are empty
     *              false otherwise
     */
    RETO_API
    bool empty( void );

    /**
     * Method that returns the number of nodes existing in the path
     * @return unsigned int with the number of nodes
     */
    RETO_API
    unsigned int nodesSize( void );

    /**
     * Method that clears all the collections (positions, lookAts and ups)
     */
    RETO_API
    void clearNodes( void );

    /**
     * Method to add a new node to the path
     * @param position node position
     * @param lookAt node look-at vector
     * @param up node up vector
     */
    RETO_API
    void addNode( const Eigen::Vector3f& position_,
                  const Eigen::Vector3f& lookAt_,
                  const Eigen::Vector3f& up_ );

    /**
     * Method that returns an intermediate position between two nodes
     * @param originNodeId with values between 0 and (nodesSize - 1).
     * origin node index (destination node will be the one with the next index)
     * @param t with values between 0 and 1. Interpolation parameter
     * between origin and destination nodes
     * @return Eigen::Vector3f with the interpolated position
     */
    RETO_API
    Eigen::Vector3f evaluatePosition( unsigned int originNodeId_,
                                      float t_ );

    /**
     * Method that returns an intermediate look-at vector between two nodes
     * @param originNodeId with values between 0 and (nodesSize - 1).
     * origin node index (destination node will be the one with the next index)
     * @param t with values between 0 and 1. Interpolation parameter
     * between origin and destination nodes
     * @return Eigen::Vector3f with the interpolated look-at vector
     */
    RETO_API
    Eigen::Vector3f evaluateLookAt( unsigned int originNodeId_,
                                    float t_ );

    /**
     * Method that returns an intermediate up vector between two nodes
     * @param originNodeId with values between 0 and (nodesSize - 1).
     * origin node index (destination node will be the one with the next index)
     * @param t with values between 0 and 1. Interpolation parameter
     * between origin and destination nodes
     * @return Eigen::Vector3f with the interpolated up vector
     */
    RETO_API
    Eigen::Vector3f evaluateUp( unsigned int originNodeId_,
                                float t_ );

  private:

    /**
     * Interpolation method: Linear interpolation
     * @param p1 a coordinate of origin position vector
     * @param p2 a coordinate of destination position vector
     * @param t with values between 0 and 1. Interpolation parameter
     * @return float with the interpolated value for this coordinate (X, Y or Z)
     */
    float _lerp( float p1, float p2, float t );

    /**
     * Interpolation method: Based on a Catmull-Rom spline
     * @param p0 a coordinate of pre-origin position vector
     * @param p1 a coordinate of origin position vector
     * @param p2 a coordinate of destination position vector
     * @param p3 a coordinate of post-destination position vector
     * @param t with values between 0 and 1. Interpolation parameter
     * between p1 and p2
     * @return float with the interpolated value for this coordinate (X, Y or Z)
     */
    float _catmullRom( float p0, float p1, float p2, float p3, float t );

    //! Collection of positions
    std::vector< Eigen::Vector3f > _positions;

    //! Collection of look-at vectors
    std::vector< Eigen::Vector3f > _lookAts;

    //! Collection of up vectors
    std::vector< Eigen::Vector3f > _ups;

    //! Interpolation method between positions
    TInterpolationMethod _interpolationMethod;

  };

} // end namespace reto

#endif // __RETO_PATH__
