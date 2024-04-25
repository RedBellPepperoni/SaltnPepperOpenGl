#ifndef SKINNEDANIMATION_H
#define SKINNEDANIMATION_H

#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <string>
#include <vector>
#include <map>
#include <unordered_map>


#include "Bone.h"
#include "Engine/Core/Rendering/Geometry/SkinnedModel.h"

namespace SaltnPepperEngine
{
	using Rendering::SkinnedModel;

	struct AssimpNodeData
	{
		Matrix4 transformation;
		std::string name;
		int childrenCount;
		std::vector<AssimpNodeData> children;
	};


	class SkinnedAnimation
	{
	public:

		SkinnedAnimation(const std::string& animationPath, SharedPtr<SkinnedModel>& model);
		~SkinnedAnimation();
	
		Bone* FindBone(const std::string& BoneName);

		inline float GetTicksPerSecond() { return m_tps; }
		
		inline float GetDuration() { return m_duration; }

		inline const AssimpNodeData* getRootNode() { return &m_rootNode; }

		inline const std::vector<BoneProps>& getBoneProps() { return m_boneProps; }

		

	private:

		typedef std::unordered_map<std::string, Bone> BoneMap;

		void LoadIntermediateBones(const aiAnimation* animation, SharedPtr<SkinnedModel>& model);

		void GenerateBoneTree(AssimpNodeData* parent, const aiNode* src);

		float m_duration = 0.0f;
		float m_tps = 0.0f;
		std::vector<Bone> m_bones{};

		BoneMap m_boneMap{};

		AssimpNodeData m_rootNode;
		std::vector<BoneProps> m_boneProps;
	};
}

#endif // !SKINNEDANIMATION_H

