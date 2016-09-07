#version 330 core

in vec3 inPos;

uniform mat4 modelViewProj;
uniform float id;

out float pid;

void main()
{
	pid = id;
	gl_Position =  modelViewProj * vec4 (inPos,1.0);
}
