#include "Utils\DeferredLighting.glsl"

out vec4 OutColor;
in vec2 TexCoord;

struct Camera
{
    vec3 position;
    mat4 invViewProjMatrix;
    mat4 viewProjMatrix;
};

uniform sampler2D albedoTex;
uniform sampler2D normalTex;
uniform sampler2D materialTex;
uniform sampler2D depthTex;

uniform int lightCount;
uniform Camera camera;

uniform Light dirlight;
uniform sampler2D lightDepthMap;

void main()
{
    FragmentInfo fragment = getFragmentInfo(TexCoord, albedoTex, normalTex, materialTex, depthTex, camera.invViewProjMatrix);
    vec3 viewDirection = normalize(camera.position - fragment.position);

    vec3 totalColor = vec3(0.0);
    vec4 pos = vec4(fragment.position, 1.0);
    float shadowFactor = calcShadowFactorCascade(pos, dirlight, lightDepthMap);
    totalColor += calculateLighting(fragment, viewDirection, dirlight.direction, dirlight.color, dirlight.intensity, shadowFactor);
  
    OutColor = vec4(totalColor, 1.0);
}