
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
    float PerceptualRoughness;
    float Reflectance;
    vec3 Emissive;
    vec3 Normal;
    float AO;  
    vec3 View;
};
