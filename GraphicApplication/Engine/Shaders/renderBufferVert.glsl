#version 450 

layout(location = 0) in vec3 position;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(position.xy, 0.0f, 1.0f);
    TexCoord = position.xy * 0.5 + vec2(0.5);
}