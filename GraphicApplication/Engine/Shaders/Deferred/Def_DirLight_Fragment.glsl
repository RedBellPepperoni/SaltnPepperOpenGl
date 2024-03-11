#include "Utils\DeferredLighting.glsl"

const int DirLightCascadeMapCount = 3;

out vec4 OutColor;
in vec2 TexCoord;



struct Camera
{
    vec3 position;
    mat4 invViewProjMatrix;
    mat4 viewProjMatrix;
};

struct DirLight
{
    mat4 transform[DirLightCascadeMapCount];
    vec4 color;
    vec3 direction;
};


uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D materialMap;
uniform sampler2D depthMap;

uniform Camera camera;

uniform DirLight dirlight;
uniform sampler2D lightDepthMap;

float calcShadowFactorCascade(vec4 position, DirLight light, sampler2D shadowMap)
{
    const vec2 TEXTURE_BIAS = vec2(0.001, 0.001);
    const float invCascadeCount = 1.0 / DirLightCascadeMapCount;
    vec3 depthBias = vec3(0.0005, 0.0003, 0.0002);
    for (int i = 0; i < DirLightCascadeMapCount; i++)
    {
        vec2 textureLimitsX = vec2(i, i + 1) * invCascadeCount + vec2(TEXTURE_BIAS.x, -TEXTURE_BIAS.x);
        vec4 textureLimitsXY = vec4(textureLimitsX, TEXTURE_BIAS.y, 1.0 - TEXTURE_BIAS.y);
        vec4 fragLightSpace = light.transform[i] * position;
        vec3 projectedPosition = fragLightSpace.xyz / fragLightSpace.w;
        float s = calcShadowFactor2D(projectedPosition, shadowMap, textureLimitsXY, depthBias[i]);
        if (s != -1.0) return s;
    }
    return 1.0;
}

void main()
{
    FragmentInfo fragment = getFragmentInfo(TexCoord, albedoMap, normalMap, materialMap, depthMap, camera.invViewProjMatrix);
    vec3 viewDirection = normalize(camera.position - fragment.position);

    vec3 totalColor = vec3(0.0);
    vec4 pos = vec4(fragment.position, 1.0);
   // float shadowFactor = calcShadowFactorCascade(pos, dirlight, lightDepthMap);
    float shadowFactor = 0.4;

    totalColor += calculateLighting(fragment, viewDirection, dirlight.direction, dirlight.color.rgb, 2.0f, shadowFactor);
    
    OutColor = vec4(totalColor, 1.0);
   
}