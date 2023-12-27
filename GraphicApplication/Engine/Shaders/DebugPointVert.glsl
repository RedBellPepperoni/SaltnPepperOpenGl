#version 450



layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec4 vertColor;
layout(location = 2) in vec2 vertUV;

layout(location = 0) out VertexData
{
    vec4 color;
    vec2 uv;

} VertexOut;



uniform mat4 projectionView;

void main()
{
    gl_Position = projectionView * vec4(vertPosition, 1.0);

    VertexOut.color = vertColor;
    VertexOut.uv = vertUV;
}

