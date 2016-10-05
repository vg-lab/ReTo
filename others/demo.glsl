#pragma glsipy: derivative = require('./others/derivative.glsl', func = position, func2 = normal)

float position(float t) {
    return 0.5 * t * t - t + 1.0;
}
vec3 normal(vec4 n) {
    return n.xyz;
}
#pragma glsipy: export(Demo)
