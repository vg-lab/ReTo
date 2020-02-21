#version 330 core

in vec3 inPos;
in vec3 inNormal;
in vec4 inColor;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec3 norm;
out vec4 color;

void main()
{
  mat3 normal = mat3(inverse(transpose(view * model)));
	norm = normal * inNormal;
  color = inColor;

	gl_Position =  proj * view * model * vec4 (inPos,1.0);
}
