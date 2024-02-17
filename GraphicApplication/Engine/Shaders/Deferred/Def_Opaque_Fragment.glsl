

struct VertexData
{
    vec2 TexCoord;
    vec3 Normal;
    vec4 Position;
    mat3 WorldNormal;
};

layout(location = 0) in VertexData fsin;

layout(location = 0) out vec4 OutAlbedo;
layout(location = 1) out vec4 OutNormal;
layout(location = 2) out vec4 OutMaterial;



struct UniformMaterialData
{
    vec4 Color;
    float Roughness;
    float Metallic;
    float Reflectance;
    vec4 EmissiveColor;
    float EmissiveMultiplier;
    float AlphaCutOff;

    float AlbedoMapFactor;
    float MetallicMapFactor;
    float RoughnessMapFactor;
    float NormalMapFactor;
    float EmissiveMapFactor;
    float AOMapFactor;
    
};



uniform sampler2D mapAlbedo;
uniform sampler2D mapAO;
uniform sampler2D mapRoughness;
uniform sampler2D mapMetallic;
uniform sampler2D mapEmissive;
uniform sampler2D mapNormal;

uniform vec3 cameraView;
uniform UniformMaterialData materialProperties;
uniform vec2 uvMultipliers;


vec3 calcNormal(vec2 texcoord, mat3 worldNormal, sampler2D normalMap)
{
    vec3 normal;
    normal.xy = texture(normalMap, texcoord).rg;
    normal.xy = 2.0 * normal.xy - 1.0;
    normal.z = sqrt(1.0 - dot(normal.xy, normal.xy));
    return worldNormal * normal;
}

vec3 DeGamma(vec3 color)
{
    return pow(color, vec3(2.2));
}

vec4 DeGamma(vec4 color)
{
    return vec4(DeGamma(color.xyz), color.w);
}

vec4 GetAlbedo()
{
    // if the albedo map is not being used just get the color
    if(materialProperties.AlbedoMapFactor < 0.05) {return materialProperties.Color;}

    // Otherwise mix both the color and the albedo map data
    vec4 param = texture(mapAlbedo, fsin.TexCoord);
    return (materialProperties.Color * (1.0 - materialProperties.AlbedoMapFactor)) + (DeGamma(param) * materialProperties.AlbedoMapFactor);

}

vec3 GetMetallic()
{
    if(materialProperties.MetallicMapFactor < 0.05)
    {
        return vec3(materialProperties.Metallic);
    }
    return vec3(materialProperties.Metallic  *(1.0 - materialProperties.MetallicMapFactor ) + (texture(mapMetallic, fsin.TexCoord).rgb * materialProperties.MetallicMapFactor).r  );
}

float GetRoughness()
{
    if(materialProperties.RoughnessMapFactor < 0.05)
    {
        return materialProperties.Roughness;
    }
    return (materialProperties.Roughness * (1.0 - materialProperties.RoughnessMapFactor)) + (materialProperties.RoughnessMapFactor *texture(mapRoughness, fsin.TexCoord).r);
}

float GetAO()
{
    if(materialProperties.RoughnessMapFactor < 0.05)
    {
        return 1.0;
    }
    return (1.0 - materialProperties.AOMapFactor) + (materialProperties.AOMapFactor * texture(mapAO, fsin.TexCoord).r);
}

vec3 GetEmissive(vec3 albedo)
{
    if (materialProperties.EmissiveMapFactor < 0.05)
    {
        return albedo * materialProperties.EmissiveMultiplier;
    }
    vec3 param = texture(mapEmissive, fsin.TexCoord).r * materialProperties.EmissiveColor.rgb;
    return (albedo * materialProperties.EmissiveMultiplier) + (DeGamma(param) * materialProperties.EmissiveMapFactor);
}



void main()
{
    vec2 TexCoord = uvMultipliers * fsin.TexCoord;
    vec3 viewDirection = fsin.Position.xyz - cameraView;
    //TexCoord = applyParallaxMapping(TexCoord, fsin.TBN * viewDirection, map_height, displacement, parallaxOcclusion);

    vec3 normal = normalize(fsin.Normal);

    if(materialProperties.NormalMapFactor > 0.04)
    {
        normal = calcNormal(TexCoord, fsin.WorldNormal, mapNormal);
    }

    vec3 albedo = GetAlbedo().rgb;
    float occlusion = GetAO();
    //float emmisive = GetEmissive(albedo);
    float emmisive = GetEmissive(albedo).r;
    float metallic = GetMetallic().r;
    float roughness = GetRoughness();

   // OutAlbedo = vec4(albedo, emmisive / (emmisive + 1.0f));
    OutAlbedo = vec4(albedo, emmisive / (emmisive + 1.0f));
    OutNormal = vec4(0.5f * normal + 0.5f, 1.0f);
    OutMaterial = vec4(occlusion, roughness, metallic, 1.0f);
}