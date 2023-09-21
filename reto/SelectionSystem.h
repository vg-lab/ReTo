/*
 * Copyright (c) 2014-2019 VG-Lab/URJC.
 *
 * Authors: Gonzalo Bayo Martinez <gonzalo.bayo@urjc.es>
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


#ifndef __RETO__SELECTION_SYSTEM__
#define __RETO__SELECTION_SYSTEM__

//std
#include <vector>

//glew
#ifndef __gl_h_
  #include <GL/glew.h>
#endif

//reto
#include <reto/api.h>
#include "ShaderProgram.h"
#include "TransformFeedback.h"
#include "PickingSystem.h"
#include "Framebuffer.h"
#include "TextureManager.h"

namespace reto
{
  namespace SelectionSystem
  {

    /**
     * Class to manage rubberband selection
     * @class RubberBand
     */
    class RubberBand
    {

      public:

        /**
         * RubberBand constructor
         * @param width: screen width
         * @param height: screen height
         */
        RETO_API
        RubberBand( const unsigned int& width, const unsigned int& height );

        /**
         * Rubberband destructor
         */
        ~RubberBand( void );

        /**
         * Method to set the selection color
         * @param color: selection color
         */
        RETO_API
        void setColor( const Eigen::Vector4f& color );

        /**
         * Method to set the width of the border line of the selection
         * @param width: line size
         */
        RETO_API
        void setLineWidth( const float& width );

        /**
         * Method used on resize callback to change width and height for
         * calculating points
         * @param width: screen width
         * @param height: screen height
         */
        RETO_API
        void resize( const unsigned int& width, const unsigned int& height );

        /**
         * Method to draw rubberband
         */
        RETO_API
        void draw( void );

        /**
         * Method used on mouse down callback to get a point
         * @param point: first point of the selection
         */
        RETO_API
        void mouseDown( const Point& point );

        /**
         * Method used on mouse move callback to get the a point
         * @param point: last point of the selection (while moving)
         */
        RETO_API
        void mouseMove( const Point& point );

        /**
         * Method used on mouse up callback to get the a point, call to
         * transform feedback and clear points
         * @param point: last point of the selection
         * @param viewProj: view projection matrix
         */
        RETO_API
        void mouseUp( const Point& point, float* viewProj );

        /**
         * Method to get program handler for line and filling
         * @return program handler.
         */
        RETO_API
        reto::ShaderProgram* const& program( void ) const;

        /**
         * Method to add a pickable object
         * @param object: Pickable object
         */
        RETO_API
        void addObject( reto::Pickable* object );

        /**
         * Method to remove a pickable object
         * @param object: Pickable object
         */
        RETO_API
        void removeObject( reto::Pickable* object );

      private:

        //! Selection color
        Eigen::Vector4f _color;

        //! Selection border line width
        float _lineWidth;

        //! Vertex array handler
        GLuint _vao;

        //! Vertex buffer handler
        GLuint _vbo;

        //! Current width
        unsigned int _width;

        //! Current height
        unsigned int _height;

        //! Selection positions
        std::vector< float > _positions;

        //!Transform Feedback
        reto::TransformFeedback* _tf;

        //! Shader program for
        reto::ShaderProgram* _program;

        //! Selection starting position
        std::vector< float > _startPoint;

        //! Selection starting position
        std::vector< float > _endPoint;

        //! Lower left selection position
        std::vector< float > _bsMin;

        //! Upper right selection position
        std::vector< float > _bsMax;

        /**
         * Method to create buffers
         */
        void create( void );

        /**
         * Method to fill buffers
         */
        void generate( void );

        /**
         * Method to draw rubberband line
         */
        void drawLine( void );

        /**
         * Method to draw rubberband filling
         */
        void drawFilling( void );

        /**
         * Method to normalize a point
         * @param point: screen point.
         * @return normalized point.
         */
        std::vector< float > normalize( const reto::Point& point ) const;

        /**
         * Method to add the starting point of rubberband
         * @param point: screen point.
         */
        void addStartPosition( Point point );

        /**
         * Method to add the end point of rubberband
         * @param point: screen point.
         */
        void addEndPosition( Point point );

        /**
         * Method to calculate rubberband positions
         * @param start: starting rubberband selection point.
         * @param end: ending rubberband selection point.
         * @return rubberband positions.
         */
        std::vector< float > calculateRubberBandPositions(
          const std::vector< float >& start,
          const std::vector< float >& end ) const;

        /**
         * Method to clear rubberband positions
         */
        void clearPositions( void );

        /**
         * Method to clear rubberband
         */
        void clear( void );

        /**
         * Method to return line and filling vertex shader code
         * @return Vertex shader code.
         */
        std::string _VertexCode( void ) const;

        /**
         * Method to return line and filling fragment shader code
         * @return Fragment shader code.
         */
        std::string _FragmentCode( void ) const;

        /**
         * Method to return transform feedback vertex shader code
         * @return Vertex shader code.
         */
        std::string _VertexCodeTransformFeedback( void ) const;

    }; /* class RubberBand */

    /**
     * Class to manage lasso selection
     * @class Lasso
     */
    class Lasso
    {

      public:

        /**
         * Lasso constructor
         * @param width: screen width
         * @param height: screen height
         */
        RETO_API
        Lasso( const unsigned int& width, const unsigned int& height );

        /**
         * Lasso destructor
         */
        ~Lasso( void );

        /**
         * Method to set the selection color
         * @param color: selection color
         */
        RETO_API
        void setColor( const Eigen::Vector4f& color );

        /**
         * Method to set the width of the border line of the selection
         * @param width: line size
         */
        RETO_API
        void setLineWidth( const float& width );

        /**
         * Method used on resize callback to change width and height for
         * calculating points
         * @param width: screen width
         * @param height: screen height
         */
        RETO_API
        void resize( const unsigned int& width, const unsigned int& height );

        /**
         * Method to draw lasso
         */
        RETO_API
        void draw( void );

        /**
         * Method used on mouse down callback to get a point
         * @param point: first point of the selection
         */
        RETO_API
        void mouseDown( const Point& point );

        /**
         * Method used on mouse move callback to get the a point
         * @param point: next point of the selection
         */
        RETO_API
        void mouseMove( const Point& point );

        /**
         * Method used on mouse up callback to get the a point, call to
         * transform feedback and clear points
         * @param point: last point of the selection
         * @param viewProj: view projection matrix
         */
        RETO_API
        void mouseUp( const Point& point, float* viewProj );

        /**
         * Method to get program handler for line
         * @return program handler.
         */
        RETO_API
        reto::ShaderProgram* const& programLine( void ) const;

        /**
         * Method to get program handler for filling
         * @return program handler.
         */
        RETO_API
        reto::ShaderProgram* const& programFilling( void ) const;

        /**
         * Method to add a pickable object
         * @param object: Pickable object
         */
        RETO_API
        void addObject( reto::Pickable* object );

        /**
         * Method to remove a pickable object
         * @param object: Pickable object
         */
        RETO_API
        void removeObject( reto::Pickable* object );

      private:

        //! Selection color
        Eigen::Vector4f _color;

        //! Selection border line width
        float _lineWidth;

        //! Vertex array handler
        GLuint _vao;

        //! Vertex buffer handler
        GLuint _vbo;

        //! Current width
        unsigned int _width;

        //! Current height
        unsigned int _height;

        //! Selection positions
        std::vector< float > _positions;

        //!Transform Feedback
        reto::TransformFeedback* _tf;

        //! Shader program for line
        reto::ShaderProgram* _programLine;

        //! Shader program for filling
        reto::ShaderProgram* _programFilling;

        //! Framebuffer
        reto::Framebuffer2D* _fb;

        /**
         * Method to create buffers
         */
        void create( void );

        /**
         * Method to fill buffers
         */
        void generate( void );

        /**
         * Method to draw lasso line
         */
        void drawLine( void );

        /**
         * Method to draw lasso filling
         */
        void drawFilling( void );

        /**
         * Method to normalize a point
         * @param point: screen point.
         * @return normalized point.
         */
        std::vector< float > normalize( const reto::Point& point ) const;

        /**
         * Method to add a point to lasso positions
         * @param point: screen point.
         */
        void addPosition( Point point );

        /**
         * Method to clear lasso positions
         */
        void clearPositions( void );

        /**
         * Method to clear lasso
         */
        void clear( void );

        /**
         * Method to return line vertex shader code
         * @return Vertex shader code.
         */
        std::string _VertexCodeLine( void ) const;

        /**
         * Method to return line fragment shader code
         * @return Fragment shader code.
         */
        std::string _FragmentCodeLine( void ) const;

        /**
         * Method to return filling vertex shader code
         * @return Vertex shader code.
         */
        std::string _VertexCodeFilling( void ) const;

        /**
         * Method to return filling fragment shader code
         * @return Fragment shader code.
         */
        std::string _FragmentCodeFilling( void ) const;

        /**
         * Method to return quad vertex shader code
         * @return Vertex shader code.
         */
        std::string _VertexCodeFBQuad( void ) const;

        /**
         * Method to return quad fragment shader code
         * @return Fragment shader code.
         */
        std::string _FragmentCodeFBQuad( void ) const;

        /**
         * Method to return transform feedback vertex shader code
         * @return Vertex shader code.
         */
        std::string _VertexCodeTransformFeedback( void ) const;

    }; /* class Lasso */

  } /* namespace SelectionSystem */

} /* namespace reto */

#endif /* __RETO__SELECTION_SYSTEM__ */
