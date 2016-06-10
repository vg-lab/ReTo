# ReTo - Render tools
(c) 2016. GMRV / URJC

www.gmrv.es
gmrv@gmrv.es

## Introduction

ReTo provides the following tools:

* Camera
* Shader manager
* Picking functionality

## Dependencies

TODO. Boost's Unit Test Framework are optional.

## Building

ReTo has been succesfully built and used on Ubuntu 14.04/16.04, Mac OSX
Yosemite and Windows 7/8 (Visual Studio 2013 Win64). The following steps
should be enough to build it:

```bash
git clone https://gitlab.gmrv.com/nsviz/ReTo.git
mkdir ReTo/build && cd ReTo/build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```
