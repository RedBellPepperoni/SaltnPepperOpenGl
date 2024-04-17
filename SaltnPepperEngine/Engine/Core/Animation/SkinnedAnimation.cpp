#include "SkinnedAnimation.h"

namespace SaltnPepperEngine
{
	SkinnedAnimation::SkinnedAnimation(const std::string& animationPath, SharedPtr<SkinnedModel>& model)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
		assert(scene && scene->mRootNode);
		if (scene->mNumAnimations == 0)
		{
			return;
		}

		aiAnimation* animation = scene->mAnimations[0];
		m_duration = (float)animation->mDuration;
		m_tps = (float)animation->mTicksPerSecond;
		GenerateBoneTree(&m_rootNode, scene->mRootNode);

		// Reset all root transformations
		m_rootNode.transformation = Matrix4(1.0f);
		LoadIntermediateBones(animation, model);

		
	}

	SkinnedAnimation::~SkinnedAnimation()
	{
	}

	 Bone* SkinnedAnimation::FindBone(const std::string& BoneName)
	{
		/*for (unsigned int i = 0; i < m_bones.size(); i++) 
		{
			if (m_bones[i].getBoneName() == BoneName)
			{
				return &m_bones[i];
			}
		}*/


		const BoneMap::iterator& iterator = m_boneMap.find(BoneName);

		if (iterator != m_boneMap.end())
		{
			 Bone* bone = &iterator->second;
			 return bone;
		}


		return nullptr;
	}

	void SkinnedAnimation::LoadIntermediateBones(const aiAnimation* animation, SharedPtr<SkinnedModel>& model)
	{
		std::vector<BoneProps>& boneProps = model->GetBoneProps();

		for (unsigned int i = 0; i < animation->mNumChannels; i++)
		{
			auto channel = animation->mChannels[i];
			std::string boneName = channel->mNodeName.data;
			int boneId = -1;

			for (unsigned int i = 0; i < boneProps.size(); i++) {
				if (boneProps[i].name == boneName) {
					boneId = i;
					break;
				}
			}

			if (boneProps.size() < 100) {
				if (boneId == -1) {
					BoneProps boneProp;
					boneProp.name = boneName;
					boneProps.push_back(boneProp);
					boneId = (int)boneProps.size() - 1;
				}
			}
			//m_bones.push_back(Bone(channel->mNodeName.data, boneId, channel));

			Bone bone = Bone{ channel->mNodeName.data, boneId, channel };
			m_boneMap.emplace(bone.getBoneName(),bone);

		}

		this->m_boneProps = boneProps;
	}

	void SkinnedAnimation::GenerateBoneTree(AssimpNodeData* parent, const aiNode* src)
	{
		assert(src);

		parent->name = src->mName.data;
		parent->transformation = aiMatrix4x4ToGlm(&src->mTransformation);
		parent->childrenCount = src->mNumChildren;

		for (unsigned int i = 0; i < src->mNumChildren; i++)
		{
			AssimpNodeData newData;
			GenerateBoneTree(&newData, src->mChildren[i]);
			parent->children.push_back(newData);
		}
	}
	
}