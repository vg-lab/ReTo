#include "MyCube.h"

#include <GL/glew.h>

#ifdef Darwin
  #define __gl_h_
  #define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <GL/freeglut.h>
#else
  #include <GL/gl.h>
  #include <GL/freeglut.h>
#endif

#include <vector>

MyCube::MyCube( float side )
{
    side /= 2;
    float v[24*3] = {
      // Front
     -side, -side, side,
      side, -side, side,
      side,  side, side,
     -side,  side, side,
     // Right
      side, -side, side,
      side, -side, -side,
      side,  side, -side,
      side,  side, side,
     // Back
     -side, -side, -side,
     -side,  side, -side,
      side,  side, -side,
      side, -side, -side,
     // Left
     -side, -side, side,
     -side,  side, side,
     -side,  side, -side,
     -side, -side, -side,
     // Bottom
     -side, -side, side,
     -side, -side, -side,
      side, -side, -side,
      side, -side, side,
     // Top
     -side,  side, side,
      side,  side, side,
      side,  side, -side,
     -side,  side, -side
    };

    float n[24*3] = {
      // Front
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      // Right
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      // Back
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      // Left
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      // Bottom
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      // Top
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f
    };

    float tex[24*2] = {
      // Front
      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
      // Right
      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
      // Back
      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
      // Left
      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
      // Bottom
      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
      // Top
      0.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f
    };

  GLuint el[] = {
    0,1,2,0,2,3,
    4,5,6,4,6,7,
    8,9,10,8,10,11,
    12,13,14,12,14,15,
    16,17,18,16,18,19,
    20,21,22,20,22,23
  };

  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);

  unsigned int handle[4];
  glGenBuffers(4, handle);

  _size = 36;

  glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
  glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(float), v, GL_STATIC_DRAW);
  glVertexAttribPointer( (GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);  // Vertex position

  glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
  glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(float), n, GL_STATIC_DRAW);
  glVertexAttribPointer( (GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);  // Vertex normal

  glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
  glBufferData(GL_ARRAY_BUFFER, 24 * 2 * sizeof(float), tex, GL_STATIC_DRAW);
  glVertexAttribPointer( (GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(2);  // texture coords

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, _size * sizeof(GLuint), el, GL_STATIC_DRAW);

  glBindVertexArray(0);
}
void MyCube::render() {
  glBindVertexArray(_vao);
  glDrawElements(GL_TRIANGLES, _size, GL_UNSIGNED_INT, 0);
}

void MyCube::render( reto::ShaderProgram* ss ) {
  //std::cout << this->model[12] << ", " << this->model[13] << ", " << this->model[14] << std::endl;
  ss->sendUniform4m("model", this->model.data( ));
  glBindVertexArray(_vao);
  glDrawElements(GL_TRIANGLES, _size, GL_UNSIGNED_INT, 0);
}
