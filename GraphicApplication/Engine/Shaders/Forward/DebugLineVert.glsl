


layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec4 vertColor;

layout(location = 0) out vec4 OutColor;



uniform mat4 projectionView;

void main()
{
    gl_Position = projectionView * vec4(vertPosition, 1.0);
    OutColor = vertColor;
}

