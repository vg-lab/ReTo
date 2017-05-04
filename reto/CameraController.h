/*
 * Copyright (c) 2014-2017 GMRV/URJC.
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

#ifndef __RETO_CAMERACONTROLLER__
#define __RETO_CAMERACONTROLLER__

#include "Camera.h"
#include "Path.h"

#include <reto/api.h>
#include <Eigen/Dense>
#include <chrono>
#include <ctime>
#include <iostream>

namespace reto
{

  //! Camera controller
  /*!
    This class manages the camera. Basically it updates camera view and
    projection matrices. With this class the user can do the following
    actions with the camera: centering, translating, rotating,
    zooming, resizing and animating.
   */
  class CameraController
  {
  public:

    /**
     * Enumerator with different types of camera projection
     */
    RETO_API
    enum TProjection
    {
      PERSPECTIVE = 0,
      ORTHOGRAPHIC
    };

    /**
     * Enumerator with different camera implementations
     * STANDARD: FPC-like camera. (FPC = First Person Controller)
     * ORBITAL: Camera orbits around a defined pivot
     */
    RETO_API
    enum TCamera
    {
      STANDARD = 0,
      ORBITAL
    };

    /**
     * Camera controller constructor
     * @param camera the one controlled by this class
     * @param path the one that will be followed by the camera
     * @param projection camera projection type
     * @param cameraType camera implementation
     * @param pivot rotation pivot for an orbital camera
     * @param radius distance from pivot for an orbital camera
     * @param animationDuration time that camera spends on completing the path
     * @param tStep step of t parameter -feature interpolations between two nodes
     * belonging to a path-
     */
    RETO_API
    CameraController( Camera* camera_ = new Camera( ),
                      Path* path_ = new Path( ),
                      TProjection projection_ = PERSPECTIVE,
                      TCamera cameraType_ = STANDARD,
                      Eigen::Vector3f pivot_ = Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
                      float radius_ = 500.0f,
                      float animationDuration_ = 5.0f,
                      float tStep_ = 0.01f );

    /**
     * Default destructor
     */
    RETO_API
    virtual ~CameraController( void );

    /**
     * Method to get a pointer to the camera
     * @return Camera* as the pointer to the camera
     */
    RETO_API
    Camera* camera( void ) const;

    /**
     * Method to set the camera
     * @param Camera* as a pointer to the new camera
     */
    RETO_API
    void camera( Camera* camera_ );

    /**
     * Method to get a pointer to the path
     * @return Path* as the pointer to the path
     */
    RETO_API
    Path* path( void ) const;

    /**
     * Method to set the path
     * @param Path* as a pointer to the new path
     */
    RETO_API
    void path( Path* path_ );

    /**
     * Method to get projection type
     * @return TProjection with camera projection type
     */
    RETO_API
    TProjection projection( void ) const;

    /**
     * Method to get camera implementation
     * @return TCamera with camera implementation
     */
    RETO_API
    TCamera cameraType( void ) const;

    /**
     * Method to get camera rotation pivot (only if working with an orbital
     * one)
     * @return Eigen::Vector3f with camera rotation pivot
     */
    RETO_API
    Eigen::Vector3f pivot( void ) const;

    /**
     * Method to set camera rotation pivot (only if working with an orbital
     * one)
     * @param Eigen::Vector3f with the new camera rotation pivot
     */
    RETO_API
    void pivot( const Eigen::Vector3f& pivot_ );

    /**
     * Method to get camera distance from pivot (only if working with an orbital
     * one)
     * @return float with camera distance from pivot
     */
    RETO_API
    float radius( void ) const;

    /**
     * Method to set camera distance from pivot (only if working with an orbital
     * one)
     * @param float with the new camera field of view
     */
    RETO_API
    void radius( float radius_ );

    /**
     * Method to get animation duration
     * @return float with animation duration
     */
    RETO_API
    float animationDuration( void ) const;

    /**
     * Method to set animation duration
     * @param float with the new animation duration
     */
    RETO_API
    void animationDuration( float animationDuration_ );

    /**
     * Method to get parameter t step
     * @return float with parameter t step
     */
    RETO_API
    float tStep( void ) const;

    /**
     * Method to set parameter t step
     * @param float with the new parameter t step
     */
    RETO_API
    void tStep( float tStep_ );

    /**
     * Method to put the camera in a specific position and orientation
     * @param position new camera position
     * @param lookAt new camera look-at vector
     * @param up new camera up vector
     * @param pivot new camera rotation pivot (only if working with an orbital one)
     * @param radius new distance from camera to pivot (only if working with an orbital one)
     * @param yaw new mouse-like yaw (in degrees) (only if working with an orbital one)
     * @param pitch new mouse-like pitch (in degrees) (only if working with an orbital one)
     */
    RETO_API
    void center( const Eigen::Vector3f& position_ = Eigen::Vector3f( 0.0f, 0.0f, -500.0f ),
                 const Eigen::Vector3f& up_ = Eigen::Vector3f( 0.0f, 1.0f, 0.0f ),
                 const Eigen::Vector3f& lookAt_ = Eigen::Vector3f( 0.0f, 0.0f, 1.0f ),
                 const Eigen::Vector3f& pivot_ = Eigen::Vector3f( 0.0f, 0.0f, 0.0f ),
                 float radius_ = 500.0f, float yaw_ = 0.0f, float pitch_ = 0.0f );

    /**
     * Method to translate the camera using a vector
     * @param increment translation vector
     */
    RETO_API
    void translate( const Eigen::Vector3f& increment_ );

    /**
     * Method to translate the camera exclusively in look-at vector direction
     * @param increment unitary look-at vector multiplier
     */
    RETO_API
    void translateInLookAtVectorDirection( float increment_ );

    /**
     * Method to translate the camera exclusively in up vector direction
     * @param increment unitary up vector multiplier
     */
    RETO_API
    void translateInUpVectorDirection( float increment_ );

    /**
     * Method to translate the camera exclusively in right vector direction
     * @param increment unitary right vector multiplier
     */
    RETO_API
    void translateInRightVectorDirection( float increment_ );

    /**
     * Method to set camera local orientation
     * @param yaw mouse-like yaw (in degrees)
     * @param pitch mouse-like pitch (in degrees)
     */
    RETO_API
    void localOrientation( float yaw_, float pitch_ );

    /**
     * Method to rotate the camera locally
     * @param yaw mouse-like yaw delta (in degrees)
     * @param pitch mouse-like pitch delta (in degrees)
     */
    RETO_API
    void localRotation( float yaw_, float pitch_ );

    /**
     * Method to zoom in and out
     * @param increment if positive a zoom in will be done
     *                  if negative a zoom out will be effected
     */
    RETO_API
    void zoom( float increment_ );

    /**
     * Method to change screen aspect ratio
     * @param width new screen width
     * @param height new screen height
     */
    RETO_API
    void resize( float width_, float height_ );

    /**
     * Method that moves the camera along the path
     * @return bool indicating whether the camera has completed the path
     */
    RETO_API
    bool animate( void );

    /**
     * Method that triggers camera animation and initializes
     * some important animation attributes
     */
    RETO_API
    void triggerAnimation( void );

  private:

    /**
     * Method that generates the camera view matrix using its position,
     * look-at and up vectors
     * @param position camera position
     * @param lookAt camera look-at vector
     * @param up camera up vector
     * @return Eigen::Matrix4f with the generated camera view matrix
     */
    RETO_API
    Eigen::Matrix4f _lookAt( const Eigen::Vector3f& position_,
                             const Eigen::Vector3f& lookAt_,
                             const Eigen::Vector3f& up_ );

    /**
     * Method that generates the camera view matrix using its orientation
     * and up vector (only if working with an orbital camera)
     * @param orientation Eigen::Matrix3f with camera orientation
     * @param up camera up vector
     * @return Eigen::Matrix4f with the generated camera view matrix
     */
    RETO_API
    Eigen::Matrix4f _orbital( const Eigen::Matrix3f& orientation_,
                              const Eigen::Vector3f& up_ );

    /**
     * Method that generates a rotation matrix with yaw, pitch
     * and roll values
     * @param yaw rotation angle about the z-axis (in radians)
     * @param pitch rotation angle about the y-axis (in radians)
     * @param roll rotation angle about the x-axis (in radians)
     * @return Eigen::Matrix3f with the generated rotation matrix
     */
    RETO_API
    Eigen::Matrix3f _yawPitchRoll( float yaw_, float pitch_, float roll_ );

    //! Camera managed by this class
    Camera* _camera;

    //! Path followed by the camera
    Path* _path;

    //! Projection type
    TProjection _projection;

    //! Camera type
    TCamera _cameraType;

    //! Camera rotation pivot
    Eigen::Vector3f _pivot;

    //! Distance from camera to pivot
    float _radius;

    //! Animation attributes
    bool _isAniming;
    bool _animationFirstStep;
    float _animationSpeed;
    float _animationDuration;
    std::chrono::time_point< std::chrono::system_clock > _animationPreviousTime;
    unsigned int _currentNodeId;
    float _currentT;
    float _tStep;

  };

} // end namespace reto

#endif // __RETO_CAMERACONTROLLER__
