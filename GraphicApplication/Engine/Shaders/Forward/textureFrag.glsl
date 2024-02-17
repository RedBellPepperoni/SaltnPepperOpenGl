#version 420 

in vec4 vertColor;
in vec4 vertPosition;
in vec4 vertexNormal;
in vec2 texCoord;
out vec4 FragColor;



uniform vec3 cameraView;


const int DirectionlightId = 2;
const int SpotLightId = 1;
const int PointLigthId = 0;
const int lightCount = 10;


struct DirLight
{
	vec3 direction;
	vec3 color;
	vec3 intensity;
	vec3 specular;

};



struct PointLight
{
	vec3 position;
	vec3 color;
	vec3 intensity;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	

};

struct SpotLight {

    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 intensity;
    vec3 color;
    vec3 specular;       
};

uniform sampler2D mapAlbedo;

uniform DirLight dirLight;

#define MAX_POINT_LIGHTS 1
uniform PointLight pointLightList[MAX_POINT_LIGHTS];

#define MAX_SPOT_LIGHTS 2
uniform SpotLight spotLightList[MAX_SPOT_LIGHTS];


vec3 CalculateDirectionalLight(vec3 viewDir ,vec3 normal,  DirLight light);
vec3 CalculatePointLight(vec3 viewDir, vec3 normal, vec3 vPos, PointLight light);
vec3 CalcSpotLight(vec3 viewDir, vec3 normal, vec3 vPos,SpotLight light);

vec4 CalculateLighting(vec4 vPos, vec4 vColor, vec4 vNormal);


void main()
{

   

	vec4 vertexLit = CalculateLighting(vertPosition, vertColor, vertexNormal);


    FragColor.xyz = vertexLit.xyz;
	FragColor.a = 1.0f;

}



vec3 CalculateDirectionalLight(vec3 viewDir ,vec3 normal,  DirLight light)
{
	

 	vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    // combine results
    vec3 ambient  = light.intensity  * vec3(texture(mapAlbedo, texCoord));
    vec3 diffuse  = light.color  * diff * vec3(texture(mapAlbedo, texCoord));
    vec3 specular = light.specular * spec * vec3(0.25f);
   
    vec3 result = (ambient + diffuse + specular);

	return result;

}

vec3 CalculatePointLight(vec3 viewDir, vec3 normal, vec3 vPos, PointLight light)
{
	

	vec3 lightDir = normalize(light.position - vPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance    = length(light.position - vPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.intensity  * vec3(texture(mapAlbedo, texCoord));
    vec3 diffuse  = light.color  * diff * vec3(texture(mapAlbedo, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(mapAlbedo, texCoord));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;


	vec3 result = (ambient + diffuse + specular);

	return result;
}

vec3 CalcSpotLight(vec3 viewDir, vec3 normal, vec3 vPos, SpotLight light)
{
	vec3 lightDir = normalize(light.position - vPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance = length(light.position - vPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.intensity * vec3(texture(mapAlbedo, texCoord));
    vec3 diffuse = light.color * diff * vec3(texture(mapAlbedo, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(mapAlbedo, texCoord));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}


vec4 CalculateLighting(vec4 vPos, vec4 vColor, vec4 vNormal)
{
 	vec3 norm = normalize(vNormal.xyz);
	vec3 viewDir = normalize(cameraView - vPos.xyz);

	vec3 result = CalculateDirectionalLight(viewDir,norm,dirLight);
	
   
	for(int index = 0; index < MAX_POINT_LIGHTS; index++)
	{
		result += CalculatePointLight(viewDir,norm,vertPosition.xyz,pointLightList[index]);
	}

     
	for(int index = 0; index < MAX_SPOT_LIGHTS; index++)
	{
		result += CalcSpotLight(viewDir,norm,vertPosition.xyz,spotLightList[index]);
	}


	vec4 finalColor = vec4(result,1.0f);
	
	return finalColor;
}