
struct Light
{
    vec3 color;
    vec3 position;
    vec3 direction;
    float intensity;
    float radius;
    int type;
    float innerAngle;
    float outerAngle;
};

struct Material
{
    vec4 Albedo;
    float Metallic;
    float Roughness;
    float FinalRoughness;
    float Reflectance;
    vec3 Emissive;
    vec3 Normal;
    float AO;  
    vec3 View;
};

struct SkyboxInfo
{
    samplerCube skybox;
    samplerCube irradiance;
    mat3 skyboxRotation;
    float luminance;
};