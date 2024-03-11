#include "Utils\ShaderUtils.glsl"
#include "Utils\PBRLightFunctions.glsl"



vec3 calculateIBL(FragmentInfo fragment, vec3 viewDirection, EnvironmentInfo environment, float gamma)
{
    float roughness = clamp(fragment.roughnessFactor, 0.05, 0.95);
    float metallic = clamp(fragment.metallicFactor, 0.05, 0.95);

    float NV = clamp(dot(fragment.normal, viewDirection), 0.0, 0.999);
    
    float lod = log2(textureSize(environment.skybox, 0).x * roughness * roughness);
    vec3 F0 = mix(vec3(0.04f), fragment.albedo, metallic);
    vec3 F = fresnelSchlickRoughness(F0, NV, roughness);
    
    vec3 prefilteredColor = calcReflectionColor(environment.skybox, environment.skyboxRotation, viewDirection, fragment.normal, lod);
    prefilteredColor = pow(prefilteredColor, vec3(gamma));
    vec2 envBRDF = texture(environment.envBRDFLUT, vec2(NV, 1.0 - roughness)).rg;
    vec3 specularColor = prefilteredColor * (F * envBRDF.x + envBRDF.y);

    vec3 irradianceColor = calcReflectionColor(environment.irradiance, environment.skyboxRotation, viewDirection, fragment.normal);
    irradianceColor = pow(irradianceColor, vec3(gamma));
    
    float diffuseCoef = 1.0f - metallic;
    vec3 diffuseColor = fragment.albedo * (irradianceColor - irradianceColor * envBRDF.y) * diffuseCoef;
    vec3 iblColor = (diffuseColor + specularColor) * environment.intensity;

    return fragment.emmisionFactor * fragment.albedo + iblColor * fragment.ambientOcclusion;
}

vec3 calculateLighting(FragmentInfo fragment, vec3 viewDirection, vec3 lightDirection, vec3 lightColor, float ambientFactor, float shadowFactor)
{
    float roughness = clamp(fragment.roughnessFactor, 0.05, 0.95);
    float metallic = clamp(fragment.metallicFactor, 0.05, 0.95);

    vec3 specularColor = vec3(0.0);
    vec3 diffuseColor = vec3(0.0);

    GGXCookTorranceSampled(fragment.normal, normalize(lightDirection), viewDirection, roughness, metallic, fragment.albedo, specularColor, diffuseColor);

    vec3 ambientColor = diffuseColor * ambientFactor;

    return ambientColor;

    float NL = clamp(dot(fragment.normal, lightDirection), 0.0, 1.0);

    float shadowCoef = NL * shadowFactor;

    vec3 totalColor = (ambientColor + (diffuseColor + specularColor) * shadowCoef) * lightColor;

    return totalColor * fragment.ambientOcclusion;
}




