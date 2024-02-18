#include "Utils\DeferredLighting.glsl"

in vec2 TexCoord;
out vec4 OutColor;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D materialMap;
uniform sampler2D depthMap;

struct Camera
{
    vec3 position;
    mat4 invViewProjMatrix;
    mat4 viewProjMatrix;
};

uniform Camera camera;
uniform EnvironmentInfo environment;

void main()
{
    float gamma = 2.2f;

    FragmentInfo fragment = getFragmentInfo(TexCoord, albedoMap, normalMap, materialMap, depthMap, camera.invViewProjMatrix);
    vec3 viewDirection = normalize(camera.position - fragment.position);

    vec3 IBL = calculateIBL(fragment, viewDirection, environment, gamma);

    OutColor = vec4(IBL, 1.0f);
}