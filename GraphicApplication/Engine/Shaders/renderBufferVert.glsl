#version 450 

layout(location = 0) in vec4 position;
//layout(location = 1) in vec2 aTexturecoords;

out vec2 TexCoord;

void main()
{
    gl_Position = position;
    TexCoord = position.xy * 0.5 + vec2(0.5);
    //gl_Position = vec4(position.xy,0.5,1.0);
    //TexCoord = aTexturecoords;


}