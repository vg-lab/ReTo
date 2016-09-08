#version 330 core

layout(location = 0) out vec4 ourColor;
in float pid;

uniform vec3 colorin;

float module(float x, float y) {
	return x - y * floor(x / y);
}

float packColor(vec3 color) {
	color *= 255.0;
	return color.r + color.g * 256.0 + color.b * 256.0 * 256.0;
}

vec3 unpackColor(float f) {
    vec3 color;
    color.b = floor(f / (256 * 256));
    color.g = floor((f - color.b * 256 * 256) / 256);
    color.r = floor(mod(f, 256.0));
    return color / 255.0;
}

void main( ) {
	vec3 cc = unpackColor(pid);
	float cid = packColor(cc);
	cid = round(cid);
	if (cid == pid) {
		ourColor = vec4(0.0, 1.0, 0.0, 1.0);
	}  else {
		ourColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
	if (pid >= 0)
		ourColor = vec4(0.0, 0.0, 0.0, 1.0);
}
