#version 330 core

out vec4 outColor;

in vec3 norm;

void main() {
  outColor = vec4(norm, 1.0);
}
