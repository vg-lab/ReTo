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

#include <reto/reto.h>
#include <iostream>
#include <vector>
#include <Eigen/Dense>

using namespace reto;

int main( void )
{
  std::vector<Eigen::Vector3f> v;
  v.push_back(Eigen::Vector3f{0.0, 0.0, 0.0});
  v.push_back(Eigen::Vector3f{0.0, 200.0, 0.0});
  v.push_back(Eigen::Vector3f{150.0, 150.0, 0.0});
  v.push_back(Eigen::Vector3f{150.0, 50.0, 0.0});
  v.push_back(Eigen::Vector3f{250.0, 100.0, 0.0});
  v.push_back(Eigen::Vector3f{250.0, 300.0, 0.0});
  v.push_back(Eigen::Vector3f{150.0, 50.0, 0.0});

  Spline sp(v);

  float dt = 0.0f;
  Eigen::Vector3f p;
  int tries = 2;
  while( tries > 0 )
  {
    p = sp.evaluate( dt );
    dt += 0.1f;
    std::cout << "dt = " << dt << " { x: " << p(0) << ", y: "
        << p(1) << ", z: " << p(2) << "}" << std::endl;
    if ( dt > 1.0f )
    {
      tries--;
      dt = 0.0f;
    }
  }

  return 0;
}
