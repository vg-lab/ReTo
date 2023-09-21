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

#ifndef __RETO_OBJPARSER__
#define __RETO_OBJPARSER__

#include <reto/api.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>

namespace reto
{
  //! Auxiliar struct from ObjParser
  struct Model
  {
    //! Container of raw vertices
    std::vector< float > vertices;
    //! Container of raw normals
    std::vector< float > normals;
    //! Container of raw indices
    std::vector< int > indices;
    //! Container of raw texCoords
    std::vector< float > texCoords;
    //! Container of raw tangents
    std::vector< float > tangents;
    //! Container of raw bitangents
    std::vector< float > bitangents;
  };
  //! Class to read obj mesh files
  class ObjParser
  {
  public:
    /**
     * ObjParser constructor
     */
    RETO_API
    ObjParser( void );
    
    /**
     * Load new obj file
     * @param filename: Wavefront OBJ file route.
     * @param calculateTangAndBi: Calculate tangents and
     *   bitangents for object.
     * @return Model object with parsed values.
     */
    RETO_API
    Model loadObj( const std::string& filename,
      bool calculateTangAndBi = false );
  protected:
    /*
      Load all file content
      @param std::string filename
      @return std::string
    */
    std::string loadFile( const std::string& filename );
    /*
      Split string by character separator
      @param string line
      @param char c
      @return std::vector<std::string>
    */
    std::vector< std::string > split( const std::string& s, char c );
    /*
      Check if string is a number
      @param string input
      @return bool
    */
    bool isNumeric( const std::string& input );
    /*
      Check if string is a float number
      @param string input
      @return bool
    */
    bool isFloat( const std::string& myString );
    /*
      Split string in a float vector
      @param string line
      @return std::vector<std::float>
    */
    std::vector< float > splitLineToFloats( std::string& line );
    /*
      Remove spaces from left and right of a string
      @param string line
      @return std::string
    */
    std::string trim( const std::string& str );
    /*
      Split string in a integer vector
      @param string line
      @return std::vector<std::int>
    */
    std::vector< int > splitFace( std::string& line );
  };
}

#endif /* __RETO_OBJPARSER__ */
