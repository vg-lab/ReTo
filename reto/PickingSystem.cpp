#include "PickingSystem.h"
namespace reto
{
  PickingSystem::PickingSystem( reto::Camera* camera )
    : _camera(camera)
  {
    _program.loadFromText(
      getVertexCode( ),
      "#version 430\n"
      "out vec4 ourColor;\n"
      "in float pid;\n"
      "vec3 unpackColor3(float f) {\n"
      "    vec3 color;\n"
      "    color.r = floor(f / 256.0 / 256.0);\n"
      "    color.g = floor((f - color.r * 256.0 * 256.0) / 256.0);\n"
      "    color.b = floor(f - color.r * 256.0 * 256.0 - color.g * 256.0);\n"
      "    // We have a vec3 with the 3 components in range [0..256]. We need to normalize it ...\n"
      "    return color / 255.0;\n"
      "}\n"
      "void main() {\n"
      "  ourColor = vec4(unpackColor3(pid), 1.0);\n"
      "}");
    _program.compileAndLink();
    _program.addUniform("modelViewProj");
    _program.addUniform("id");
  }

  PickingSystem::~PickingSystem( void ) 
  {
    this->Clear();
  }

  int PickingSystem::click( Point point ) 
  {
    int selected = -1;
    int WINDOW_HEIGHT = 500;  // TODO: Hardcoded

    unsigned int currentId = 0;
    std::set< reto::Pickable* >::iterator it;
    for (it = _objects.begin(); it != _objects.end(); it++) 
    {
      currentId = (*it)->sendId( currentId );
      (*it)->render( );
    }

    GLubyte color[4];
    glReadPixels(point.x, WINDOW_HEIGHT - point.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
    unsigned int v = (unsigned int)(color[2] + color[1] * 256.0 + color[0] * 256.0 * 256.0);
    if(v < _objects.size())
    {
      selected = v;
    }

    return selected;
  }
  std::set<unsigned int> PickingSystem::area( Point minPoint, Point maxPoint ) 
  {
    std::set<unsigned int> ret;
    int WINDOW_HEIGHT = 500;  // TODO: Hardcoded

    unsigned int currentId = 0;
    std::set< reto::Pickable* >::iterator it;
    for (it = _objects.begin(); it != _objects.end(); it++) 
    {
      currentId = (*it)->sendId( currentId );
      (*it)->render( );
    }

    GLubyte color[4];
    unsigned int v;
    for(int x = minPoint.x; x < maxPoint.x; x++)
    {
      for(int y = minPoint.y; y < maxPoint.y; y++)
      {
        glReadPixels(x, WINDOW_HEIGHT - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
        v = (unsigned int)(color[2] + color[1] * 256.0 + color[0] * 256.0 * 256.0);
        if(v < _objects.size())
        {
          ret.insert(v);
        }
      }
    }

    return ret;
  }

  std::string PickingSystem::getVertexCode( )
  {
    return std::string("#version 430\n"
      "layout (location = 0) in vec3 Position;\n"
      "uniform mat4 modelViewProj;\n"
      "uniform int id;\n"
      "out float pid;\n"
      "void main() {\n"
      "    pid = float(id);\n"
      "    gl_Position = modelViewProj * vec4(Position,1.0);\n"
      "}");
  }

  void PickingSystem::AddObject( Pickable * p ) 
  {
    _objects.insert( p );
  }

  void PickingSystem::RemoveObject( Pickable *p )
  {
    _objects.erase( p );
  }

  void PickingSystem::Clear ( void )
  {
    _objects.clear( );
  }
}