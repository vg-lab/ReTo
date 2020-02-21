#include "SelectionSystem.h"

// OpenGL, GLEW, GLUT.
#include <GL/glew.h>
#ifdef Darwin
#define __gl_h_
#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#endif

namespace reto
{

  namespace SelectionSystem
  {

    RubberBand::RubberBand( const unsigned int& width,
      const unsigned int& height )
        : _color( Eigen::Vector4f::Zero(4) )
        , _lineWidth( 1.0f )
        , _width( width )
        , _height( height )
        , _program( new reto::ShaderProgram( ) )
    {
      create( );

      //For line and filling
      _program->loadFromText( _VertexCode( ), _FragmentCode( ) );
      _program->compileAndLink( );
      _program->autocatching( );

      //Create Transform Feedback
      _tf = new reto::TransformFeedback( _VertexCodeTransformFeedback( ),
        { "insideBS" }, GL_SEPARATE_ATTRIBS );
    }

    RubberBand::~RubberBand( void )
    {
      clear( );
    }

    void RubberBand::setColor( const Eigen::Vector4f& color )
    {
      _color = color;
      _program->use( );
      _program->sendUniform4v( "uColor", &color[ 0 ] );
    }

    void RubberBand::setLineWidth( const float& width )
    {
      _lineWidth = width;
    }

    void RubberBand::resize( const unsigned int& width,
      const unsigned int& height )
    {
      _width = width;
      _height = height;
    }

    void RubberBand::draw( void )
    {
      glBindVertexArray( _vao );

      _program->use( );

      //Line
      drawLine( );

      //Filling
      drawFilling( );
    }

    void RubberBand::mouseDown( const Point& point )
    {
      addStartPosition( point );
      generate( );
    }

    void RubberBand::mouseMove( const Point& point )
    {
      addEndPosition( point );
      generate( );
    }

    void RubberBand::mouseUp( const Point& point, float* viewProj )
    {
      addEndPosition( point );
      generate( );

      //Do the transform feedback with _startPoint & _endPoint
      _tf->program( )->use( );
      _tf->program( )->sendUniform4m( "viewProj", viewProj );
      _tf->program( )->sendUniform2v( "bsMin", _bsMin );
      _tf->program( )->sendUniform2v( "bsMax", _bsMax );
      _tf->draw( );

      //Remove rubberband
      clearPositions( );
    }

    reto::ShaderProgram* const& RubberBand::program( void ) const
    {
      return _program;
    }

    void RubberBand::addObject( reto::Pickable* object )
    {
      _tf->addObject( object );
    }

    void RubberBand::removeObject( reto::Pickable* object )
    {
      _tf->removeObject( object );
    }

    void RubberBand::create( void )
    {
      //Vars
      GLuint positionAttribIndex = 0;

      //Vertex Array
      glGenVertexArrays( 1, &_vao );
      glBindVertexArray( _vao );

      //Vertex Buffer
      glGenBuffers( 1, &_vbo );
      glBindBuffer( GL_ARRAY_BUFFER, _vbo );
      glEnableVertexArrayAttrib( _vao, positionAttribIndex );
      glVertexAttribPointer( positionAttribIndex, 2, GL_FLOAT, GL_FALSE, 0, 0 );

      glBindVertexArray( 0 );
    }

    void RubberBand::generate( void )
    {
      _positions = calculateRubberBandPositions( _startPoint, _endPoint );

      //Save min and max for transform feedback
      _bsMin = { _positions.at( 0 ), _positions.at( 1 ) };
      _bsMax = { _positions.at( 4 ), _positions.at( 5 ) };

      //Vertex Buffer
      glBindBuffer( GL_ARRAY_BUFFER, _vbo );
      glBufferData( GL_ARRAY_BUFFER, _positions.size( ) * sizeof( float ),
        &_positions[ 0 ], GL_DYNAMIC_DRAW );
    }

    void RubberBand::drawLine( void )
    {
      glLineWidth( _lineWidth );
      glDrawArrays( GL_LINE_LOOP, 0,
        static_cast< GLsizei >( _positions.size( ) / 2 ) );
      glLineWidth( 1.0f );
    }

    void RubberBand::drawFilling( void )
    {
      //Disable cullface, depth and enable blend
      glDisable( GL_CULL_FACE );
      glDisable( GL_DEPTH_TEST );
      glEnable( GL_BLEND );
      glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

      //Draw inside
      glDrawArrays( GL_TRIANGLE_FAN, 0,
        static_cast< GLsizei >(_positions.size( ) / 2 ) );

      //Disable blending and enable depth and cullface
      glDisable( GL_BLEND );
      glEnable( GL_DEPTH_TEST );
      glEnable( GL_CULL_FACE );
    }

