#ifndef SKINNEDANIMATOR_H
#define SKINNEDANIMATOR_H
#include <unordered_map>

#include "SkinnedAnimation.h"

namespace SaltnPepperEngine
{
	class SkinnedAnimator
	{
	public:

		SkinnedAnimator();
		~SkinnedAnimator();

		void OnUpdate(const float& deltatime);

		void PlayAnimation(SharedPtr<SkinnedAnimation>& animation, bool repeat = true);
	
		void AddAnimation(const std::string& name, SharedPtr<SkinnedAnimation> animref);

		SharedPtr<SkinnedAnimation> GetAnimationByName(const std::string& name);

		void CalculateBoneTranslation(const AssimpNodeData* curNode, Matrix4 parentTransform, SharedPtr<SkinnedAnimation>& prevAnimation, SharedPtr<SkinnedAnimation>& nextAnimation, float haltTime, float currentTime, float transitionTime);
	
		void CalculateBoneTransform(const AssimpNodeData* node, Matrix4 parentTransform, SharedPtr<SkinnedAnimation>& animation, float currentTime);
	
		std::vector<Matrix4>& GetFinalBoneMatrices();

	private:
		std::vector<glm::mat4> m_finalBoneMatrices;
		SharedPtr<SkinnedAnimation> m_currentAnimation = nullptr;
		SharedPtr<SkinnedAnimation> m_nextAnimation = nullptr;
		SharedPtr<SkinnedAnimation> m_queueAnimation = nullptr;
		float m_currentTime;
		bool m_interpolating;
		float m_haltTime;
		float m_interTime;


		std::unordered_map<std::string ,SharedPtr<SkinnedAnimation>> m_animationMap;
	};
}

#endif // !SKINNEDANIMATOR_H

