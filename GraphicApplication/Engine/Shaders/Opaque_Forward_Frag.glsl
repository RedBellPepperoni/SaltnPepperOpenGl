#include "Utils/LightingFunctions.glsl"

struct VertexData
{
	vec4 Position;
	vec3 Normal;
	vec2 TexCoord;
	mat3 WorldNormal;
};


struct UniformMaterialData
{
    vec4 AlbedoColor;
    float Roughness;
    float Metallic;
    float Reflectance;
    float Emissive;
    float AlphaCutOff;

    float AlbedoMapFactor;
    float MetallicMapFactor;
    float RoughnessMapFactor;
    float NormalMapFactor;
    float EmissiveMapFactor;
    float AOMapFactor;


    
};


const int MAX_LIGHT_COUNT = 32;

struct UBOLight
{
    Light lights[MAX_LIGHT_COUNT];

    mat4 ShadowTransform[4];
    mat4 ViewMatrix;
    mat4 LightMatrix;
    mat4 BiasMatrix;
    float LightSize;

    float ShadowFade;
    float CascadeFade;

    float InitialBias;
    float Width;
    float Height;

    int lightCount;

    int castShadow;

};



layout(location = 0) in VertexData VertexOutput;
layout(location = 0) out vec4 FragColor;

uniform sampler2D mapAlbedo;
uniform sampler2D mapAO;
uniform sampler2D mapRoughness;
uniform sampler2D mapMetallic;
uniform sampler2D mapEmissive;
uniform sampler2D mapNormal;
uniform sampler2DArray mapShadow;


uniform int renderMode = 0;
uniform vec3 cameraView;
uniform UBOLight uboLights;
uniform UniformMaterialData materialProperties;
uniform SkyboxInfo skyboxProperties;
// The golden Ratio
const float PHI = 1.6180339;

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
    if(materialProperties.AlbedoMapFactor < 0.1)
    {
       return materialProperties.AlbedoColor;
    }

    vec4 param = texture(mapAlbedo, VertexOutput.TexCoord);

    return (materialProperties.AlbedoColor * (1.0 - materialProperties.AlbedoMapFactor)) + (DeGamma(param) * materialProperties.AlbedoMapFactor);

}

vec3 GetMetallic()
{
    if(materialProperties.MetallicMapFactor < 0.1)
    {
        return vec3(materialProperties.Metallic);
    }

    return vec3((1.0 - materialProperties.MetallicMapFactor) * materialProperties.Metallic) + (texture(mapMetallic, VertexOutput.TexCoord).xyz * materialProperties.MetallicMapFactor);
}

float GetRoughness()
{
    if(materialProperties.RoughnessMapFactor < 0.1)
    {
        return materialProperties.Roughness;
    }

    return ((1.0 - materialProperties.RoughnessMapFactor) * materialProperties.Roughness) + (materialProperties.RoughnessMapFactor * texture(mapRoughness, VertexOutput.TexCoord).x);
}

float GetAO()
{
    if(materialProperties.RoughnessMapFactor < 0.1)
    {
        return 1.0;
    }

    return (1.0 - materialProperties.AOMapFactor) + (materialProperties.AOMapFactor * texture(mapAO, VertexOutput.TexCoord).x);
}

vec3 GetEmissive(vec3 albedo)
{
    if (materialProperties.EmissiveMapFactor < 0.1)
    {
        return albedo * materialProperties.Emissive;
    }

    vec3 param = texture(mapEmissive, VertexOutput.TexCoord).xyz;
    return (albedo * materialProperties.Emissive) + (DeGamma(param) * materialProperties.EmissiveMapFactor);
}



vec3 CalculateLighting(Material material)
{
    vec3 result = vec3(0.0);
    Light light;
    int actualcount = MAX_LIGHT_COUNT > uboLights.lightCount ? uboLights.lightCount : MAX_LIGHT_COUNT;
    
    for (int index = 0; index < actualcount; index++)
    {
        light.color = uboLights.lights[index].color;
        light.position = uboLights.lights[index].position;
        light.direction = uboLights.lights[index].direction;
        light.intensity = uboLights.lights[index].intensity;
        light.type = uboLights.lights[index].type;
        light.innerAngle = uboLights.lights[index].innerAngle;
        light.outerAngle = uboLights.lights[index].outerAngle;
        light.radius = uboLights.lights[index].radius;

        //vec3 normal = texture(mapNormal, VertexOutput.TexCoord).rgb;
        vec3 normal = (VertexOutput.Normal.xyz);
        vec3 position = VertexOutput.Position.xyz;
        // Directional Light
        if(light.type == 0)
        {
           result += CalculateDirectionalLight(light, material, normal);
        }

        // PointLight
        else if(light.type == 2)
        {
           result += CalculatePointLight(light, material, normal,position);
        }
        // spotlight
        else if(light.type == 1)
        {
            result += CalculateSpotLight(light, material, normal,position);
        }
        
    }

    
    return result;
    

}



void main()
{
    // gather the calculated Albedo color
    vec4 texColor = GetAlbedo();

    if(texColor.w < 0.2)
    {
        discard;
    }

    float metallic = 0.0;
    float roughness = 0.0;

    
    // Can add differnt Map layout stuff
    metallic = GetMetallic().r;
    roughness = GetRoughness();



    // Create a material var
    Material material;


    material.Albedo = texColor;
    material.Metallic = metallic;

    // Roughness Calculations
    material.FinalRoughness = roughness;
    material.FinalRoughness = clamp(material.FinalRoughness,0.045,1.0);

    

    // Normal Stuff
    material.Normal = normalize(VertexOutput.Normal);

    if(materialProperties.NormalMapFactor > 0.05)
    {
        material.Normal = normalize((texture(mapNormal, VertexOutput.TexCoord).xyz * 2.0) - vec3(1.0));
        material.Normal = normalize(VertexOutput.WorldNormal * material.Normal);
        material.Normal = normalize(material.Normal);
    }

    material.View = normalize(cameraView - VertexOutput.Position.xyz);

    // Gather the Ambient occlusion
    material.AO  = GetAO();

    material.Emissive = GetEmissive(material.Albedo.xyz);

    
    vec3 finalColor = CalculateLighting(material);


    if(renderMode == 0)
    {
        FragColor = vec4(finalColor, 1.0);
    }

    else
    {
        switch(renderMode)
        {
            // Pure Albedo
            case 1:
            {
                FragColor = material.Albedo;
                break;
            }

            // Pure Metallic
            case 2 :
            {
                FragColor = vec4(vec3(material.Metallic),1.0f);
                break;
            }

            // Pure RoughNess
            case 3 :
            {
                FragColor = vec4(vec3(material.FinalRoughness),1.0f);
            }

        }
    }
    

}


