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

#include <reto/api.h>

namespace reto
{
  class Pickable
  {
  public:
    Pickable( void );
    virtual ~Pickable ( void ); 

      /**
       * Method to update currentOffset
       * @param currentOffset: Current offset
       * @return Updated current offset
       */
      RETO_API
    virtual unsigned int sendId ( unsigned int currentOffset );

      /**
       * Method to render a Pickable object
       */
      RETO_API
    virtual void render ( void ) = 0;

  protected:
    int _numIds = 1;
    void setNumIDs ( int numIds );
  };
};

#endif // __RETO__PICKABLE__
