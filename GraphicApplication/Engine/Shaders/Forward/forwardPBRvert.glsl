// VCustom  Basic Vertex Shader
#version 450 


struct VertexData
{
	vec4 Color;
	vec4 Position;
	vec3 Normal;
	vec2 TexCoord;
	mat3 WorldNormal;
};

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec3 vTangent;
layout (location = 4) in vec3 vBitangent;
layout (location = 5) in vec2 aTexCoord;




uniform mat4 model;
uniform mat4 viewProj;
uniform mat4 normalMat;


layout(location = 0) out VertexData VertexOutput;


void main()
{
	
	mat4 modelViewProj = viewProj * model;
	gl_Position = modelViewProj * vec4(vPosition, 1.0f);



	vec4 tempNorm = normalMat * vec4( vNormal.xyz, 1.0f);
	VertexOutput.Normal = normalize(tempNorm.xyz);

	VertexOutput.Position = model * vec4(vPosition, 1.0f);

	VertexOutput.Color = vColor;

	VertexOutput.TexCoord = aTexCoord;
	VertexOutput.WorldNormal = mat3(normalMat) * mat3(vec3(vTangent), vec3(vBitangent), vec3(vNormal));
	
}


