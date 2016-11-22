#version 330 core

layout(location = 0) out vec4 ourColor;
in float pid;

float module(float x, float y) {
  return x - y * floor(x / y);
}

vec3 unpackColor(float f) {
  vec3 color;
  color.b = floor(f / (256 * 256));
  color.g = floor((f - color.b * 256 * 256) / 256);
  color.r = floor(module(f, 256.0));
  return color / 255.0;
}

void main( ) {
  vec3 cc = unpackColor(pid);
  ourColor = vec4(cc, 1.0);
}
