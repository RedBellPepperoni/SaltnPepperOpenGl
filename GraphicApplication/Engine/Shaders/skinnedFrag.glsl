

struct VertexData
{
	vec4 Position;
	vec3 Normal;
	vec2 TexCoord;
	mat3 WorldNormal;
	
	vec4 BoneWights;
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


const int MAX_LIGHT_COUNT = 16;

struct UBOLight
{
    Light lights[MAX_LIGHT_COUNT];
    int lightCount;
    int castShadow;

};



uniform sampler2D mapAlbedo;
uniform sampler2D mapAO;
uniform sampler2D mapRoughness;
uniform sampler2D mapMetallic;
uniform sampler2D mapEmissive;
uniform sampler2D mapNormal;

in VertexData VertexOutput;
flat in ivec4 BoneIds;
out vec4 FragColor;




uniform vec3 cameraView;
uniform UBOLight uboLights;
uniform UniformMaterialData materialProperties;

uniform bool displayBoneWeights;


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
   

    vec4 param = texture(mapAlbedo, VertexOutput.TexCoord);
  
    return (materialProperties.AlbedoColor  + (param ));

}

vec3 GetMetallic()
{
    if(materialProperties.MetallicMapFactor < 0.05)
    {
        return vec3(materialProperties.Metallic);
    }
    return vec3(materialProperties.Metallic  *(1.0 - materialProperties.MetallicMapFactor ) + (texture(mapMetallic, VertexOutput.TexCoord).rgb * materialProperties.MetallicMapFactor)  );
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

        // Directional Light
        if(light.type == 0)
        {
            	vec3 lightDir = normalize(-light.direction);

                // diffuse shading
                float diff = max(0.0, dot(normal, lightDir));

                // specular shading
                vec3 reflectDir = reflect(-lightDir, normal);
                float spec = pow(max(dot(material.View, reflectDir), 0.0), 32);

                // combine results
                vec3 ambient  = light.intensity  * material.Albedo.rgb * 0.3;
                vec3 diffuse  = light.color  * diff * material.Albedo.rgb * light.intensity;
                vec3 specular = vec3(spec)* material.Albedo.rgb * material.Metallic * 2.0;
                //vec3 lightContrib = (ambient + diffuse + specular * 0.01);
                vec3 lightContrib = (diffuse) + (specular) + (ambient);


                result += lightContrib;
                //return result;
        }

        // PointLight
        else if(light.type == 2)
        {
            vec3 lightDir = normalize(light.position - VertexOutput.Position.xyz);
             // diffuse shading
            float diff = max(dot(normal, lightDir), 0.0);
            // specular shading
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(material.View, reflectDir), 0.0), 32);
            // attenuation
            float distance    = length(light.position - VertexOutput.Position.xyz);
           // float finalAttenuation = 1.0 / (1.0 + ((2.0/light.radius) * distance) + ((1.0/(light.radius*light.radius)) * (distance * distance)));    
            float finalAttenuation = clamp(1.0 - ((distance * distance)/(light.radius * light.radius)),0.0,1.0);    
            // combine results

            vec3 diffuse  = light.color  * diff * material.Albedo.rgb *  light.intensity;
            vec3 specular = vec3(light.color) * spec * 0.01;


	    //vec3 result = (ambient + diffuse + specular);
            vec3 lightContrib = (diffuse + specular) * finalAttenuation * finalAttenuation;

            result += lightContrib;

        }
        // spotlight
       else if(light.type == 1)
        {
            vec3 lightDir = normalize(light.position - VertexOutput.Position.xyz);
            float distance    = length(light.position - VertexOutput.Position.xyz);
             // diffuse shading
            float diff = max(dot(normal, lightDir), 0.0);
            // specular shading
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(material.View, reflectDir), 0.0), 32);
            // attenuation
           
            //float finalAttenuation = 1.0 / (1.0 + ((3.0/light.intensity) * distance) + ((1.0/(light.intensity*light.intensity)) * (distance * distance)));    
           float finalAttenuation = 1.0 / (1.0 + ((1.0/light.intensity) * distance) + ((1.0/(light.intensity*light.intensity)) * (distance * distance)));    
           // float finalAttenuation = clamp(1.0 - ((distance * distance)/(light.intensity * light.intensity * 2.0)),0.0,1.0);    
            // combine results


            float theta = dot(lightDir, normalize(-light.direction)); 

            float epsilon = light.innerAngle - light.outerAngle;
            float finalIntensity = clamp((theta - light.outerAngle)/ epsilon, 0.0,1.0);

            vec3 ambient = light.intensity * material.Albedo.rgb * 0.6;
            vec3 diffuse  = light.color  * diff * material.Albedo.rgb * light.intensity;
            vec3 specular = vec3(light.color) * spec * 0.01 ;
            
          //  ambient *= finalAttenuation * finalIntensity;
           // diffuse *= finalAttenuation * finalIntensity;
            //specular *= finalAttenuation * finalIntensity;
	    //vec3 result = (ambient + diffuse + specular);
            vec3 lightContrib = (diffuse + specular + ambient) * finalIntensity * finalAttenuation;

            result += lightContrib;
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

    // Can add different work flows here

    metallic = GetMetallic().r;
    roughness = GetRoughness();
    
    Material material;


    material.Albedo = texColor;
    material.Metallic = metallic;
    material.Roughness = roughness;

    material.Normal = normalize(VertexOutput.Normal);
    material.View = normalize(cameraView - VertexOutput.Position.xyz);

    //vec3 finalColor = CalculateLighting(material) + material.Emissive;
    vec3 finalColor = CalculateLighting(material);


    // Displaying Bone Weights
    for(int index = 0; index < 4; index++)
    {
       
    }

    FragColor = vec4(finalColor, 1.0);

}


