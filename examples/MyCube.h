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

#ifndef __RETO__MYCUBE__
#define __RETO__MYCUBE__

#include <reto/reto.h>

class MyCube: public reto::Pickable
{
public:
  MyCube( float side );
  void render( void );
  virtual void render( reto::ShaderProgram* );
  void setModel( const std::vector<float> value )
  {
    this->model = value;
  }
  std::vector<float> model;
protected:
  unsigned int _vao;
  unsigned int _size;
};

#endif // __RETO__MYCUBE__
