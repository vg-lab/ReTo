
#ifndef __RETO__MYCUBE__
#define __RETO__MYCUBE__

#include <reto/reto.h>

class MyCube: public reto::Pickable {
public:
  MyCube( float side );
  void render( );
  virtual void render( reto::ShaderProgram* );
protected:
  unsigned int _vao;
  unsigned int _size;
};

#endif // __RETO__MYCUBE__
