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

		void SkinnedMesh::GetBoneTransforms(float TimeInSeconds, vector<Matrix4>& Transforms)
		{
			Matrix4 Identity = Matrix4();
			
			float TicksPerSecond = (float)(pScene->mAnimations[0]->mTicksPerSecond != 0 ? pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
			float TimeInTicks = TimeInSeconds * TicksPerSecond;
			float AnimationTimeTicks = fmod(TimeInTicks, (float)pScene->mAnimations[0]->mDuration);

			ReadNodeHierarchy(AnimationTimeTicks, pScene->mRootNode, Identity);
			Transforms.resize(m_BoneInfo.size());

			for (int i = 0; i < (int)m_BoneInfo.size(); i++)
			{
				Transforms[i] = m_BoneInfo[i].FinalTransformation;
			}
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
			for (int i = 0; i < (int)pMesh->mNumBones; i++) 
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

		int SkinnedMesh::GetBoneId(const aiBone* pBone)
		{
			int BoneIndex = 0;
			string BoneName(pBone->mName.C_Str());

			if (m_BoneNameToIndexMap.find(BoneName) == m_BoneNameToIndexMap.end()) {
				// Allocate an index for a new bone
				BoneIndex = (int)m_BoneNameToIndexMap.size();
				m_BoneNameToIndexMap[BoneName] = BoneIndex;
			}
			else {
				BoneIndex = m_BoneNameToIndexMap[BoneName];
			}

			return BoneIndex;
		}

		void SkinnedMesh::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
		{
			// we need at least two values to interpolate...
			if (pNodeAnim->mNumScalingKeys == 1) {
				Out = pNodeAnim->mScalingKeys[0].mValue;
				return;
			}

			int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
			int NextScalingIndex = ScalingIndex + 1;
			assert(NextScalingIndex < (int)pNodeAnim->mNumScalingKeys);
			float t1 = (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime;
			float t2 = (float)pNodeAnim->mScalingKeys[NextScalingIndex].mTime;
			float DeltaTime = t2 - t1;
			float Factor = (AnimationTime - (float)t1) / DeltaTime;
			assert(Factor >= 0.0f && Factor <= 1.0f);
			const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
			const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
			aiVector3D Delta = End - Start;
			Out = Start + Factor * Delta;
		}

		void SkinnedMesh::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
		{
			// we need at least two values to interpolate...
			if (pNodeAnim->mNumRotationKeys == 1) {
				Out = pNodeAnim->mRotationKeys[0].mValue;
				return;
			}

			int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
			int NextRotationIndex = RotationIndex + 1;
			assert(NextRotationIndex < (int)pNodeAnim->mNumRotationKeys);
			float t1 = (float)pNodeAnim->mRotationKeys[RotationIndex].mTime;
			float t2 = (float)pNodeAnim->mRotationKeys[NextRotationIndex].mTime;
			float DeltaTime = t2 - t1;
			float Factor = (AnimationTime - t1) / DeltaTime;
			assert(Factor >= 0.0f && Factor <= 1.0f);
			const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
			const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
			aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
			Out.Normalize();
		}

		void SkinnedMesh::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
		{
			// we need at least two values to interpolate...
			if (pNodeAnim->mNumPositionKeys == 1) {
				Out = pNodeAnim->mPositionKeys[0].mValue;
				return;
			}

			int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
			int NextPositionIndex = PositionIndex + 1;
			assert(NextPositionIndex < (int)pNodeAnim->mNumPositionKeys);
			float t1 = (float)pNodeAnim->mPositionKeys[PositionIndex].mTime;
			float t2 = (float)pNodeAnim->mPositionKeys[NextPositionIndex].mTime;
			float DeltaTime = t2 - t1;
			float Factor = (AnimationTime - t1) / DeltaTime;
			assert(Factor >= 0.0f && Factor <= 1.0f);
			const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
			const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
			aiVector3D Delta = End - Start;
			Out = Start + Factor * Delta;
		}

		int SkinnedMesh::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
		{
			assert(pNodeAnim->mNumScalingKeys > 0);

			for (int i = 0; i < (int)pNodeAnim->mNumScalingKeys - 1; i++) 
			{
				float t = (float)pNodeAnim->mScalingKeys[i + 1].mTime;
				if (AnimationTime < t)
				{
					return i;
				}
			}

			return 0;
		}

		int SkinnedMesh::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
		{
			assert(pNodeAnim->mNumRotationKeys > 0);

			for (int i = 0; i < (int)pNodeAnim->mNumRotationKeys - 1; i++) {
				float t = (float)pNodeAnim->mRotationKeys[i + 1].mTime;

				if (AnimationTime < t) 
				{
					return i;
				}
			}

			return 0;
		}

		int SkinnedMesh::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
		{
			for (int i = 0; i < (int)pNodeAnim->mNumPositionKeys - 1; i++) 
			{
				float t = (float)pNodeAnim->mPositionKeys[i + 1].mTime;

				if (AnimationTime < t) 
				{
					return i;
				}
			}

			return 0;
		}

		const aiNodeAnim* SkinnedMesh::FindNodeAnim(const aiAnimation* pAnimation, const string& NodeName)
		{
			for (int i = 0; i < (int)pAnimation->mNumChannels; i++) {
				const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

				if (string(pNodeAnim->mNodeName.data) == NodeName) {
					return pNodeAnim;
				}
			}

			return NULL;
		}

		void SkinnedMesh::ReadNodeHierarchy(float AnimationTime, const aiNode* pNode, const Matrix4& ParentTransform)
		{
			string NodeName(pNode->mName.data);

			const aiAnimation* pAnimation = pScene->mAnimations[0];

			Matrix4 NodeTransformation;

			AssimpToGLM(pNode->mTransformation, NodeTransformation);

			const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

			if (pNodeAnim) {
				// Interpolate scaling and generate scaling transformation matrix
				aiVector3D Scaling;
				CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
				Matrix4 ScalingM = Matrix4(1.0f);
				Scale(ScalingM, Vector3(Scaling.x, Scaling.y, Scaling.z));


				// Interpolate rotation and generate rotation transformation matrix
				aiQuaternion RotationQ;
				CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);

				Quaternion quat = Quaternion(RotationQ.x, RotationQ.y, RotationQ.z, RotationQ.w);

				Matrix4 RotationM = QuatToMatrix(quat);

				// Interpolate translation and generate translation transformation matrix
				aiVector3D Translation;
				CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
				Matrix4 TranslationM = Matrix4(1.0f);;
				Translate(TranslationM, Vector3(Translation.x, Translation.y, Translation.z));

				// Combine the above transformations
				NodeTransformation = TranslationM * RotationM * ScalingM;
			}

			Matrix4 GlobalTransformation = ParentTransform * NodeTransformation;

			if (m_BoneNameToIndexMap.find(NodeName) != m_BoneNameToIndexMap.end()) {
				int BoneIndex = m_BoneNameToIndexMap[NodeName];
				m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].OffsetMatrix;
			}

			for (int i = 0; i < (int)pNode->mNumChildren; i++) {
				ReadNodeHierarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
			}
		}

}
}