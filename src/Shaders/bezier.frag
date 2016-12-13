#version 330

in VertexData{
    vec4 mColor;
    vec2 mTexCoord;
} VertexIn;

void main(void)
{
    gl_FragColor = VertexIn.mColor;
}
