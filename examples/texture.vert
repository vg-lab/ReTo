#version 330 core

in vec3 inPos;
in vec3 inNormal;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec3 outPos;
out vec3 outNormal;

void main()
{
  gl_Position =  proj * view * model * vec4 (inPos,1.0);
  outPos = vec3(view * model * vec4(inPos, 1.0));
  mat3 normal = mat3(inverse(transpose(view * model)));
  outNormal = normal * inNormal;
}
