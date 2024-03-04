
struct VertexData
{
	vec4 Position;
	vec3 Normal;
	vec2 TexCoord;
	mat3 WorldNormal;
	
	vec4 BoneWights;
};



layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in ivec4 vBoneId;
layout (location = 4) in vec4 vBoneWeights;

const int MAX_BONES = 200;

uniform mat4 model;
uniform mat4 viewProj;
uniform mat4 normalMat;
uniform mat4 boneTransforms[MAX_BONES];

out VertexData VertexOutput;
flat out ivec4 BoneIds;




void main()
{
	mat4 boneT = boneTransforms[BoneIds[0]] * vBoneWeights[0];
	boneT += boneTransforms[BoneIds[1]] * vBoneWeights[1];
	boneT += boneTransforms[BoneIds[2]] * vBoneWeights[2];
	boneT += boneTransforms[BoneIds[3]] * vBoneWeights[3];	

	vec4 posL = boneT * vec4(vPosition, 1.0f);

	mat4 modelViewProj = viewProj * model;
	//gl_Position = modelViewProj * posL;


	
	vec3 tempNorm = normalize(mat3(normalMat) * vNormal);


	VertexOutput.Normal = tempNorm;
	VertexOutput.Position = model * vec4(vPosition, 1.0f);
	//VertexOutput.Position = model * posL;


	VertexOutput.Position = vec4(vPosition, 1.0f);
	VertexOutput.TexCoord = aTexCoord;

	// The TBN matrix
	//VertexOutput.WorldNormal = transpose(mat3(tempTangent, tempBiTangent, tempNorm));
	BoneIds = vBoneId;
	VertexOutput.BoneWights = vBoneWeights;

	//gl_Position = modelViewProj * vec4(vPosition, 1.0f);
	gl_Position = vec4(vPosition, 1.0f);

}


