#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Copyright (c) 2014-2016 GMRV/URJC.

Authors: Cristian Rodríguez Bernal

This file is part of ReTo <https://gitlab.gmrv.es/nsviz/ReTo>

This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License version 3.0 as published
by the Free Software Foundation.

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
'''

import sys

from reto_shaders_import import *

ss = ShaderCompiler(["./others"])

def str2bool(v):
    return v.lower() in ("yes", "true", "t", "1", "True", "Yes", "T")

if __name__ == "__main__":

    if len(sys.argv) == 4:
        entry_src = sys.argv[1]
        exit_src = sys.argv[2]
        minification = str2bool(sys.argv[3])

        ss.gen_file(entry_src, exit_src, minification)
