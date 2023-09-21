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

#ifndef __RETO_ORBITAL_CAMERA_CONTROLLER__
#define __RETO_ORBITAL_CAMERA_CONTROLLER__

#include "AbstractCameraController.h"

#include <reto/api.h>

namespace reto
{
  /** \brief Class to control orbital camera
   * This class manage the movement, rotation, zoom and animation of orbital
   * camera
   *
   */
  class OrbitalCameraController: public AbstractCameraController
  {
  public:
      /** \brief OrbitalCameraController class constructor.
       * \param[in] camera_ Camera object.
       * \param[in] session ZeroEQ session name.
       * \param[in] subscriber Only if ZeroEQ enabled. ZeroEQ subscriber for null session.
       *
       */
    RETO_API
    OrbitalCameraController( Camera* camera_ = nullptr, const std::string zeqSession = std::string()
#ifdef RETO_USE_ZEROEQ
        , std::shared_ptr<zeroeq::Subscriber> subscriber = nullptr
#endif
        );
    
    RETO_API
    virtual ~OrbitalCameraController( void );

    RETO_API
    void localTranslate( const Eigen::Vector3f& translation_ );

    /**
     * Method to establish the pivot or orbital center for the camera.
     @param position_ new value to be assigned as pivot or orbital center.
     */
    RETO_API
    void position( const Eigen::Vector3f& position_ );

    /**
     * Method to obtain the pivot or orbital center.
     * @return position of the pivot or orbital center.
     */
    RETO_API
    Eigen::Vector3f position( void ) const;

  protected:
    RETO_API
    void _conformSetViewMatrix( const Eigen::Vector3f& position_,
                                const Eigen::Matrix3f& rotation_,
                                float radius_ );
  };
}

#endif
