
layout(location = 0)  in vec3 position;
layout(location = 1)  in vec2 texCoord;
layout(location = 2)  in vec3 normal;
layout(location = 3)  in vec3 tangent;
layout(location = 4)  in vec3 bitangent;

uniform mat4 model;
uniform mat4 normalMat;



struct Camera
{
    vec3 position;
    mat4 viewProjMatrix;
    mat4 invViewProjMatrix;
};


struct VertexOut
{
    vec2 TexCoord;
    vec3 Normal;
    vec4 Position;
    mat3 WorldNormal;

};

uniform Camera camera;

layout(location = 0) out VertexOut vsout;

void main()
{

    mat4 modelViewProj = camera.viewProjMatrix * model;

    // gathering nomalized TBN data
    vec3 tempTangent = normalize(mat3(normalMat) * tangent);
    vec3 tempBiTangent = normalize(mat3(normalMat) * bitangent);
    vec3 tempNorm = normalize(mat3(normalMat) * normal);

    // The Tangent Binormal and Normal Matrix
    vsout.WorldNormal = (mat3(tempTangent, tempBiTangent, tempNorm));
    vsout.Normal = tempNorm;
    vsout.Position = model * vec4(position, 1.0f);
    vsout.TexCoord = texCoord;

    gl_Position = modelViewProj * vec4(position, 1.0f);

   
}