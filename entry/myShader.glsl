#version 430

out vec4 fragColor;

#pragma glsipy: MyPartial = require("./myPartial.glsl")
#pragma glsipy: Demo = require("./demo.glsl")

void main() {
  vec3 v = ident();

  fragColor = vec4(v, 1.0);
}

