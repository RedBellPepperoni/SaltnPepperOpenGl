#include "ModelLoader.h"
#include "Engine/Utils/Logging/Log.h"
#include "Engine/Utils/Loading/ImageLoader.h"
#include "Engine/Core/Rendering/Textures/Image.h"

// Assimp stuff
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/GltfMaterial.h>

#define nominmax

namespace SaltnPepperEngine
{
    const char* const AlbedoTextureName = "albedo";
    const char* const EmissiveTextureName = "emmisive";
    const char* const HeightTextureName = "height";
    const char* const NormalTextureName = "normal";
    const char* const AORoughMetalName = "aoroughmetal";
    const char* const PreferredExtension = ".png";

    ModelLoader::ModelLoader()
    {
    }

    ModelLoader::~ModelLoader()
    {
    }


    FilePath GetTexturePath(const FilePath& lookupDir, const std::string& name, const aiScene* scene, const aiMaterial* material, aiTextureType type)
    {
        FilePath path = lookupDir / FileSystem::GetFilePathfromString(name + PreferredExtension);

        if (FileSystem::Exists(path)) { return path; }

        if (material->GetTextureCount(type) > 0)
        {
            aiString assimPath;

            if (material->GetTexture(type, 0, &assimPath) == aiReturn_SUCCESS)
            {
                const char* filepath = assimPath.C_Str();

                // Checking if the texture is embeded in the model file
                const aiTexture* data = scene->GetEmbeddedTexture(filepath);

                if (data == nullptr) { return lookupDir / filepath; }

                // Maybe Make a Image converter and Saving  later on
                LOG_WARN("Texture : EmbededTexture Found [{0}]", path.string());

                return path;

            }
        }

        return FilePath();
    }



    ModelDetail ModelLoader::LoadModel(const FilePath& filePath)
    {
        // Creating a New ModelDetails Ref
        ModelDetail model;

        FilePath directory = filePath.parent_path();

        // File doesnt exist at the given path
        if (!FileSystem::Exists(filePath))
        {
            LOG_ERROR("Assimp : file doesn't exist: [{0}]", filePath.string());
            return model;
        }


        // LOG_TRACE("Assimp : Loading model from file: [{0}]", filePath.string());

        static Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(filePath.string().c_str(),
            aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices |
            aiProcess_OptimizeMeshes | aiProcess_ImproveCacheLocality | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace);



        if (scene == nullptr)
        {
            LOG_ERROR("Assimp : ImporterError -> {0}", importer.GetErrorString());
            return model;
        }


        model.meshes.resize((size_t)scene->mNumMeshes);
        model.materials.resize((size_t)scene->mNumMaterials);

        for (size_t index = 0; index < model.materials.size(); index++)
        {
            MaterialDetails& materialDetail = model.materials[index];
            aiMaterial*& material = scene->mMaterials[index];

            materialDetail.name = material->GetName().C_Str();

            if (materialDetail.name.empty())
            {
                materialDetail.name = "material_#" + std::to_string(index);
            }

#define GETFLOAT(type, field) \
            {  ai_real val;\
               if(material->Get(type, val) == aiReturn_SUCCESS)\
               {\
                    materialDetail.field = (float)val;\
               }\
            }


            GETFLOAT(AI_MATKEY_OPACITY, transparency);
            GETFLOAT(AI_MATKEY_METALLIC_FACTOR, metalness);
            GETFLOAT(AI_MATKEY_ROUGHNESS_FACTOR, roughness);

            // Some formats export alpha channel as 0 instead of 1
            if (materialDetail.transparency = 0.0f) { materialDetail.transparency = 1.0f; }

            aiColor4D baseColor;

            if (material->Get(AI_MATKEY_BASE_COLOR, baseColor) == aiReturn_SUCCESS)
            {
                materialDetail.baseColor[0] = baseColor[0];
                materialDetail.baseColor[1] = baseColor[1];
                materialDetail.baseColor[2] = baseColor[2];
                materialDetail.transparency *= baseColor[3];
            }

            aiString alphaMode;

            if (material->Get(AI_MATKEY_GLTF_ALPHAMODE, alphaMode) == aiReturn_SUCCESS)
            {
                if (strcmp(alphaMode.C_Str(), "MASK") == 0)
                {
                    materialDetail.alphaMask = true;
                }
                else
                {
                    materialDetail.alphaMask = false;
                }
            }

            float alphaCutoff = 0.0f;
            if (materialDetail.alphaMask && material->Get(AI_MATKEY_GLTF_ALPHACUTOFF, alphaCutoff) == aiReturn_SUCCESS)
            {
                materialDetail.transparency *= alphaCutoff;
            }

            aiColor3D emissiveColor;
            if (material->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveColor) == aiReturn_SUCCESS)
            {
                materialDetail.emission = Max(emissiveColor.r, emissiveColor.g, emissiveColor.b);
            }

            materialDetail.albedoMap = GetTexturePath(directory, AlbedoTextureName + '_' + std::to_string(index), scene, material, aiTextureType_DIFFUSE);
            materialDetail.emissionMap = GetTexturePath(directory, EmissiveTextureName + '_' + std::to_string(index), scene, material, aiTextureType_EMISSIVE);
            materialDetail.heightMap = GetTexturePath(directory, HeightTextureName + '_' + std::to_string(index), scene, material, aiTextureType_HEIGHT);

            // Storing the Packed data into the metallness map
            materialDetail.aoRoughMetalMap = GetTexturePath(directory, AORoughMetalName + '_' + std::to_string(index), scene, material, aiTextureType_METALNESS);

            if (!materialDetail.emissionMap.empty() && materialDetail.emission == 0.0f) { materialDetail.emission = 1.0f; }


        }
        //// Since minwindef conflicts with its macros , putting the function in parenthesis to solve that
        //Vector3 minCoords = Vector3((std::numeric_limits<float>::max)());
        //Vector3 maxCoords = Vector3(1-.0f * (std::numeric_limits<float>::max)());

