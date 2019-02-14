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

#ifndef __RETO__PICKABLE__
#define __RETO__PICKABLE__

#include <reto/api.h>
#include "ShaderProgram.h"

namespace reto
{
  class Pickable
  {
  public:
    RETO_API
    Pickable( void );
    RETO_API
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
    virtual void render ( reto::ShaderProgram* ) = 0;

    RETO_API
    virtual std::vector< float > getModel( void ) const = 0;

    RETO_API
    virtual std::vector< float > getPositions( void ) const = 0;

    RETO_API
    virtual bool getSelected( void ) const = 0;

    RETO_API
    virtual void setSelected( const bool& selected ) = 0;

    RETO_API
    int getId( void ) const;

    RETO_API
    void setId( const int& id );

  protected:
    int _numIds = 1;
    void setNumIDs( int numIds );

  private:
    int _id;
  };
};

#endif // __RETO__PICKABLE__
