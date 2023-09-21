# ReTo - Render auxiliary Tools
(c) 2016. VG-Lab / URJC

www.vg-lab.es
vg-lab@vg-lab.es

## Introduction

ReTo provides the following tools:

* Camera
* Shader manager
* Picking functionality
* Object Loader (triangle/quad mode)
* Spline navigation
* TextureManager

## Dependencies

As required dependencies ReTo needs:
* OpenGL
* GLEW
* Eigen3

The following dependencies are optional:
* GLUT for some examples
* ZeroEQ/Lexis for publishing camera
* Boost's Unit Test Framework for unit testing.
* FreeImage for Texture2D using file images.

## Building

ReTo has been succesfully built and used on Ubuntu 14.04/16.04, Mac OSX
Yosemite and Windows 7/8 (Visual Studio 2013 Win64). The following steps
should be enough to build it:

```bash
git clone --recursive https://gitlab.vg-lab.es/nsviz/ReTo.git
mkdir ReTo/build && cd ReTo/build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

## Testing

After building ReTo, you can run some examples in bin directory. Some of the
examples need to find the shaders through an environment variable pointing to
the examples directory (available on both the source and installation trees).
For example, from the building directory, created as explained before, you can run:
~~~~~~~~~~~~~
RETO_SHADERS_PATH=../examples/ bin/ReToDemoCubes
RETO_SHADERS_PATH=../examples/ bin/ReToTextureReader ../examples/gmrv_grande.png
~~~~~~~~~~~~~

## Documentation

You can access the online API documentation generated from the source (using
[Doxygen](http://doxygen.org/)) in https://vg-lab.github.io/doc/ReTo/
