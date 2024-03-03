// VCustom  Basic Vertex Shader
#version 420 

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec2 aTexCoord;



out vec4 vertColor;
out vec4 vertPosition;
out vec4 vertexNormal;
out vec2 texCoord;



uniform mat4 model;
uniform mat4 viewProj;

uniform mat4 normalMat;


// For now flat coloring until i figure out textures
uniform vec4 matColor;




void main()
{
	
	mat4 modelViewProj = viewProj * model;
	gl_Position = modelViewProj * vec4(vPosition, 1.0f);


	

	vec4 tempNorm = normalMat * vec4( vNormal.xyz, 1.0f);
	vertexNormal = vec4(normalize(tempNorm.xyz), 1.0f);

	vertPosition = model * vec4(vPosition, 1.0f);

	vertColor = vColor;

	if (matColor != vec4 ( 0.0f, 0.0f, 0.0f, 1.0f) )
	{		
		vertColor = matColor;
	}

 	texCoord = aTexCoord;
	
	
}
