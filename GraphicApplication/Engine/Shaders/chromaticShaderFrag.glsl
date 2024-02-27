

struct VertexData
{
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

layout(location = 0) in VertexData VertexOutput;
layout(location = 0) out vec4 FragColor;

uniform float shaderTime;
uniform bool distort;

uniform vec3 cameraView;
uniform UBOLight uboLights;
uniform UniformMaterialData materialProperties;



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

float noise(vec2 p)
{
	float s = texture(mapMetallic,vec2(1.,2.*cos(shaderTime))*shaderTime*8. + p*1.).x;
	s *= s;
	return s;
}

float onOff(float a, float b, float c)
{
	return step(c, sin(shaderTime + a*cos(shaderTime*b)));
}

float ramp(float y, float start, float end)
{
	float inside = step(start,y) - step(end,y);
	float fact = (y-start)/(end-start)*inside;
	return (1.-fact) * inside;
	
}

float stripes(vec2 uv)
{
	
	float noi = noise(uv*vec2(0.5,1.) + vec2(1.,3.));
	return ramp(mod(uv.y*4. + shaderTime/2.+sin(shaderTime + sin(shaderTime*0.63)),1.),0.5,0.6)*noi;
}

vec3 getVideo(vec2 uv)
{
	vec2 look = uv;
	float window = 1./(1.+20.*(look.y-mod(shaderTime/4.,1.))*(look.y-mod(shaderTime/4.,1.)));
	look.x = look.x + sin(look.y*10. + shaderTime)/50.*onOff(4.,4.,.3)*(1.+cos(shaderTime*80.))*window;
	float vShift = 0.4*onOff(2.,3.,.9)*(sin(shaderTime)*sin(shaderTime*20.) + 
										 (0.5 + 0.1*sin(shaderTime*200.)*cos(shaderTime)));
	look.y = mod(look.y + vShift, 1.);
	vec3 video = vec3(texture(mapAlbedo,look));
	return video;
}

vec2 screenDistort(vec2 uv)
{
	uv -= vec2(.5,.5);
	uv = uv*1.2*(1./1.2+2.*uv.x*uv.x*uv.y*uv.y);
	uv += vec2(.5,.5);
	return uv;
}


float rand(vec2 co)
{
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}


vec3 GetChromatic()
{
	vec2 uv = VertexOutput.TexCoord;
    
	if (mod(shaderTime, 2.0) > 1.9)
		uv.x += cos(shaderTime * 10.0 + uv.y * 1000.0) * 0.01;
    
    //if (mod(shaderTime, 4.0) > 3.0)
	//	uv = floor(uv * 32.0) / 32.0;
    
	if (mod(shaderTime, 5.0) > 3.75)
    	uv += 1.0 / 64.0 * (2.0 * vec2(rand(floor(uv * 32.0) + vec2(32.05,236.0)), rand(floor(uv.y * 32.0) + vec2(-62.05,-36.0))) - 1.0);

	vec4 color = texture(mapAlbedo, uv);
    
    if (rand(vec2(shaderTime)) > 0.90)
		color = vec4(dot(color.rgb, vec3(0.25, 0.5, 0.25)));
    
    color.rgb += 0.25 * vec3(rand(shaderTime + VertexOutput.TexCoord / vec2(-213, 5.53)), rand(shaderTime - VertexOutput.TexCoord / vec2(213, -5.53)), rand(shaderTime + VertexOutput.TexCoord / vec2(213, 5.53))) - 0.125;

    return color.rgb;
}

vec3 GetDistortion()
{
    vec2 uv = VertexOutput.TexCoord;
	uv = screenDistort(uv);
	vec3 video = getVideo(uv);
	float vigAmt = 3.+.3*sin(shaderTime + 5.*cos(shaderTime*5.));
	float vignette = (1.-vigAmt*(uv.y-.5)*(uv.y-.5))*(1.-vigAmt*(uv.x-.5)*(uv.x-.5));
	
	video += stripes(uv);
	video += noise(uv*2.)/2.;
	video *= vignette;
	video *= (12.+mod(uv.y*30.+shaderTime,1.))/13.;
	
	return vec3(video);
}

void main()
{
    vec3 finalColor;

	if(distort)
    {
        finalColor = GetDistortion();
    }
    else
    {
        finalColor = GetChromatic();
    }
	
	FragColor = vec4(finalColor,1.0);
}