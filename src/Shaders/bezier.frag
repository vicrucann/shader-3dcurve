#version 330

/* \brief Fragment GLSL shader that demonstrates how to apply the simplest fog effect
 * to your geometry. This file is a part of shader-3dcurve example (https://github.com/vicrucann/shader-3dcurve).
 * \author Victoria Rudakova
 * \date January 2017
 * \copyright MIT license
*/

uniform vec4 CameraEye;
uniform vec4 FogColor;
uniform bool IsFogged;

in VertexData{
    vec4 mColor;
    vec2 mTexCoord;
    vec4 mVertex;
} VertexIn;

float getFogFactor(float d)
{
    const float FogMax = 20.0;
    const float FogMin = 10.0;

    if (d>=FogMax) return 1;
    if (d<=FogMin) return 0;

    return 1 - (FogMax - d) / (FogMax - FogMin);
}

void main(void)
{
    float alpha = 1.f;
    if (IsFogged){
        vec4 V = VertexIn.mVertex;
        float d = distance(CameraEye, V);
        alpha = getFogFactor(d);
    }
    gl_FragColor = mix(VertexIn.mColor, FogColor, alpha);
}
