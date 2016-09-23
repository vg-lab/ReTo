#version 330 core

in vec3 inPos;
in vec3 inNormal;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec3 norm;

void main()
{
  mat3 normal = mat3(inverse(transpose(view * model)));
	norm = normal * inNormal;

	gl_Position =  proj * view * model * vec4 (inPos,1.0);
}
