#version 330 core

out vec4 outColor;

in vec3 outPos;
in vec3 outNormal;

uniform sampler2D tex;

vec2 matcap(vec3 eye, vec3 normal)
{
  vec3 reflected = reflect(eye, normal);

  float m = 2.0 * sqrt(
      pow(reflected.x, 2.0) +
      pow(reflected.y, 2.0) +
      pow(reflected.z + 1.0, 2.0)
  );

  return reflected.xy / m + 0.5;
}

void main() {
  outColor = texture(tex, matcap(outPos, outNormal));
}
