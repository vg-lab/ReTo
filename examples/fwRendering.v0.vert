#version 330 core

in vec3 inPos;

//uniform mat4 modelViewProj;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform float id;

out float pid;

void main()
{
	pid = id;
	//gl_Position =  modelViewProj * vec4 (inPos,1.0);
	//gl_Position =  proj * view * model * vec4 (inPos,1.0);
	gl_Position = vec4(0.5, 0.5, 0.0, 1.0);
}
