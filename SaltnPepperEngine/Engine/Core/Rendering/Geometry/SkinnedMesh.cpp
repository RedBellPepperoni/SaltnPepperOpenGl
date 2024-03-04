#include "SkinnedMesh.h"
#include "Engine/Core/Resources/ResourceManager.h"
#include "Engine/Core/Rendering/Material/Material.h"
#include "Engine/Utils/Logging/Log.h"
#include "Engine/Utils/GLUtils.h"


namespace SaltnPepperEngine
{
	namespace Rendering
	{
#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   1
#define NORMAL_LOCATION      2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4


		SkinnedMesh::SkinnedMesh()
		{
		}

		SkinnedMesh::~SkinnedMesh()
		{
			Clear();
		}

		bool SkinnedMesh::LoadMesh(const std::string filepath)
		{
			// Release the previously loaded mesh (if it exists)
			Clear();

			// Create the VAO
			skinnedVAO = MakeShared<VertexArray>();
			skinnedVAO->Bind();

			// Create the buffers for the vertices attributes
			glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

			bool success = false;

			pScene = Importer.ReadFile(filepath.c_str(), (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices));

			if (pScene) {

				AssimpToGLM(pScene->mRootNode->mTransformation, m_GlobalInverseTransform);
				m_GlobalInverseTransform = Inverse(m_GlobalInverseTransform);
				success = InitFromScene(pScene, filepath);
			}
			else 
			{
				LOG_ERROR("Error parsing {0} : {1}",filepath.c_str(), Importer.GetErrorString());
			}

			// Make sure the VAO is not changed from the outside
			skinnedVAO->UnBind();

			return success;
		}

		void SkinnedMesh::Render()
		{
		}

		void SkinnedMesh::GetBoneTransforms(float AnimationTimeSec, vector<Matrix4>& Transforms)
		{
		}

		void SkinnedMesh::Clear()
		{
			if (m_Buffers[0] != 0) {
				glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);
			}

		}

		bool SkinnedMesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
		{
			m_Meshes.resize(pScene->mNumMeshes);
			//m_Materials.resize(pScene->mNumMaterials);

			unsigned int NumVertices = 0;
			unsigned int NumIndices = 0;

			GLDEBUG(CountVerticesAndIndices(pScene, NumVertices, NumIndices)); 

			GLDEBUG(ReserveSpace(NumVertices, NumIndices));

			GLDEBUG(InitAllMeshes(pScene));

			if (!InitMaterials(pScene, Filename)) {
					return false;
			} 

			GLDEBUG(PopulateBuffers());

			return true;
		}

