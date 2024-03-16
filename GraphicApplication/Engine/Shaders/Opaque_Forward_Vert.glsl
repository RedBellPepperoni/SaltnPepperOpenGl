
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

layout (location = 5) in ivec4 vboneIds;
layout (location = 6) in vec4 vboneWeights;


const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 boneTransforms[MAX_BONES];

uniform mat4 model;
uniform mat4 viewProj;
uniform mat4 normalMat;

uniform bool isSkinned;

layout(location = 0) out VertexData VertexOutput;


void main()
{
	
	vec4 updatedPosition = vec4(0.0f);
    vec3 updatedNormal = vec3(0.0f);

	if(isSkinned == true)
	{
		 for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
        {
            // Current bone-weight pair is non-existing
            if(vboneIds[i] == -1) 
                continue;

            // Ignore all bones over count MAX_BONES
            if(vboneIds[i] >= MAX_BONES) 
            {
                updatedPosition = vec4(vPosition,1.0f);
                break;
            }
            // Set pos
            vec4 localPosition = boneTransforms[vboneIds[i]] * vec4(vPosition,1.0f);
            updatedPosition += localPosition * vboneWeights[i];
			
            // Set normal
            vec3 localNormal = mat3(boneTransforms[vboneIds[i]]) * vNormal;
            updatedNormal += localNormal * vboneWeights[i];
        }
	}

	else
	{
		updatedPosition = vec4(vPosition,1.0f);
		updatedNormal = vNormal;
	}

	mat4 modelViewProj = viewProj * model;
	gl_Position = modelViewProj * updatedPosition;



	vec3 tempNorm = normalize(mat3(normalMat) * updatedNormal);
	vec3 tempTangent = normalize(mat3(normalMat) * vTangent);
	vec3 tempBiTangent = normalize(mat3(normalMat) * vBitangent);


	VertexOutput.Normal = tempNorm;
	VertexOutput.Position = model * updatedPosition;
	VertexOutput.TexCoord = aTexCoord;

	// The TBN matrix
	VertexOutput.WorldNormal = transpose(mat3(tempTangent, tempBiTangent, tempNorm));
	
}