    std::vector< float > RubberBand::normalize( const reto::Point& point ) const
    {
      return
      {
        2.0f * static_cast< float >( point.first ) / _width - 1.0f,
        2.0f * static_cast< float >( point.second ) / _height - 1.0f
      };
    }

    void RubberBand::addStartPosition( Point point )
    {
      Point p( point.first, _height - point.second );
      _startPoint = normalize( p );
      _endPoint = _startPoint;
    }

    void RubberBand::addEndPosition( Point point )
    {
      Point p( point.first, _height - point.second );
      _endPoint = normalize( p );
    }

    std::vector< float > RubberBand::calculateRubberBandPositions(
      const std::vector< float >& start, const std::vector< float >& end ) const
    {
      //Get min and max positions
      float minX, minY, maxX, maxY;
      if ( start[0] < end[0] )
      {
        minX = start[0];
        maxX = end[0];
      }
      else
      {
        minX = end[0];
        maxX = start[0];
      }
      if ( start[1] < end[1] )
      {
        minY = start[1];
        maxY = end[1];
      }
      else
      {
        minY = end[1];
        maxY = start[1];
      }

      // order is important to drawArrays {0,1,3,2}
      return { minX, minY, minX, maxY, maxX, maxY, maxX, minY };
    }

    void RubberBand::clearPositions( void )
    {
      _positions.clear( );
      _startPoint.clear( );
      _endPoint.clear( );
    }

    void RubberBand::clear( void )
    {
      glDeleteBuffers( 1, &_vbo );
      glDeleteVertexArrays( 1, &_vao );
    }

    std::string RubberBand::_VertexCode( void ) const
    {
      return std::string("#version 430\n"
        "layout (location = 0) in vec2 inPos;\n"
        "uniform vec4 uColor;\n"
        "out vec4 color;\n"
        "void main( ) {\n"
        "    gl_Position = vec4( inPos, 0.0, 1.0 );\n"
        "    color = uColor;\n"
        "}");
    }

    std::string RubberBand::_FragmentCode( void ) const
    {
      return std::string("#version 430\n"
      "in vec4 color;\n"
      "layout( location = 0 ) out vec4 outColor;\n"
      "void main( ) {\n"
      "  outColor = color;\n"
      "}\n");
    }

    std::string RubberBand::_VertexCodeTransformFeedback( void ) const
    {
      return std::string("#version 430\n"
        "layout( location = 0) in vec3 inPos;\n"
        "uniform vec2 bsMin;\n"
        "uniform vec2 bsMax;\n"
        "out float insideBS;\n"
        "uniform mat4 viewProj;\n"
        "uniform mat4 model;\n"

        "float inBS( in vec2 bbMin, in vec2 bbMax, in vec2 p )\n"
        "{\n"
        "  vec2 s = step( bbMin, p ) - step( bbMax, p );\n"
        "  return s.x * s.y;\n"
        "}\n"

        "void main( void )\n"
        "{\n"
        "  vec4 p = viewProj * model * vec4( inPos, 1.0 );\n"
        "  vec2 ndcSpace = p.xy / p.w;\n"
        "  insideBS = inBS( bsMin, bsMax, ndcSpace );\n"
        "}");
    }

    Lasso::Lasso( const unsigned int& width, const unsigned int& height )
      : _color( Eigen::Vector4f::Zero(4) )
      , _lineWidth( 1.0f )
      , _width( width )
      , _height( height )
      ,  _programLine( new reto::ShaderProgram( ) )
      ,  _programFilling( new reto::ShaderProgram( ) )
    {
      create( );

      //Lasso line (border)
      _programLine->loadFromText( _VertexCodeLine( ), _FragmentCodeLine( ) );
      _programLine->compileAndLink( );
      _programLine->autocatching( );

      //Lasso filling (inside)
      _programFilling->loadFromText( _VertexCodeFilling( ),
        _FragmentCodeFilling( ) );
      _programFilling->compileAndLink( );
      _programFilling->autocatching( );

      //Texture Config
      TextureConfig options;
      options.internalFormat = GL_RG8;
      options.format = GL_RG;
      options.type = GL_UNSIGNED_BYTE;
      options.border = 0;
      options.level = 0;
      options.wrapS = GL_CLAMP_TO_EDGE;
      options.wrapT = GL_CLAMP_TO_EDGE;
      options.minFilter = GL_NEAREST;
      options.magFilter = GL_NEAREST;

      //Framebuffer
      _fb = new Framebuffer2D(
         _VertexCodeFBQuad( ), _FragmentCodeFBQuad( ),
        { std::pair< GLuint, TextureConfig >( GL_COLOR_ATTACHMENT0, options ) },
        width, height );

      //Create Transform Feedback
      _tf = new reto::TransformFeedback( _VertexCodeTransformFeedback( ),
        { "insideBS" }, GL_SEPARATE_ATTRIBS );
    }

