#version 330 core

out vec4 outColor;

in vec3 norm;
in vec4 color;

uniform vec4 uColor;

void main() {
  //outColor = vec4( norm, 1.0 );
  //outColor = color;
  outColor = uColor;
}
