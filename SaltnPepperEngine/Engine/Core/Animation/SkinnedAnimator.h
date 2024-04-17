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

		void PlayAnimation(SkinnedAnimation* animation, bool repeat = true);
		void PlayAnimationbyName(const std::string& name, bool repeat = true);
	
		void AddAnimation(const std::string& name, SkinnedAnimation* animref);

		SkinnedAnimation* GetAnimationByName(const std::string& name);

		void CalculateBoneTranslation(const AssimpNodeData* curNode, Matrix4 parentTransform, SkinnedAnimation* prevAnimation, SkinnedAnimation* nextAnimation, float haltTime, float currentTime, float transitionTime);
	
		void CalculateBoneTransform(const AssimpNodeData* node, Matrix4 parentTransform, SkinnedAnimation* animation, float currentTime);
	
		std::vector<Matrix4>& GetFinalBoneMatrices();

		void SetTransitiontime(float time) { m_transition = time; }

		SkinnedAnimation* GetCurrentAnimation();

	private:
		std::vector<glm::mat4> m_finalBoneMatrices;
		/*SharedPtr<SkinnedAnimation> m_currentAnimation = nullptr;
		SharedPtr<SkinnedAnimation> m_nextAnimation = nullptr;
		SharedPtr<SkinnedAnimation> m_queueAnimation = nullptr*/

		SkinnedAnimation* m_currentAnimation = nullptr;
		SkinnedAnimation* m_nextAnimation = nullptr;
		SkinnedAnimation* m_queueAnimation = nullptr;

		float m_currentTime;
		bool m_interpolating;
		float m_haltTime;
		float m_interTime;
		float m_transition = 0.2f;


		std::unordered_map<std::string ,SkinnedAnimation*> m_animationMap;
	};
}

#endif // !SKINNEDANIMATOR_H

