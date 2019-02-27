#ifndef __RETO__SSBO__
#define __RETO__SSBO__

#include <reto/api.h>

#ifndef __gl_h_
  #include <GL/glew.h>
#endif

namespace reto
{
  class SSBO
  {
  public:
    RETO_API
    SSBO( unsigned int size = 0, unsigned int type = GL_DYNAMIC_COPY );
    RETO_API
    virtual ~SSBO( void );
    RETO_API
    void bind( void );
    RETO_API
    void bind( unsigned int index );
    RETO_API
    void bufferSubData( int offset, unsigned int size, void* _data );
    RETO_API
    void data( unsigned int size, void* _data, unsigned int usage );
    RETO_API
    void* getData( void );
    RETO_API
    unsigned int getBuffer( void ) const;
    RETO_API
    static void unbind( void );
  protected:
    unsigned int _handler;
  };
}
#endif /* __RETO__SSBO__ */