#include "SkinnedModel.h"
#include "Engine/Utils/Logging/Log.h"

namespace SaltnPepperEngine
{


	namespace Rendering
	{
		SkinnedModel::SkinnedModel(const std::string& filePath)
		{
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				LOG_ERROR("ERROR : [{0}]", importer.GetErrorString());
				return;
			}

			m_filePath = filePath;

			ProcessNode(scene->mRootNode, scene);
		}

		void SkinnedModel::ExtractBoneWeightForVertices(std::vector<Vector4Int>& boneIDs_all, std::vector<Vector4>& weights_all, aiMesh* mesh, const aiScene* scene)
		{
			// Set the max bones to 100
			unsigned int numBones = mesh->mNumBones > 100 ? 100 : mesh->mNumBones;
			// For each bone
			for (unsigned int boneIndex = 0; boneIndex < numBones; ++boneIndex)
			{
				int boneID = -1;
				std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
				if (boneIndex >= m_boneProps.size()) 
				{
					m_boneProps.push_back({ boneName, aiMatrix4x4ToGlm(&mesh->mBones[boneIndex]->mOffsetMatrix) });
					boneID = boneIndex;
					m_boneCounter++;
				}
				else 
				{
					for (unsigned int i = 0; i < m_boneProps.size(); i++) 
					{
						if (m_boneProps[i].name == boneName) 
						{
							boneID = i;
							break;
						}
					}
				}

				assert(boneID != -1);

				// Get all vertex weights for current bone
				aiVertexWeight* weights = mesh->mBones[boneIndex]->mWeights;
				unsigned int numWeights = mesh->mBones[boneIndex]->mNumWeights;

				// For each weight at vertex x for current bone
				for (int weightIndex = 0; weightIndex < static_cast<int>(numWeights); ++weightIndex)
				{
					unsigned int vertexId = weights[weightIndex].mVertexId;
					float weight = weights[weightIndex].mWeight;
					assert(vertexId <= boneIDs_all.size());

					// Update four most influential bones
					for (int i = 0; i < 4; ++i)
					{
						if (boneIDs_all[vertexId][i] < 0)
						{
							weights_all[vertexId][i] = weight;
							boneIDs_all[vertexId][i] = boneID;
							break;
						}
					}
				}
			}
		}

		void SkinnedModel::ProcessNode(aiNode* node, const aiScene* scene)
		{
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				m_meshes.push_back(ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene));
			}

			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				ProcessNode(node->mChildren[i], scene);
			}
		}



		SharedPtr<Mesh> SkinnedModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
		{
			// Mesh to fill with data
			std::vector<Vertex> vertices;

			size_t vertexCount = mesh->mNumVertices;

			std::vector<Vector4Int> boneIdList;
			std::vector<Vector4> boneWeightList;

			boneIdList.resize(vertexCount,Vector4Int(-1));
			boneWeightList.resize(vertexCount, Vector4(0.0f));

			// Load boneIDs and weights for each vertex
			ExtractBoneWeightForVertices(boneIdList, boneWeightList, mesh, scene);


			bool useBoneIds = boneIdList.size() == vertexCount;
			bool useBoneWeights = boneWeightList.size() == vertexCount;

			// Loop all vertices in loaded mesh
			for (unsigned int i = 0; i < vertexCount; i++)
			{
				Vertex vertex;

				Vector4Int boneIDs;
				Vector4 weights;

				// Set default values
				for (int i = 0; i < 4; i++)
				{
					boneIDs[i] = -1;
					weights[i] = 0.0f;
				}

				vertex.boneIds = boneIDs;
				boneIdList.push_back(boneIDs);
				
				vertex.boneWeights = weights;
				boneWeightList.push_back(weights);


				Vector3 vector;
				// Set positions
				vector.x = mesh->mVertices[i].x;
				vector.y = mesh->mVertices[i].y;
				vector.z = mesh->mVertices[i].z;

				vertex.position = vector;
				

				if (mesh->HasNormals())
				{
					// Set normals
					vector.x = mesh->mNormals[i].x;
					vector.y = mesh->mNormals[i].y;
					vector.z = mesh->mNormals[i].z;
					vertex.normal = (vector);
				}

				if (mesh->mTextureCoords[0])
				{
					// Set texture coords
					Vector2 vec;
					vec.x = mesh->mTextureCoords[0][i].x;
					vec.y = mesh->mTextureCoords[0][i].y;
					vertex.texCoord = (vec);

					if (mesh->HasTangentsAndBitangents()) 
					{
						// Set tangent
						vector.x = mesh->mTangents[i].x;
						vector.y = mesh->mTangents[i].y;
						vector.z = mesh->mTangents[i].z;
						vertex.tangent = (vector);
						// Set bitangent
						vector.x = mesh->mBitangents[i].x;
						vector.y = mesh->mBitangents[i].y;
						vector.z = mesh->mBitangents[i].z;
						vertex.bitangent = (vector);
					}
				}

				if (useBoneIds)
				{
					vertex.boneIds = boneIdList[i];
				}

				if (useBoneWeights)
				{
					vertex.boneWeights = boneWeightList[i];
				}

				vertices.push_back(vertex);
				
			}

			std::vector<uint32_t> indicies;

			// Set indices
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++)
					indicies.push_back(face.mIndices[j]);
			}

			
			

			


			LOG_TRACE("Processed: {0} Bones || Triangle count : {1}", mesh->mNumBones, boneIdList.size());


			SharedPtr<Mesh> rendermesh = MakeShared<Mesh>(vertices, indicies);

			return rendermesh;
		}


		
		

	}

	
}