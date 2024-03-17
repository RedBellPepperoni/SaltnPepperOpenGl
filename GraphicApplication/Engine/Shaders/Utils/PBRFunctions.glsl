
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

vec3 fresnelSchlickRoughness(vec3 F0, float cosTheta, float roughness)
{
    return F0 + ((max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0));
}
