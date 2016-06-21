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
#ifndef __RETO__PICKABLE__
#define __RETO__PICKABLE__

#include <vector>
#include "ProgramShader.h"

#include <vector>
#include <set>
#include "Model.h"
#include "ProgramShader.h"

#include <reto/api.h>

namespace reto
{
  typedef struct {
    int x;
    int y;
  } Point;

  class Pickable
  {
  public:
    RETO_API
    Pickable();

    /**
     * Method to find front object in a specific point
     * @param point: Point
     * @param models: List of elements to render
     * @param vp: ModelView matrix
     * @return int: Indice that is visible
     */
    RETO_API
    int click(Point point, 
      const std::vector<reto::Model*>& models, std::vector<float>& vp);


    /**
     * Method to find front object in a specific area
     * @param minPoint: minPoint
     * @param maxPoint: maxPoint
     * @param models: List of elements to render
     * @param vp: ModelView matrix
     * @return std::set<unsigned int> Indices that objects are visibles
     */
    RETO_API
    std::set<unsigned int> area(Point minPoint, Point maxPoint, 
      const std::vector<reto::Model*>& models, std::vector<float>& vp);

    protected:
      reto::ProgramShader _program;
  };
}

#endif // __RETO__PICKABLE__
