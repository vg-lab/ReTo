/*
 * Copyright (c) 2014-2017 VG-Lab/URJC.
 *
 * Authors: Cristian Rodríguez Bernal <cristian.rodriguez@urjc.es>
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

#ifndef __RETO__MYCUBE__
#define __RETO__MYCUBE__

#include <reto/reto.h>

class MyCube: public reto::Pickable
{
public:
  MyCube( float side );
  void render( void );
  virtual void render( reto::ShaderProgram* );

  std::vector< float > getModel( void ) const;
  void setModel( const std::vector<float> value );

  std::vector< float > getPositions( void ) const;

  bool getSelected( void ) const;
  void setSelected( const bool& selected );

private:
  unsigned int _vao;
  unsigned int _size;
  unsigned int _handle[5];
  std::vector< float > _model;
  std::vector< float > _v;
  std::vector< float > _c;
  bool _selected;

};

#endif // __RETO__MYCUBE__
