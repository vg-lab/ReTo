#include "Pickable.h"
namespace reto
{
  Pickable::Pickable() {
    _program.loadFromText(
  "#version 430\n"
  "layout (location = 0) in vec3 Position;\n"
  "uniform mat4 modelViewProj;\n"
  "void main() {\n"
  "    gl_Position = modelViewProj * vec4(Position,1.0);\n"
  "}",
  "#version 430\n"
  "out vec4 ourColor;\n"
  "uniform int id;\n"
  "vec3 unpackColor3(float f) {\n"
  "    vec3 color;\n"
  "    color.r = floor(f / 256.0 / 256.0);\n"
  "    color.g = floor((f - color.r * 256.0 * 256.0) / 256.0);\n"
  "    color.b = floor(f - color.r * 256.0 * 256.0 - color.g * 256.0);\n"
  "    // We have a vec3 with the 3 components in range [0..256]. We need to normalize it ...\n"
  "    return color / 255.0;\n"
  "}\n"
  "void main() {\n"
  "  ourColor = vec4(unpackColor3(float(id)), 1.0);\n"
  "}");
    _program.compileAndLink();
    _program.addUniform("modelViewProj");
    _program.addUniform("id");
  }

  int Pickable::click(Point point, 
    const std::vector<reto::Model*>& models, std::vector<float>& vp) 
  {
    int selected = -1;
    int WINDOW_HEIGHT = 500;

    // TODO: Render
    vp.size();
    int n = 0;
    for(Model* m: models) {
      _program.sendUniformi("id", n++);
      m->render();
    }

    GLubyte color[4];
    glReadPixels(point.x, WINDOW_HEIGHT - point.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
    unsigned int v = (unsigned int)(color[2] + color[1] * 256.0 + color[0] * 256.0 * 256.0);
    if(v < models.size()) {
      selected = v;
    }

    return selected;
  }
  std::set<unsigned int> Pickable::area(Point minPoint, Point maxPoint, 
    const std::vector<reto::Model*>& models, std::vector<float>& vp) 
  {
    std::set<unsigned int> ret;
    int WINDOW_HEIGHT = 500;

    // TODO: Render
    vp.size();
    int n = 0;
    for(Model* m: models) {
      _program.sendUniformi("id", n++);
      m->render();
    }

    GLubyte color[4];
    unsigned int v;
    for(int x = minPoint.x; x < maxPoint.x; x++) {
      for(int y = minPoint.y; y < maxPoint.y; y++) {
        glReadPixels(x, WINDOW_HEIGHT - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
        v = (unsigned int)(color[2] + color[1] * 256.0 + color[0] * 256.0 * 256.0);
        if(v < models.size()) {
          ret.insert(v);
        }
      }
    }

    return ret;
  }
}