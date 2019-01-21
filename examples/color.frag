#version 330 core

out vec4 outColor;

in vec3 norm;

uniform vec3 color;

void main() {
  if( color.r == 1.0 && color.g == 1.0 && color.b == 1.0 )
  {
    outColor = vec4( 1.0, 0.0, 0.0, 1.0 );
  }
  else
  {
    outColor = vec4(norm, 1.0);
  }
}
