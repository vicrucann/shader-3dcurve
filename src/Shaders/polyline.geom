#version 330

/* \brief Geometry GLSL shader that demonstrates how to draw basic thick and smooth lines in 3D.
 * This file is a part of shader-3dcurve example (https://github.com/vicrucann/shader-3dcurve).
 *
 * \author Victoria Rudakova
 * \date January 2017
 * \copyright MIT license
*/

uniform float Thickness;
uniform vec2 Viewport;
uniform float MiterLimit;

layout(lines_adjacency) in;
layout(triangle_strip, max_vertices = 7) out;

in VertexData{
    vec4 mColor;
} VertexIn[4];

out VertexData{
    vec2 mTexCoord;
    vec4 mColor;
} VertexOut;

vec2 toScreenSpace(vec4 vertex)
{
    return vec2( vertex.xy / vertex.w ) * Viewport;
}

float toZValue(vec4 vertex)
{
    return (vertex.z/vertex.w);
}

void drawSegment(vec2 points[4], vec4 colors[4], float zValues[4])
{
    vec2 p0 = points[0];
    vec2 p1 = points[1];
    vec2 p2 = points[2];
    vec2 p3 = points[3];

    /* perform naive culling */
    vec2 area = Viewport * 4;
    if( p1.x < -area.x || p1.x > area.x ) return;
    if( p1.y < -area.y || p1.y > area.y ) return;
    if( p2.x < -area.x || p2.x > area.x ) return;
    if( p2.y < -area.y || p2.y > area.y ) return;

    /* determine the direction of each of the 3 segments (previous, current, next) */
    vec2 v0 = normalize( p1 - p0 );
    vec2 v1 = normalize( p2 - p1 );
    vec2 v2 = normalize( p3 - p2 );

    /* determine the normal of each of the 3 segments (previous, current, next) */
    vec2 n0 = vec2( -v0.y, v0.x );
    vec2 n1 = vec2( -v1.y, v1.x );
    vec2 n2 = vec2( -v2.y, v2.x );

    /* determine miter lines by averaging the normals of the 2 segments */
    vec2 miter_a = normalize( n0 + n1 );	// miter at start of current segment
    vec2 miter_b = normalize( n1 + n2 ); // miter at end of current segment

    /* determine the length of the miter by projecting it onto normal and then inverse it */
    float an1 = dot(miter_a, n1);
    float bn1 = dot(miter_b, n2);
    if (an1==0) an1 = 1;
    if (bn1==0) bn1 = 1;
    float length_a = Thickness / an1;
    float length_b = Thickness / bn1;

    /* prevent excessively long miters at sharp corners */
    if( dot( v0, v1 ) < -MiterLimit ) {
        miter_a = n1;
        length_a = Thickness;

        /* close the gap */
        if( dot( v0, n1 ) > 0 ) {
            VertexOut.mTexCoord = vec2( 0, 0 );
            VertexOut.mColor = colors[1];
            gl_Position = vec4( ( p1 + Thickness * n0 ) / Viewport, zValues[1], 1.0 );
            EmitVertex();

            VertexOut.mTexCoord = vec2( 0, 0 );
            VertexOut.mColor = colors[1];
            gl_Position = vec4( ( p1 + Thickness * n1 ) / Viewport, zValues[1], 1.0 );
            EmitVertex();

            VertexOut.mTexCoord = vec2( 0, 0.5 );
            VertexOut.mColor = colors[1];
            gl_Position = vec4( p1 / Viewport, 0.0, 1.0 );
            EmitVertex();

            EndPrimitive();
        }
        else {
            VertexOut.mTexCoord = vec2( 0, 1 );
            VertexOut.mColor = colors[1];
            gl_Position = vec4( ( p1 - Thickness * n1 ) / Viewport, zValues[1], 1.0 );
            EmitVertex();

            VertexOut.mTexCoord = vec2( 0, 1 );
            VertexOut.mColor = colors[1];
            gl_Position = vec4( ( p1 - Thickness * n0 ) / Viewport, zValues[1], 1.0 );
            EmitVertex();

            VertexOut.mTexCoord = vec2( 0, 0.5 );
            VertexOut.mColor = colors[1];
            gl_Position = vec4( p1 / Viewport, zValues[1], 1.0 );
            EmitVertex();

            EndPrimitive();
        }
    }
    if( dot( v1, v2 ) < -MiterLimit ) {
        miter_b = n1;
        length_b = Thickness;
    }
    // generate the triangle strip
    VertexOut.mTexCoord = vec2( 0, 0 );
    VertexOut.mColor = colors[1];
    gl_Position = vec4( ( p1 + length_a * miter_a ) / Viewport, zValues[1], 1.0 );
    EmitVertex();

    VertexOut.mTexCoord = vec2( 0, 1 );
    VertexOut.mColor = colors[1];
    gl_Position = vec4( ( p1 - length_a * miter_a ) / Viewport, zValues[1], 1.0 );
    EmitVertex();

    VertexOut.mTexCoord = vec2( 0, 0 );
    VertexOut.mColor = colors[2];
    gl_Position = vec4( ( p2 + length_b * miter_b ) / Viewport, zValues[2], 1.0 );
    EmitVertex();

    VertexOut.mTexCoord = vec2( 0, 1 );
    VertexOut.mColor = colors[2];
    gl_Position = vec4( ( p2 - length_b * miter_b ) / Viewport, zValues[2], 1.0 );
    EmitVertex();

    EndPrimitive();
}

void main(void)
{
    // 4 points
    vec4 Points[4];
    Points[0] = gl_in[0].gl_Position;
    Points[1] = gl_in[1].gl_Position;
    Points[2] = gl_in[2].gl_Position;
    Points[3] = gl_in[3].gl_Position;

    // 4 attached colors
    vec4 colors[4];
    colors[0] = VertexIn[0].mColor;
    colors[1] = VertexIn[1].mColor;
    colors[2] = VertexIn[2].mColor;
    colors[3] = VertexIn[3].mColor;

    // screen coords
    vec2 points[4];
    points[0] = toScreenSpace(Points[0]);
    points[1] = toScreenSpace(Points[1]);
    points[2] = toScreenSpace(Points[2]);
    points[3] = toScreenSpace(Points[3]);

    // deepness values
    float zValues[4];
    zValues[0] = toZValue(Points[0]);
    zValues[1] = toZValue(Points[1]);
    zValues[2] = toZValue(Points[2]);
    zValues[3] = toZValue(Points[3]);

    drawSegment(points, colors, zValues);
}
