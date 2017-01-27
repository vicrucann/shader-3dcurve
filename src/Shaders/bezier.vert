#version 330

/* \brief Vertex GLSL shader that demonstrates how to draw basic thick and smooth bezier curves in 3D.
 * This file is a part of shader-3dcurve example (https://github.com/vicrucann/shader-3dcurve).
 *
 * \author Victoria Rudakova
 * \date January 2017
 * \copyright MIT license
*/

uniform mat4 ModelViewProjectionMatrix;

layout(location = 0) in vec4 Vertex;
layout(location = 1) in vec4 Color;

out VertexData{
    vec4 mColor;
    vec4 mVertex;
} VertexOut;

void main(void)
{
    VertexOut.mColor = Color;
    VertexOut.mVertex = Vertex;
    gl_Position = ModelViewProjectionMatrix * Vertex;
}
