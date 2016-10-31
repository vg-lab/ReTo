#version 430

out vec4 fragColor;

vec3 ident() {
  return vec3(1.0);
}
float position(float);
vec3 normal(vec4 v);
float derivative(float t, float epsilon) {
  return 0.5 * (position(t+epsilon) - normal(vec4(vec3(t-epsilon), 1.0));
}

float position(float t) {
    return 0.5 * t * t - t + 1.0;
}
vec3 normal(vec4 n) {
    return n.xyz;
}

void main() {
  vec3 v = ident();

  fragColor = vec4(v, 1.0);
}

