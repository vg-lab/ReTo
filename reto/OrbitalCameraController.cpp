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

#include "OrbitalCameraController.h"

namespace reto
{
  OrbitalCameraController::OrbitalCameraController( Camera* camera_, const std::string zeqSession
#ifdef RETO_USE_ZEROEQ
      , std::shared_ptr<zeroeq::Subscriber> subscriber
#endif
      )
  : AbstractCameraController( camera_, zeqSession
#ifdef RETO_USE_ZEROEQ
      , subscriber
#endif
      )

  {}

  OrbitalCameraController::~OrbitalCameraController()
  {}

  void OrbitalCameraController::localTranslate(
    const Eigen::Vector3f& translation_ )
  {
    position( _position + _rotation.transpose( ) * translation_ );
  }

  void OrbitalCameraController::_conformSetViewMatrix(
    const Eigen::Vector3f& position_, const Eigen::Matrix3f& rotation_,
    float radius_ )
  {
    Eigen::Matrix3f rotationT = rotation_.transpose( );
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity( );
    view.block< 3, 3 >( 0, 0 ) = rotation_;

    Eigen::Vector3f pos = rotation_ *
      ( rotationT * Eigen::Vector3f( 0.0f, 0.0f, 1.0f ) * radius_ + position_ );

    view.block< 3, 1 >( 0, 3 ) = -pos;
    _setCameraViewMatrix( view );
  }

  void OrbitalCameraController::position( const Eigen::Vector3f& position_ )
  {
    AbstractCameraController::position( position_ );
  }

  Eigen::Vector3f OrbitalCameraController::position( void ) const
  {
    return AbstractCameraController::position( );
  }
}
