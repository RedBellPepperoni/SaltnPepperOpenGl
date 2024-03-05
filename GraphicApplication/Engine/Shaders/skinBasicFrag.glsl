out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D mapAlbedo;

void main()
{    
    FragColor = texture(mapAlbedo, TexCoords);
}
