/*
 * Copyright (c) 2014-2019 GMRV/URJC.
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

#ifndef __RETO_FREE_CAMERA_CONTROLLER__
#define __RETO_FREE_CAMERA_CONTROLLER__

#include "AbstractCameraController.h"

#include <reto/api.h>

namespace reto
{

  //! Class to control orbtial camera
  /*!
   * This class manage the movement, rotation, zoom and animation of free
   * camera
   */
  class FreeCameraController
    : public AbstractCameraController
  {

  public:

    RETO_API
    FreeCameraController( Camera* camera_ = nullptr );

    RETO_API
    ~FreeCameraController( void ) { };

    RETO_API
    void localTranslate( const Eigen::Vector3f& translation_ );

  protected:

    void _conformSetViewMatrix( const Eigen::Vector3f& position_,
                                const Eigen::Matrix3f& rotation_,
                                float radius_ );

  };
}

#endif
