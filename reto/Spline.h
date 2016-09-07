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

#ifndef __RETO__SPLINE__
#define __RETO__SPLINE__

#include <reto/api.h>
#include <vector>
#include <Eigen/Dense>

namespace reto
{
  class Spline
  {
    public:
      Spline(const std::vector<Eigen::Vector3f>& points);
      // Time inside [0, 1]
      RETO_API
      Eigen::Vector3f evaluate(float dt);

      RETO_API
      Eigen::Vector3f getTangent();

      RETO_API
      float angleBetweenPoints();
      RETO_API
      float angleBetweenPoints(float dt0, float dt1);
    protected:
      float _catmullRom(float p0, float p1, float p2, float p3, float t);
      std::vector<Eigen::Vector3f> _points;

      float _currentdt;
      float _olddt;
  };
};

#endif /* __RETO__SPLINE__ */