        //for (size_t index = 0; index < model.meshes.size(); index++)
        //{
        //    aiMesh*& mesh = scene->mMeshes[index];
        //    std::pair<Vector3, Vector3> coords = MinMaxComponents((Vector3*)mesh->mVertices, (size_t)mesh->mNumVertices);

        //    minCoords = VectorMin(minCoords, coords.first);
        //    maxCoords = VectorMax(maxCoords, coords.second);
        //}

        //Vector3 modelCenter = (minCoords + maxCoords) * 0.5f;


        for (size_t index = 0; index < model.meshes.size(); index++)
        {
            aiMesh*& mesh = scene->mMeshes[index];
            MeshDetails& meshDetails = model.meshes[index];

            meshDetails.name = mesh->mName.C_Str();

            if (meshDetails.name.find("meshes_") != meshDetails.name.npos)
            {
                meshDetails.name = "mesh_#" + std::to_string(index);
            }

            meshDetails.useNormal = mesh->HasNormals();
            meshDetails.useTexture = mesh->HasTextureCoords(0);
            meshDetails.useColor = !meshDetails.useTexture;
            meshDetails.material = model.materials.data() + (size_t)mesh->mMaterialIndex;

            if (mesh->mNormals == nullptr)
            {
                LOG_ERROR("Assimp : No normals found");
            }

            if (mesh->mVertices == nullptr)
            {
                LOG_ERROR("Assimp : No vertices found");
            }

            if (mesh->mNumFaces <= 0)
            {
                LOG_ERROR("Assimp : No Faces/Indices found");
            }

            std::vector<Vertex> vertices;

            vertices.resize((size_t)mesh->mNumVertices);

            for (size_t index = 0; index < (size_t)mesh->mNumVertices; index++)
            {
                vertices[index].position = ((Vector3*)mesh->mVertices)[index];
                // vertices[index].position -= modelCenter;
                vertices[index].normal = ((Vector3*)mesh->mNormals)[index];

                if (meshDetails.useTexture)
                {
                    vertices[index].texCoord = Vector2(mesh->mTextureCoords[0][index].x, mesh->mTextureCoords[0][index].y);
                    vertices[index].tangent = ((Vector3*)mesh->mTangents)[index];
                    vertices[index].bitangent = ((Vector3*)mesh->mBitangents)[index];
                }
                else
                {

                    vertices[index].tangent = Cross(vertices[index].normal, Vector3(1.0f, 0.0f, 0.0f));
                    vertices[index].bitangent = Cross(vertices[index].normal, vertices[index].tangent);
                }


                // No color sicne textures are implemented?
                if (meshDetails.useColor)
                {
                    /* vertices[index].color.r = mesh->mColors[0][index].r;
                     vertices[index].color.g = mesh->mColors[0][index].g;
                     vertices[index].color.b = mesh->mColors[0][index].b;
                     vertices[index].color.a = mesh->mColors[0][index].a;*/
                }
                else
                {
                    /* vertices[index].color = Vector4(1.0f);*/
                }
            }


            meshDetails.indices.resize((size_t)mesh->mNumFaces * 3);
            for (size_t index = 0; index < (size_t)mesh->mNumFaces; index++)
            {
                if (mesh->mFaces[index].mNumIndices == 3)
                {
                    meshDetails.indices[3 * index + 0] = mesh->mFaces[index].mIndices[0];
                    meshDetails.indices[3 * index + 1] = mesh->mFaces[index].mIndices[1];
                    meshDetails.indices[3 * index + 2] = mesh->mFaces[index].mIndices[2];
                }
                else continue;

            }


            meshDetails.boneData.resize(vertices.size());
            LoadMeshBones(meshDetails,mesh);

            if (meshDetails.name.empty())
            {
                meshDetails.name = FileSystem::GetFileName(filePath);
            }





            meshDetails.useTexture = true;
            meshDetails.vertices = std::move(vertices);



        }


        importer.FreeScene();


        // model = LoadPlyModel(filePath);


        return model;
    }

    ModelDetail ModelLoader::LoadModel(const string& stringPath)
    {
        return LoadModel(FileSystem::GetFilePathfromString(stringPath));
    }

    void ModelLoader::LoadMeshBones(MeshDetails& meshDetail, const aiMesh* pMesh)
    {
        for (uint32_t index = 0; index < pMesh->mNumBones; index++)
        {
            LoadSingleBone(meshDetail,pMesh->mBones[index]);
        }
    }

    void ModelLoader::LoadSingleBone(MeshDetails& meshDetail, const aiBone* pBone)
    {

        int BoneId = 0;

        std::string BoneName(pBone->mName.C_Str());

        if (meshDetail.BoneNameToIndexMap.find(BoneName) == meshDetail.BoneNameToIndexMap.end())
        {
            BoneId = (int)meshDetail.BoneNameToIndexMap.size();
            meshDetail.BoneNameToIndexMap[BoneName] = BoneId;
        }
        else
        {
            BoneId = meshDetail.BoneNameToIndexMap[BoneName];
        }

        for (uint32_t i = 0; i < pBone->mNumWeights; i++) {
            const aiVertexWeight& vw = pBone->mWeights[i];
            uint32_t GlobalVertexID = pBone->mWeights[i].mVertexId;
            meshDetail.boneData[GlobalVertexID].AddBoneData(BoneId, vw.mWeight);
        }

    }



}