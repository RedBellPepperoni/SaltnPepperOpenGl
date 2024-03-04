#ifndef SKINNED_MESH_H
#define SKINNED_MESH_H
#include "Engine/Core/Rendering/Buffers/VertexArray.h"
#include "Engine/Core/Rendering/Material/Material.h"
#include "Mesh.h"
#include <map>
#include <unordered_map>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> 

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

namespace SaltnPepperEngine
{
	namespace Rendering
	{


		class SkinnedMesh 
		{
		public:

			SkinnedMesh(const std::string filePath); 
			virtual ~SkinnedMesh();

			bool LoadMesh(const std::string filepath);

			void Render();

			int NumBones() const { return (int)m_BoneNameToIndexMap.size(); }
			
			SharedPtr<Material>& GetMaterial() 
			{
				for (unsigned int i = 0; i < m_Materials.size(); i++) {
					if (m_Materials[i]->albedoColour != Vector4(0.0f, 0.0f, 0.0f,0.0f)) {
						return m_Materials[i];
					}
				}
				
				return m_Materials[0]; 
			}

			void GetBoneTransforms(float TimeInSeconds, std::vector<Matrix4>& Transforms);

			

			
		protected:

#define MAX_NUM_BONES_PER_VERTEX 4

			inline void AssimpToGLM(const aiMatrix4x4& a, Matrix4& g)
			{

				g[0][0] = a.a1; g[0][1] = a.b1; g[0][2] = a.c1; g[0][3] = a.d1;
				g[1][0] = a.a2; g[1][1] = a.b2; g[1][2] = a.c2; g[1][3] = a.d2;
				g[2][0] = a.a3; g[2][1] = a.b3; g[2][2] = a.c3; g[2][3] = a.d3;
				g[3][0] = a.a4; g[3][1] = a.b4; g[3][2] = a.c4; g[3][3] = a.d4;

			}

			void Clear();

			bool InitFromScene(const aiScene* pScene, const std::string& Filename);

			void CountVerticesAndIndices(const aiScene* pScene, unsigned int& NumVertices, unsigned int& NumIndices);

			void ReserveSpace(unsigned int NumVertices, unsigned int NumIndices);

			void InitAllMeshes(const aiScene* pScene);

			void InitSingleMesh(int MeshIndex, const aiMesh* paiMesh);

			bool InitMaterials(const aiScene* pScene, const std::string& Filename);

			void PopulateBuffers();

			struct VertexBoneData
			{
				int BoneIDs[MAX_NUM_BONES_PER_VERTEX] = { 0 };
				float Weights[MAX_NUM_BONES_PER_VERTEX] = { 0.0f };

				VertexBoneData()
				{
				}

				void AddBoneData(int BoneID, float Weight)
				{
					for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(BoneIDs); i++) {
						if (Weights[i] == 0.0) {
							BoneIDs[i] = BoneID;
							Weights[i] = Weight;
							//printf("Adding bone %d weight %f at index %i\n", BoneID, Weight, i);
							return;
						}
					}

					// should never get here - more bones than we have space for
					assert(0);
				}
			};
			

			void LoadMeshBones(int MeshIndex, const aiMesh* pMesh);
			void LoadSingleBone(int MeshIndex, const aiBone* pBone);
			int GetBoneId(const aiBone* pBone);
			void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
			void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
			void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
			int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
			int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
			int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
			const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string& NodeName);
			void ReadNodeHierarchy(float AnimationTime, const aiNode* pNode, const Matrix4& ParentTransform);


			enum BUFFER_TYPE {
				INDEX_BUFFER = 0,
				POS_VB = 1,
				TEXCOORD_VB = 2,
				NORMAL_VB = 3,
				BONE_VB = 4,
				NUM_BUFFERS = 5
			};

			SharedPtr<VertexArray> skinnedVAO;
			GLuint m_Buffers[NUM_BUFFERS] = { 0 };

#define INVALID_MATERIAL 0xFFFFFFFF
			struct BasicMeshEntry {
				BasicMeshEntry()
				{
					NumIndices = 0;
					BaseVertex = 0;
					BaseIndex = 0;
					MaterialIndex = INVALID_MATERIAL;
				}

				unsigned int NumIndices;
				unsigned int BaseVertex;
				unsigned int BaseIndex;
				unsigned int MaterialIndex;
			};

			Assimp::Importer Importer;
			const aiScene* pScene = NULL;
			std::vector<BasicMeshEntry> m_Meshes;
			std::vector<SharedPtr<Material>> m_Materials;

			vector<Vector3> m_Positions;
			vector<Vector3> m_Normals;
			vector<Vector2> m_TexCoords;
			vector<unsigned int> m_Indices;
			vector<VertexBoneData> m_Bones;

			map<std::string, int> m_BoneNameToIndexMap;

			struct BoneInfo
			{
				Matrix4 OffsetMatrix;
				Matrix4 FinalTransformation;

				BoneInfo(const Matrix4& Offset)
				{
					OffsetMatrix = Offset;
					FinalTransformation = Matrix4(0.0f);
				};
			};

			vector<BoneInfo> m_BoneInfo;
			Matrix4 m_GlobalInverseTransform;
		};
	}

}

#endif // !SKINNED_MESH_H