    Lasso::~Lasso( void )
    {
      clear( );
    }

    void Lasso::setColor( const Eigen::Vector4f& color )
    {
      _color = color;
      _programLine->use( );
      _programLine->sendUniform4v( "uColor", &color[ 0 ] );
      _fb->program( )->use();
      _fb->program( )->sendUniform4v( "uColor", &color[0] );
    }

    void Lasso::setLineWidth( const float& width )
    {
      _lineWidth = width;
    }

    void Lasso::resize( const unsigned int& width,
      const unsigned int& height )
    {
      _width = width;
      _height = height;
      _fb->resize( width, height );
    }

    void Lasso::draw( void )
    {
      glBindVertexArray( _vao );

      //Line
      _programLine->use( );
      drawLine( );

      //Filling
      //First pass
      _fb->bind( );
      glClearColor( 0.0f, 0.5f, 0.0f, 0.0f ); //watch out!
      glClear( GL_COLOR_BUFFER_BIT );
      glViewport(0, 0, _width, _height);
      _programFilling->use( );
      drawFilling( );
      _fb->unbind( );

      //Second pass
      glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
      glViewport(0, 0, _width, _height);

      _fb->bindAttachments( );
      _fb->program( )->use( );
      _fb->program( )->sendUniformi( "colorTex", 0 );
      glEnable( GL_BLEND );
      glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      glBlendEquation( GL_FUNC_ADD );
      _fb->draw( );
      glDisable( GL_BLEND );
    }

    void Lasso::mouseDown( const Point& point )
    {
      addPosition( point );
      generate( );
    }

    void Lasso::mouseMove( const Point& point )
    {
      //Same as mouseDown ( TODO: see if we should call one from another or join functions )
      addPosition( point );
      generate( );
    }

    void Lasso::mouseUp( const Point& point, float* viewProj )
    {
      addPosition( point );
      generate(  );

      //Do the transform feedback
      _tf->program( )->use( );
      _tf->program( )->sendUniform4m( "viewProj", viewProj );
      _tf->draw( );

      //Remove lasso
      clearPositions( );
    }

    reto::ShaderProgram* const& Lasso::programLine( void ) const
    {
      return _programLine;
    }

    reto::ShaderProgram* const& Lasso::programFilling( void ) const
    {
      return _programFilling;
    }

    void Lasso::addObject( reto::Pickable* object )
    {
      _tf->addObject( object );
    }

    void Lasso::removeObject( reto::Pickable* object )
    {
      _tf->removeObject( object );
    }

    void Lasso::create( void )
    {
      //Vars
      GLuint positionAttribIndex = 0;

      //Vertex Array
      glGenVertexArrays( 1, &_vao );
      glBindVertexArray( _vao );

      //Vertex Buffer
      glGenBuffers( 1, &_vbo );
      glBindBuffer( GL_ARRAY_BUFFER, _vbo );
      glEnableVertexArrayAttrib( _vao, positionAttribIndex );
      glVertexAttribPointer( positionAttribIndex, 2, GL_FLOAT, GL_FALSE, 0, 0 );

      glBindVertexArray( 0 );
    }

    void Lasso::generate( void )
    {
      //Vertex Buffer
      glBindBuffer( GL_ARRAY_BUFFER, _vbo );
      glBufferData( GL_ARRAY_BUFFER, _positions.size( ) * sizeof( float ),
        &_positions[ 0 ], GL_DYNAMIC_DRAW );
    }

    void Lasso::drawLine( void )
    {
      glLineWidth( _lineWidth );
      glDrawArrays( GL_LINE_LOOP, 0,
        static_cast< GLsizei >( _positions.size( ) / 2 ) );
      glLineWidth( 1.0f );
    }

    void Lasso::drawFilling( void )
    {
      glDisable( GL_CULL_FACE );
      //glDisable( GL_DEPTH_TEST );
      glEnable( GL_BLEND );
      glBlendFunc( GL_ONE, GL_ONE );
      glBlendEquation( GL_FUNC_ADD );

      //Draw
      glDrawArrays( GL_TRIANGLE_FAN, 0, static_cast< GLsizei >(
        _positions.size( ) / 2  ) );

      //Disable blending and enable depth and cullface
      glDisable( GL_BLEND );
      //glEnable( GL_DEPTH_TEST );
      glEnable( GL_CULL_FACE );
    }

    std::vector< float > Lasso::normalize( const reto::Point& point ) const
    {
      return
      {
        2.0f * static_cast< float >( point.first ) / _width - 1.0f,
        2.0f * static_cast< float >( point.second ) / _height - 1.0f
      };
    }

