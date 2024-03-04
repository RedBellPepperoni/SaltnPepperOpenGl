#include "SkinnedModel.h"

namespace SaltnPepperEngine
{
	SkinnedModel::SkinnedModel(const FilePath& filepath)
	{
		Load(filepath);
	}

	std::vector<SharedPtr<SkinnedMesh>>& SkinnedModel::GetMeshes()
	{
		return m_meshes;
	}

	void SkinnedModel::Load(const FilePath& filepath)
	{
		// fetching the model details from the given filepath
		//ModelDetail loadedmodelDetail = ModelLoader::LoadSkinnedModel(filepath);

		//int meshCount = 0;

		//// Incase there are sub meshes we generate a mesh for each of them
		//for (MeshDetails meshDetail : loadedmodelDetail.meshes)
		//{

		//	// Creating a new Mesh from the gathered data
		//	SharedPtr<SkinnedMesh> newMesh = MakeShared<SkinnedMesh>(meshDetail.skinnedVertices, meshDetail.indices, meshDetail.boneData, meshDetail.BoneNameToIndexMap);
		//	newMesh->SetName(FileSystem::GetFileName(filepath) + std::to_string(meshCount));

		//	newMesh->SetBoneTransforms(meshDetail.boneTransforms);
		//	// Adding the generated mesh to the Model List of meshes
		//	m_meshes.push_back(newMesh);

		//	meshCount++;
		//}


	}
	const std::string& SkinnedModel::GetFilePath() const
	{
		return m_filePath;
	}
}
