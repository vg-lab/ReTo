#!/usr/bin/python
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

import re, os, io

class ModuleData:
  def __init__(self, name, content):
      self.name = name.replace(" ", "")
      self.content = content
  def _str__(self):
    return "{\n\tName=" + self.name + ", \n\tContent=" + str(self.content) + "\n}"

class ShaderCompiler:
  items1 = [
    re.compile('#pragma glsipy: (.+?) = require\(\'(.+?)\'\)'),
    re.compile("#pragma glsipy: (.+?) = require\(\"(.+?)\"\)"),
    re.compile("#pragma glsipy: (.+?) = require\((.+?)\)")
  ]
  items2 = [
    re.compile('#pragma export\(\'(.+?)\'\)'),
    re.compile("#pragma export\(\"(.+?)\"\)"),
    re.compile("#pragma export\((.+?)\)")
  ]
  items3 = [
    re.compile('#pragma glsipy: export\(\'(.+?)\'\)'),
    re.compile("#pragma glsipy: export\(\"(.+?)\"\)"),
    re.compile("#pragma glsipy: export\((.+?)\)")
  ]

  import_modules = dict()

  def __match__(self, items, line):
    for item in items:
      v = re.match(item, line)
      if not v is None:
        return v
    return None

  def __format_key__(self, key):
    return key.replace("\"", "").replace('\'', "")

  def __format_values__(self, values):
    if(len(values) > 0):
      values = [x.replace(" ", "") for x in values]
      values = dict(s.split('=') for s in values)
    else:
      values = dict()
    return values

  def __init__(self, directories = []):
    self.directories = directories + ["./partials"]
    self.frag_repeat = []

  '''
  Generate a shader output file with entry shader
  @type file: string
  @param file: Shader file input
  @type exit_src: string
  @param exit_src: Shader file output
  @type minification: boolean
  @param minification: Minification file (Default: False)
  '''
  def gen_file(self, file, exit_src, minification = False):
    self.frag_repeat = []


    dir_path = os.path.dirname(os.path.realpath(os.getcwd() + "/" + file)) + "/"

    pt = os.path.dirname(os.path.abspath(file))
    print "dir_path=>" + dir_path
    print "file=>" + file
    print "pt=>" + pt
    #print "dir_path=>" + dir_path

    content = ''.join(self.__read_file__(True, dir_path, file, False))

    print ("GUARDO FICHERO: " + os.path.abspath(dir_path + "/" + exit_src))

    file_ = io.open(os.path.abspath(dir_path + "/" + exit_src), mode="w",  encoding='utf-8')
    #with open(exit_src, 'w') as file_:
    if minification is True:
      content = content.split("\n");
      file_.write(content[0] + "\n")
      del content[0]
      content = "".join(content).replace('\t', ' ')
    file_.write(u"{0}".format(content))
    file_.close( )

    '''
    pt = os.path.dirname(os.path.abspath(file))
    print "dir_path=>" + dir_path
    print "file=>" + file
    print "pt=>" + pt
    '''

  '''
  Returns a shader file content with all imports correct
  @type file: string
  @return Shader output content
  '''
  def __read_file__(self, is_rootfile, dir_path, file, is_module, mod = "", others = []):
    #print (os.path.exists(dir_path + "/" + file))
    #print ("ABRO FICHERO: " + os.path.abspath(dir_path + "/../" + file))
    #file = (dir_path + "/" + file)
    #if is_rootfile:
    file = os.path.abspath(dir_path + "/../" + file)
    #else:
    #  file = os.path.abspath(dir_path + "/" + file)
    #print file
    #return [file]
    #print mod
    if is_module:
      #print mod + "~>" + file
      if mod in ShaderCompiler.import_modules:
        #print(mod + " repeated")
        return ShaderCompiler.import_modules[mod].content
    with open(file, "r") as ins:
      array = []
      for line in ins:
        if line.startswith("#pragma glsipy:"):
          print line
          v = self.__match__(ShaderCompiler.items1, line)
          if not v is None:
            if not v.group(1) in self.frag_repeat:
              self.frag_repeat.append(v.group(1))
              values = v.group(2).split(",")

              key = self.__format_key__(values[0])
              if is_rootfile:
                key2 = key
                founded = False
                for route_dir in self.directories:
                  key2 = route_dir + "/" + key;
                  if os.path.exists(dir_path + "/../" + key2):
                    key = key2
                    founded = True
                    break
                if not founded:
                  raise Exception ("SHADER UNDEFINED")
                print (os.path.abspath(dir_path + "/../" + key))

              del values[0]
              pt = os.path.dirname(os.path.abspath(file))
              #print pt
              array += self.__read_file__(False, pt, key, True, v.group(1), self.__format_values__(values))
        elif line.startswith("#pragma export"):
          v_ = self.__match__(ShaderCompiler.items2, line)
          if not v_ is None:
            for k, v in others.items():
              array = [s.replace(k, v) for s in array]
        else:
          array.append(line)

      if is_module:
        v = self.__match__(ShaderCompiler.items3, line) # line value is the last value of file
        if not v is None:
          ShaderCompiler.import_modules[v.group(1)] = ModuleData(file, array)

      return array

# End of shaderimport.py