    void Lasso::addPosition( Point point )
    {
      Point p( point.first, _height - point.second );
      std::vector< float > fPoint = normalize( p );
      _positions.insert( _positions.end( ),fPoint.begin( ), fPoint.end( ) );
      /*_positions.insert(
          _positions.end( ),
          std::make_move_iterator( fPoint.begin( ) ),
          std::make_move_iterator( fPoint.end( ) )
        );*/
    }

    void Lasso::clearPositions( void )
    {
      _positions.clear( );
    }

    void Lasso::clear( void )
    {
      glDeleteBuffers( 1, &_vbo );
      glDeleteVertexArrays( 1, &_vao );
    }

    std::string Lasso::_VertexCodeLine( void ) const
    {
      return std::string("#version 430\n"
        "layout (location = 0) in vec2 inPos;\n"
        "uniform vec4 uColor;\n"
        "out vec4 color;\n"
        "void main( ) {\n"
        "    gl_Position =  vec4( inPos, 0.0, 1.0 );\n"
        "    color = uColor;\n"
        "}");
    }

    std::string Lasso::_FragmentCodeLine( void ) const
    {
      return std::string("#version 430\n"
      "in vec4 color;\n"
      "layout( location = 0 ) out vec4 outColor;\n"
      "void main( ) {\n"
      "  outColor = color;\n"
      "}\n");
    }

    std::string Lasso::_VertexCodeFilling( void ) const
    {
      return std::string("#version 430\n"
        "layout (location = 0) in vec2 inPos;\n"
        "void main( ) {\n"
        "    gl_Position = vec4( inPos, 0.0, 1.0 );\n"
        "}");
    }

    std::string Lasso::_FragmentCodeFilling( void ) const
    {
      return std::string("#version 430\n"
      "layout( location = 0 ) out vec2 outColor;\n"
      "void main( ) {\n"
      "  outColor = gl_FrontFacing ? vec2( 1.0 / 255.0, 0.0 ) : vec2( 0.0, 1.0 / 255.0 );\n"
      "}\n");
    }

    std::string Lasso::_VertexCodeFBQuad( void ) const
    {
      return std::string("#version 430\n"
        "layout( location = 0 ) in vec2 inPos;\n"
        "uniform vec4 uColor;\n"
        "out vec2 texCoord;\n"
        "out vec4 color;\n"
        "void main( void )\n"
        "{\n"
        "  gl_Position = vec4( inPos, 0.0, 1.0 );\n"
        "  color = uColor;\n"
        "  texCoord = ( inPos + vec2( 1.0, 1.0 ) ) / 2.0;\n"
        "}\n");
    }

    std::string Lasso::_FragmentCodeFBQuad( void ) const
    {
      return std::string("#version 430\n"
        "in vec2 texCoord;\n"
        "in vec4 color;\n"
        "uniform sampler2D colorTex;\n"
        "out vec4 outColor;\n"

        "void main( void )\n"
        "{\n"
        "  vec2 tex = vec4( texture( colorTex, texCoord ) ).rg;\n"
        "  float substract = tex.r - tex.g;\n"
        "  if ( substract == 0.0 )\n"
        "  {\n"
        "    discard;\n"
        "  }\n"
        "  else\n"
        "  {\n"
        "    if ( mod( floor( abs( substract) * 255.0 ), 2.0 ) == 0.0 )\n"
        "    {\n"
        "      outColor = color;\n"
        "    }\n"
        "    else\n"
        "    {\n"
        "      discard;\n"
        "    }\n"
        "  }\n"

        "}\n");
    }

    std::string Lasso::_VertexCodeTransformFeedback( void ) const
    {
      return std::string("#version 430\n"
        "layout( location = 0) in vec3 inPos;\n"
        "in vec2 texCoord;\n"
        "uniform sampler2D colorTex;\n"
        "uniform mat4 viewProj;\n"
        "uniform mat4 model;\n"
        "out float insideBS;\n"

        "void main( void )\n"
        "{\n"
        "  vec4 p = viewProj * model * vec4( inPos, 1.0 );\n"
        "  vec2 ndcSpace = p.xy / p.w;\n"
        "  vec2 texCoord = ( ndcSpace + vec2( 1.0, 1.0 ) ) / 2.0;\n"
        "  vec2 tex = vec4( texture( colorTex, texCoord ) ).rg;\n"
        "  float substract = tex.r - tex.g;\n"
        "  if ( substract == 0.0 )\n"
        "  {\n"
        "    insideBS = 0.0;\n"
        "  }\n"
        "  else\n"
        "  {\n"
        "    if ( mod( floor( abs( substract ) * 255.0 ), 2.0 ) == 0.0 )\n"
        "    {\n"
        "      insideBS = 1.0;\n"
        "    }\n"
        "    else\n"
        "    {\n"
        "      insideBS = 0.0;\n"
        "    }\n"
        "  }\n"
        "}\n");
    }

  }

}
