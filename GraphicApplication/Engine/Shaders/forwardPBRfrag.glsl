
struct VertexData
{
	vec4 Color;
	vec4 Position;
	vec3 Normal;
	vec2 TexCoord;
	mat3 WorldNormal;
};

struct Material
{
    vec4 Albedo;
    float Metallic;
    float Roughness;
    float PerceptualRoughness;
    float Reflectance;
    vec3 Emissive;
    vec3 Normal;
    float AO;
    vec3 View;
    float NDotV;
    vec3 F0;
    vec3 EnergyCompensation;
    vec2 dfg;
};

layout(location = 0) in VertexData VertexOutput;
layout(location = 0) out vec4 FragColor;



uniform vec3 cameraView;


const int DirectionlightId = 2;
const int SpotLightId = 1;
const int PointLigthId = 0;
const int lightCount = 10;


struct Light
{
    vec4 colour;
    vec4 position;
    vec4 direction;
    float intensity;
    float radius;
    int type;
    float angle;
};

uniform UniformMaterialData
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
    
} materialProperties;

uniform UBOLight
{
    Light lights[32];
    int LightCount;
    int Mode;
    float Width;
    float Height;
    float LightSize;
    vec4 cameraPosition;
    int shadowEnabled;
    
} uboLight;



uniform sampler2D mapAlbedo;
uniform sampler2D mapAO;
uniform sampler2D mapRoughness;
uniform sampler2D mapMetallic;
uniform sampler2D mapEmissive;
uniform sampler2D mapNormal;


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
    if(materialProperties.AlbedoMapFactor < 0.05)
    {
        return materialProperties.AlbedoColor;
    }
    vec4 param = texture(mapAlbedo, VertexOutput.TexCoord);
    return (materialProperties.AlbedoColor * (1.0- materialProperties.AlbedoMapFactor) + (DeGamma(param) * materialProperties.AlbedoMapFactor));
}

vec3 GetMetallic()
{
    if(materialProperties.MetallicMapFactor < 0.05)
    {
        return vec3(materialProperties.Metallic);
    }
    return vec3(materialProperties.Metallic  *(1.0 - materialProperties.MetallicMapFactor ) + (texture(mapMetallic, VertexOutput.TexCoord).xyz * materialProperties.MetallicMapFactor)  );
}

float GetRoughness()
{
    if(materialProperties.RoughnessMapFactor < 0.05)
    {
        return materialProperties.Roughness;
    }
    return (materialProperties.Roughness * (1.0 - materialProperties.RoughnessMapFactor)) + (materialProperties.RoughnessMapFactor *texture(mapRoughness, VertexOutput.TexCoord).x);
}

float GetAO()
{
    if(materialProperties.RoughnessMapFactor < 0.05)
    {
        return 1.0;
    }
    return (1.0 - materialProperties.AOMapFactor) + (materialProperties.AOMapFactor * texture(mapAO, VertexOutput.TexCoord).x);
}

vec3 GetEmissive(vec3 albedo)
{
    if (materialProperties.EmissiveMapFactor < 0.05)
    {
        return albedo * materialProperties.Emissive;
    }
    vec3 param = texture(mapEmissive, VertexOutput.TexCoord).xyz;
    return (albedo * materialProperties.Emissive) + (DeGamma(param) * materialProperties.EmissiveMapFactor);
}

float perceptualRoughnessToRoughness(float perceptualRoughness)
{
    return perceptualRoughness * perceptualRoughness;
}

float saturate(float value)
{
    return clamp(value, 0.0, 1.0);
}

float computeDielectricF0(float reflectance)
{
    return (0.1599999964237213134765625 * reflectance) * reflectance;
}

vec3 computeF0(vec4 baseColor, float metallic, float reflectance)
{
    return (baseColor.xyz * metallic) + vec3(reflectance * (1.0 - metallic));
}

vec3 computeDiffuseColor(vec4 baseColor, float metallic)
{
    return baseColor.xyz * (1.0 - metallic);
}


float clampNoV(float NoV)
{
    return max(NoV, 9.9999997473787516355514526367188e-05);
}

float F_Schlick(float u, float f0, float f90)
{
    return f0 + ((f90 - f0) * pow(1.0 - u, 5.0));
}

float Fd_Burley(float roughness, float NoV, float NoL, float LoH)
{
    float f90 = 0.5 + (((2.0 * roughness) * LoH) * LoH);
    float param = 1.0;
    float param_1 = f90;
    float param_2 = NoL;
    float lightScatter = F_Schlick(param, param_1, param_2);
    float param_3 = 1.0;
    float param_4 = f90;
    float param_5 = NoV;
    float viewScatter = F_Schlick(param_3, param_4, param_5);
    return (lightScatter * viewScatter) * 0.3183098733425140380859375;
}

float Diffuse(float roughness, float NoV, float NoL, float LoH)
{
    float param = roughness;
    float param_1 = NoV;
    float param_2 = NoL;
    float param_3 = LoH;
    return Fd_Burley(param, param_1, param_2, param_3);
}

vec3 DiffuseLobe(Material material, float NoV, float NoL, float LoH)
{
    float param = material.Roughness;
    float param_1 = NoV;
    float param_2 = NoL;
    float param_3 = LoH;
    return material.Albedo.xyz * Diffuse(param, param_1, param_2, param_3);
}

float D_GGX(float roughness, float NoH, vec3 n, vec3 h)
{
    vec3 NxH = cross(n, h);
    float a = NoH * roughness;
    float k = roughness / (dot(NxH, NxH) + (a * a));
    float d = (k * k) * 0.3183098733425140380859375;
    return min(d, 65504.0);
}

