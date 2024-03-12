

out vec4 FragColor;
in vec3 TexCoords;

uniform samplerCube skybox;
uniform float luminance;

void main()
{
    vec3 skyboxColor = texture(skybox, TexCoords).rgb;
    skyboxColor = pow(skyboxColor, vec3(2.2f));
    skyboxColor = luminance * skyboxColor;
    FragColor = vec4(skyboxColor, 1.0f);
}