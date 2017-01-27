#version 330

/* \brief Fragment GLSL shader that demonstrates how perform pass through fragment shader.
 * This file is a part of shader-3dcurve example (https://github.com/vicrucann/shader-3dcurve).

 * \author Victoria Rudakova
 * \date January 2017
 * \copyright MIT license
*/

in VertexData{
    vec2 mTexCoord;
    vec4 mColor;
} VertexIn;

void main(void)
{
    gl_FragColor = VertexIn.mColor;
}