float distribution(float roughness, float NoH, vec3 h, vec3 normal)
{
    float param = roughness;
    float param_1 = NoH;
    return D_GGX(param, param_1, normal, h);
}

float V_SmithGGXCorrelated(float NoV, float NoL, float a)
{
    float a2 = a * a;
    float GGXL = NoV * sqrt(((((-NoL) * a2) + NoL) * NoL) + a2);
    float GGXV = NoL * sqrt(((((-NoV) * a2) + NoV) * NoV) + a2);
    return 0.5 / (GGXV + GGXL);
}

float visibility(float roughness, float NoV, float NoL)
{
    float param = roughness;
    float param_1 = NoV;
    float param_2 = NoL;
    return V_SmithGGXCorrelated(param, param_1, param_2);
}

float pow5(float x)
{
    float x2 = x * x;
    return (x2 * x2) * x;
}

vec3 F_Schlick(vec3 f0, float f90, float VoH)
{
    float param = 1.0 - VoH;
    return f0 + ((vec3(f90) - f0) * pow5(param));
}

vec3 fresnel(vec3 f0, float LoH)
{
    float param = dot(f0, vec3(16.5));
    float f90 = saturate(param);
    float param_1 = f90;
    float param_2 = LoH;
    return F_Schlick(f0, param_1, param_2);
}

vec3 IsotropicLobe(Material material, Light light, vec3 h, float NoV, float NoL, float NoH, float LoH)
{
    float param = material.Roughness;
    float param_1 = NoH;
    float D = distribution(param, param_1, material.Normal, h);
    float param_2 = material.Roughness;
    float param_3 = NoV;
    float param_4 = NoL;
    float V = visibility(param_2, param_3, param_4);
    float param_5 = LoH;
    vec3 F = fresnel(material.F0, param_5);
    return F * (D * V);
}

vec3 SpecularLobe(Material material, Light light, vec3 h, float NoV, float NoL, float NoH, float LoH)
{
    float param = NoV;
    float param_1 = NoL;
    float param_2 = NoH;
    float param_3 = LoH;
    return IsotropicLobe(material, light, h, param, param_1, param_2, param_3);
}

float ComputeMicroShadowing(float NoL, float visibility_1)
{
    float aperture = inversesqrt(1.0 - min(visibility_1, 0.99989998340606689453125));
    float param = NoL * aperture;
    float microShadow = saturate(param);
    return microShadow * microShadow;
}


vec3 Lighting(vec3 F0, vec3 wsPos, Material material)
{
    vec3 result = vec3(0.0);
    Light light;
    for (int i = 0; i < ubo.LightCount; i++)
    {
        light.colour = ubo.lights[i].colour;
        light.position = ubo.lights[i].position;
        light.direction = ubo.lights[i].direction;
        light.intensity = ubo.lights[i].intensity;
        light.radius = ubo.lights[i].radius;
        light.type = ubo.lights[i].type;
        light.angle = ubo.lights[i].angle;
        float value = 0.0;

        // point
        if (light.type == 2)
        {
            vec3 L = light.position.xyz - wsPos;
            float dist = length(L);
            L = normalize(L);
            float atten = light.radius / (pow(dist, 2.0) + 1.0);
            float attenuation = clamp(1.0 - ((dist * dist) / (light.radius * light.radius)), 0.0, 1.0);
            value = attenuation;
            light.direction = vec4(L, 1.0);
        }
        else
        {
            // Spot light
            if (light.type == 1)
            {
                vec3 L_1 = light.position.xyz - wsPos;
                float cutoffAngle = 1.0 - light.angle;
                float dist_1 = length(L_1);
                L_1 = normalize(L_1);
                float theta = dot(L_1, light.direction.xyz);
                float epsilon = cutoffAngle - (cutoffAngle * 0.90);
                float attenuation_1 = (theta - cutoffAngle) / epsilon;
                attenuation_1 *= (light.radius / (pow(dist_1, 2.0) + 1.0));
                value = clamp(attenuation_1, 0.0, 1.0);
            }

            // Directional
            else
            {
                vec3 param = wsPos;
                
                value = 1.0;
                 
            }
        }


        vec3 Li = light.direction.xyz;
        vec3 Lradiance = light.colour.xyz * light.intensity;
        vec3 Lh = normalize(Li + material.View);
        float param_5 = dot(material.Normal, Li);
        float lightNoL = saturate(param_5);
        vec3 h = normalize(material.View + Li);
        float param_6 = dot(material.Normal, material.View);
        float shading_NoV = clampNoV(param_6);
        float NoV = shading_NoV;
        float param_7 = lightNoL;
        float NoL = saturate(param_7);
        float param_8 = dot(material.Normal, h);
        float NoH = saturate(param_8);
        float param_9 = dot(Li, h);
        float LoH = saturate(param_9);
        float param_10 = NoV;
        float param_11 = NoL;
        float param_12 = LoH;
        vec3 Fd = DiffuseLobe(material, param_10, param_11, param_12);
        float param_13 = NoV;
        float param_14 = NoL;
        float param_15 = NoH;
        float param_16 = LoH;
        vec3 Fr = SpecularLobe(material, light, h, param_13, param_14, param_15, param_16);
        vec3 colour = Fd + Fr;
        float param_17 = NoL;
        float param_18 = material.AO;
        result += ((colour * Lradiance) * ((value * NoL) * ComputeMicroShadowing(param_17, param_18)));

    }

    return result;
}


void main()
{

   

	//vec4 vertexLit = CalculateLighting(VertexOutput.Position, VertexOutput.Color, VertexOutput.Normal);


    FragColor.xyz = vec3(1.0f);
	FragColor.a = 1.0f;

}


