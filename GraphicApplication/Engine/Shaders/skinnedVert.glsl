
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
layout(location = 5) in ivec4 vBoneIds; 
layout(location = 6) in vec4 vWeights;



const int MAX_BONES = 200;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 model;
uniform mat4 viewProj;
uniform mat4 normalMat;
uniform mat4 finalBonesMatrices[MAX_BONES];

out VertexData VertexOutput;




void main()
{
	
 	vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(vBoneIds[i] == -1) 
            continue;
        if(vBoneIds[i] >=MAX_BONES) 
        {
            totalPosition = vec4(vPosition,1.0f);
            break;
        }
        vec4 localPosition = finalBonesMatrices[vBoneIds[i]] * vec4(vPosition,1.0f);
        totalPosition += localPosition * vWeights[i];
        vec3 localNormal = mat3(finalBonesMatrices[vBoneIds[i]]) * vNormal;
   }

	mat4 modelViewProj = viewProj * model;
	//gl_Position = modelViewProj * vec4(vPosition, 1.0f);
	gl_Position = modelViewProj * totalPosition;



	vec3 tempNorm = normalize(mat3(normalMat) * vNormal);
	vec3 tempTangent = normalize(mat3(normalMat) * vTangent);
	vec3 tempBiTangent = normalize(mat3(normalMat) * vBitangent);


	VertexOutput.Normal = tempNorm;
	VertexOutput.Position = model * vec4(vPosition, 1.0f);
	VertexOutput.TexCoord = aTexCoord;

	// The TBN matrix
	VertexOutput.WorldNormal = transpose(mat3(tempTangent, tempBiTangent, tempNorm));
	

}


