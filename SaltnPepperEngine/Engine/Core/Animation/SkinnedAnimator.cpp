#include "SkinnedAnimator.h"
#include "Engine/Utils/Logging/Log.h"

namespace SaltnPepperEngine
{
	SkinnedAnimator::SkinnedAnimator()
	{
		m_currentTime = 0.0;
		m_interpolating = false;
		m_haltTime = 0.0;
		m_interTime = 0.0;

		m_currentAnimation = nullptr;
		m_nextAnimation = nullptr;
		m_queueAnimation = nullptr;

		m_finalBoneMatrices.resize(100, Matrix4(1.0f));

		//m_finalBoneMatrices.reserve(100);

		/*for (int i = 0; i < 100; i++)
		{
			m_finalBoneMatrices.push_back(glm::mat4(1.0f));
		}*/
	}

	SkinnedAnimator::~SkinnedAnimator()
	{
		
	}

	void SkinnedAnimator::OnUpdate(const float& deltatime)
	{
		

		if (m_currentAnimation) {
			m_currentTime = fmod(m_currentTime + m_currentAnimation->GetTicksPerSecond() * deltatime, m_currentAnimation->GetDuration());

			float transitionTime = m_currentAnimation->GetTicksPerSecond() * 0.2f;
			if (m_interpolating && m_interTime <= transitionTime) {
				m_interTime += m_currentAnimation->GetTicksPerSecond() * deltatime;
				CalculateBoneTranslation(m_currentAnimation->getRootNode(), glm::mat4(1.0f), m_currentAnimation, m_nextAnimation, m_haltTime, m_interTime, transitionTime);
				return;
			}
			else if (m_interpolating) 
			{
				if (m_queueAnimation)
				{
					m_currentAnimation = m_nextAnimation;
					m_haltTime = 0.0f;
					m_nextAnimation = m_queueAnimation;
					m_queueAnimation = nullptr;
					m_currentTime = 0.0f;
					m_interTime = 0.0;
					return;
				}

				m_interpolating = false;
				m_currentAnimation = m_nextAnimation;
				m_currentTime = 0.0;
				m_interTime = 0.0;
			}

			CalculateBoneTransform(m_currentAnimation->getRootNode(), glm::mat4(1.0f), m_currentAnimation, m_currentTime);
		}
	}

	void SkinnedAnimator::PlayAnimation(SharedPtr<SkinnedAnimation>& animation, bool repeat)
	{
		if (!m_currentAnimation) {
			m_currentAnimation = animation;
			return;
		}

		if (m_interpolating) {
			// Handle interpolating from current interpolation here
			if (animation != m_nextAnimation)
				m_queueAnimation = animation;
		}
		else {
			// Else: Just playing current animation
			// Start interpolation
			if (animation != m_nextAnimation) {
				m_interpolating = true;
				m_haltTime = fmod(m_currentTime, m_currentAnimation->GetDuration());
				m_nextAnimation = animation;
				m_currentTime = 0.0f;
				m_interTime = 0.0;
			}
		}
	}

	void SkinnedAnimator::PlayAnimationbyName(const std::string& name, bool repeat)
	{
		SharedPtr<SkinnedAnimation> animRef = GetAnimationByName(name);

		if (animRef != nullptr)
		{
			PlayAnimation(animRef, repeat);
		}
	}

	void SkinnedAnimator::AddAnimation(const std::string& name, SharedPtr<SkinnedAnimation> animref)
	{
		auto itr = m_animationMap.find(name);

		if (itr != m_animationMap.end())
		{
			LOG_WARN("Overwriting animation data for : {0}",name);
			
			m_animationMap[name] = animref;
			return;
		}

		m_animationMap.emplace(name, animref);

	}

	SharedPtr<SkinnedAnimation> SkinnedAnimator::GetAnimationByName(const std::string& name)
	{
		auto itr = m_animationMap.find(name);

		if (itr == m_animationMap.end())
		{
			LOG_WARN("No animation found with name : {0}", name);

			return nullptr;
		}

		return itr->second;
	}

	void SkinnedAnimator::CalculateBoneTranslation(const AssimpNodeData* curNode, Matrix4 parentTransform, SharedPtr<SkinnedAnimation>& prevAnimation, SharedPtr<SkinnedAnimation>& nextAnimation, float haltTime, float currentTime, float transitionTime)
	{
		std::string nodeName = curNode->name;
		glm::mat4 transform = curNode->transformation;

		Bone* prevBone = prevAnimation->FindBone(nodeName);
		Bone* nextBone = nextAnimation->FindBone(nodeName);

		if (prevBone && nextBone)
		{
			KeyPosition prevPos = prevBone->getPositions(haltTime);
			KeyRotation prevRot = prevBone->getRotations(haltTime);
			KeyScale prevScl = prevBone->getScalings(haltTime);

			KeyPosition nextPos = nextBone->getPositions(0.0f);
			KeyRotation nextRot = nextBone->getRotations(0.0f);
			KeyScale nextScl = nextBone->getScalings(0.0f);

			prevPos.timeStamp = 0.0f;
			prevRot.timeStamp = 0.0f;
			prevScl.timeStamp = 0.0f;

			nextPos.timeStamp = transitionTime;
			nextRot.timeStamp = transitionTime;
			nextScl.timeStamp = transitionTime;

			glm::mat4 p = interpolatePosition(currentTime, prevPos, nextPos);
			glm::mat4 r = interpolateRotation(currentTime, prevRot, nextRot);
			glm::mat4 s = interpolateScaling(currentTime, prevScl, nextScl);

			transform = p * r * s;
		}

		glm::mat4 globalTransformation = parentTransform * transform;

		auto boneProps = nextAnimation->getBoneProps();
		for (unsigned int i = 0; i < boneProps.size(); i++) {
			if (boneProps[i].name == nodeName) {
				glm::mat4 offset = boneProps[i].offset;
				m_finalBoneMatrices[i] = globalTransformation * offset;
				break;
			}
		}

		for (int i = 0; i < curNode->childrenCount; i++)
			CalculateBoneTranslation(&curNode->children[i], globalTransformation, prevAnimation, nextAnimation, haltTime, currentTime, transitionTime);
	}

	void SkinnedAnimator::CalculateBoneTransform(const AssimpNodeData* node, Matrix4 parentTransform, SharedPtr<SkinnedAnimation>& animation, float currentTime)
	{
		std::string nodeName = node->name;
		glm::mat4 boneTransform = node->transformation;

		Bone* bone = animation->FindBone(nodeName);

		if (bone)
		{
			bone->update(currentTime);
			boneTransform = bone->getTransform();
		}

		glm::mat4 globalTransformation = parentTransform * boneTransform;

		auto boneProps = animation->getBoneProps();

		for (unsigned int i = 0; i < boneProps.size(); i++) {
			if (boneProps[i].name == nodeName) {
				glm::mat4 offset = boneProps[i].offset;
				m_finalBoneMatrices[i] = globalTransformation * offset;
				break;
			}
		}

		for (int i = 0; i < node->childrenCount; i++)
		{
			CalculateBoneTransform(&node->children[i], globalTransformation, animation, currentTime);
		}
	}

	std::vector<Matrix4>& SkinnedAnimator::GetFinalBoneMatrices()
	{
		return m_finalBoneMatrices;
	}
}