		void SkinnedMesh::CountVerticesAndIndices(const aiScene* pScene, unsigned int& NumVertices, unsigned int& NumIndices)
		{
			for (unsigned int i = 0; i < m_Meshes.size(); i++) {
				m_Meshes[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
				m_Meshes[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
				m_Meshes[i].BaseVertex = NumVertices;
				m_Meshes[i].BaseIndex = NumIndices;

				NumVertices += pScene->mMeshes[i]->mNumVertices;
				NumIndices += m_Meshes[i].NumIndices;
			}
		}

		void SkinnedMesh::ReserveSpace(unsigned int NumVertices, unsigned int NumIndices)
		{
			m_Positions.reserve(NumVertices);
			m_Normals.reserve(NumVertices);
			m_TexCoords.reserve(NumVertices);
			m_Indices.reserve(NumIndices);
			m_Bones.resize(NumVertices);
		}

		void SkinnedMesh::InitAllMeshes(const aiScene* pScene)
		{
			for (unsigned int i = 0; i < m_Meshes.size(); i++)
			{
				const aiMesh* paiMesh = pScene->mMeshes[i];
				InitSingleMesh(i, paiMesh); 
			}
		}

		void SkinnedMesh::InitSingleMesh(int MeshIndex, const aiMesh* paiMesh)
		{
			const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

			// Populate the vertex attribute vectors
			for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {

				const aiVector3D& pPos = paiMesh->mVertices[i];
				m_Positions.push_back(Vector3(pPos.x, pPos.y, pPos.z));

				if (paiMesh->mNormals) {
					const aiVector3D& pNormal = paiMesh->mNormals[i];
					m_Normals.push_back(Vector3(pNormal.x, pNormal.y, pNormal.z));
				}
				else {
					aiVector3D Normal(0.0f, 1.0f, 0.0f);
					m_Normals.push_back(Vector3(Normal.x, Normal.y, Normal.z));
				}

				const aiVector3D& pTexCoord = paiMesh->HasTextureCoords(0) ? paiMesh->mTextureCoords[0][i] : Zero3D;
				m_TexCoords.push_back(Vector2(pTexCoord.x, pTexCoord.y));
			}

			LoadMeshBones(MeshIndex, paiMesh);

			// Populate the index buffer
			for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
				const aiFace& Face = paiMesh->mFaces[i];
				//        printf("num indices %d\n", Face.mNumIndices);
				//        assert(Face.mNumIndices == 3);
				m_Indices.push_back(Face.mIndices[0]);
				m_Indices.push_back(Face.mIndices[1]);
				m_Indices.push_back(Face.mIndices[2]);
			}
		}

		bool SkinnedMesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
		{
			m_Materials.clear();
			
			bool success = true;

			// Initialize the materials
			for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
			{
				SharedPtr<Material> mat = MakeShared<Material>();

				m_Materials.push_back(mat);
				
			}

			return success;
		}

		void SkinnedMesh::PopulateBuffers()
		{
			GLDEBUG(glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]));
			GLDEBUG(glBufferData(GL_ARRAY_BUFFER, sizeof(m_Positions[0]) * m_Positions.size(), &m_Positions[0], GL_STATIC_DRAW));
			GLDEBUG(glEnableVertexAttribArray(POSITION_LOCATION));
			GLDEBUG(glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0));

			GLDEBUG(glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]));
			GLDEBUG(glBufferData(GL_ARRAY_BUFFER, sizeof(m_TexCoords[0]) * m_TexCoords.size(), &m_TexCoords[0], GL_STATIC_DRAW));
			GLDEBUG(glEnableVertexAttribArray(TEX_COORD_LOCATION));
			GLDEBUG(glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0));

			GLDEBUG(glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]));
			GLDEBUG(glBufferData(GL_ARRAY_BUFFER, sizeof(m_Normals[0]) * m_Normals.size(), &m_Normals[0], GL_STATIC_DRAW));
			GLDEBUG(glEnableVertexAttribArray(NORMAL_LOCATION));
			GLDEBUG(glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0));

			GLDEBUG(glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]));
			GLDEBUG(glBufferData(GL_ARRAY_BUFFER, sizeof(m_Bones[0]) * m_Bones.size(), &m_Bones[0], GL_STATIC_DRAW));
			GLDEBUG(glEnableVertexAttribArray(BONE_ID_LOCATION));
			GLDEBUG(glVertexAttribIPointer(BONE_ID_LOCATION, MAX_NUM_BONES_PER_VERTEX, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0));
			GLDEBUG(glEnableVertexAttribArray(BONE_WEIGHT_LOCATION));
			GLDEBUG(glVertexAttribPointer(BONE_WEIGHT_LOCATION, MAX_NUM_BONES_PER_VERTEX, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData),
				(const GLvoid*)(MAX_NUM_BONES_PER_VERTEX * sizeof(int32_t))));

			GLDEBUG(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]));
			GLDEBUG(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_Indices[0]) * m_Indices.size(), &m_Indices[0], GL_STATIC_DRAW));
		}

		void SkinnedMesh::LoadMeshBones(int MeshIndex, const aiMesh* pMesh)
		{
			for (int i = 0; i < pMesh->mNumBones; i++) 
			{
				LoadSingleBone(MeshIndex, pMesh->mBones[i]);

			}
		}

		void SkinnedMesh::LoadSingleBone(int MeshIndex, const aiBone* pBone)
		{
			int BoneId = GetBoneId(pBone);

			if (BoneId == m_BoneInfo.size())
			{
				Matrix4 offsetMat;
				AssimpToGLM(pBone->mOffsetMatrix, offsetMat);

				BoneInfo bi(offsetMat);

				m_BoneInfo.push_back(bi);
			}

			for (int i = 0; i < (int)pBone->mNumWeights; i++) 
			{
				const aiVertexWeight& vw = pBone->mWeights[i];
				int GlobalVertexID = m_Meshes[MeshIndex].BaseVertex + pBone->mWeights[i].mVertexId;
				m_Bones[GlobalVertexID].AddBoneData(BoneId, vw.mWeight);
			}
		}

}
}