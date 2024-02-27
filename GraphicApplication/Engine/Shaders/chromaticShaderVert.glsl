
struct VertexData
{
	vec4 Position;
	vec3 Normal;
	vec2 TexCoord;
	mat3 WorldNormal;
};

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec3 vTangent;
layout (location = 4) in vec3 vBitangent;





uniform mat4 model;
uniform mat4 viewProj;
uniform mat4 normalMat;



layout(location = 0) out VertexData VertexOutput;


void main()
{
	
	mat4 modelViewProj = viewProj * model;
	gl_Position = modelViewProj * vec4(vPosition, 1.0f);



	vec3 tempNorm = normalize(mat3(normalMat) * vNormal);
	vec3 tempTangent = normalize(mat3(normalMat) * vTangent);
	vec3 tempBiTangent = normalize(mat3(normalMat) * vBitangent);


	VertexOutput.Normal = tempNorm;
	VertexOutput.Position = model * vec4(vPosition, 1.0f);
	VertexOutput.TexCoord = aTexCoord;


	// The TBN matrix
	VertexOutput.WorldNormal = transpose(mat3(tempTangent, tempBiTangent, tempNorm));
	
	
}


