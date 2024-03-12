#include "Utils/ShaderUtils.glsl"

vec3 CalculateDirectionalLight(Light light,Material material,vec3 normal)
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

    vec3 lightContrib = (diffuse) + (specular) + (ambient);


    return lightContrib;
}

vec3 CalculatePointLight(Light light, Material material,vec3 normal, vec3 position)
{
     
    vec3 lightDir = normalize(light.position - position);
             // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
            // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(material.View, reflectDir), 0.0), 32);
            // attenuation
    float distance    = length(light.position - position);
           // float finalAttenuation = 1.0 / (1.0 + ((2.0/light.radius) * distance) + ((1.0/(light.radius*light.radius)) * (distance * distance)));    
    float finalAttenuation = clamp(1.0 - ((distance * distance)/(light.radius * light.radius)),0.0,1.0);    
            // combine results

    vec3 diffuse  = light.color  * diff * material.Albedo.rgb *  light.intensity;
    vec3 specular = vec3(light.color) * spec * 0.01;


    vec3 lightContrib = (diffuse + specular) * finalAttenuation * finalAttenuation;

    return lightContrib;

}

vec3 CalculateSpotLight(Light light, Material material,vec3 normal,vec3 position)
{
    vec3 lightDir = normalize(light.position - position);
    float distance    = length(light.position - position);
             // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
            // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(material.View, reflectDir), 0.0), 32);
          
          
    float finalAttenuation = 1.0 / (1.0 + ((1.0/light.intensity) * distance) + ((1.0/(light.intensity*light.intensity)) * (distance * distance)));    
           

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

    return lightContrib;
}


vec3 ForwardLighting(Light light, Material material,vec3 normal, vec3 position)
{
    vec3 result = vec3(0.0f);

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

        return result;
}


vec3 ImageBasedLighting(vec3 albedo, vec3 lightReflection, Material material)
{
    return vec3(1.0f);
}