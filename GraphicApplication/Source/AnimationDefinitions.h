#pragma once

#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include <vector>
#include <string>
#include <functional>
#include <algorithm>

using namespace SaltnPepperEngine::Math;
using namespace SaltnPepperEngine;

enum class EasingType : uint8_t
{
	Linear,
	SineEaseIn,
	SineEaseOut,
	SineEaseInOut
};


struct KeyFrame
{
	KeyFrame()
		:keyData(Vector3(0.0f,0.0f,0.0f))
		, time(0.0f)
		,easeType(EasingType::Linear)
	{
		
	}

	KeyFrame(const Vector3& newData, float newTime, EasingType newEaseType = EasingType::Linear)
		: keyData(newData)
		, easeType(newEaseType)
		, time(newTime)
	{

	}

	Vector3 keyData;
	EasingType easeType;
	float time;

	// Helper Functions for sorting Key frames
	bool operator > (const KeyFrame& key) const
	{
		return (time > key.time);
	}

	bool operator < (const KeyFrame& key) const
	{
		return (time < key.time);
	}

};

//struct PositionKey
//{
//	PositionKey(const Vector3& newPosition, float newTime, EasingType newEaseType = EasingType::Linear)
//		: position(newPosition)
//		, easeType(newEaseType)
//		,	time(newTime)
//
//	{}
//	Vector3 position;
//	EasingType easeType;
//	float time;
//};
//
//
//struct RotationKey
//{
//	RotationKey(const Vector3& newRotation, float newTime, EasingType newEaseType = EasingType::Linear)
//		: rotation(newRotation)
//		, easeType(newEaseType)
//		, time(newTime)
//
//	{
//	}
//	Quaternion rotation;
//	EasingType easeType;
//	float time;
//};
//
//
//struct ScaleKey
//{
//	ScaleKey(const Vector3& newScale, float newTime, EasingType newEaseType = EasingType::Linear)
//		: scale(newScale)
//		, easeType(newEaseType)
//		, time(newTime)
//
//	{}
//	Vector3 scale;
//	EasingType easeType;
//	float time;
//};


struct EventKey
{
	float time;
	std::function<void()> animeventCallback;
	bool fired;

	// Helper Functions for sorting Key frames
	bool operator > (const EventKey& key) const
	{
		return (time > key.time);
	}

	bool operator < (const EventKey& key) const
	{
		return (time < key.time);
	}
};


struct Animation
{
	std::vector<KeyFrame> positionKeyFrameList;
	std::vector<KeyFrame> rotationKeyFrameList;
	std::vector<KeyFrame> scaleKeyFrameList;
	std::vector<EventKey> eventKeyList;

	void AddPositionKey(const Vector3& newPosition, float newTime, EasingType newEaseType = EasingType::Linear)
	{
		KeyFrame& key = positionKeyFrameList.emplace_back();
		key.keyData = newPosition;
		key.time = newTime;
		key.easeType = newEaseType;

		if (key.time > totalAnimTime)
		{
			totalAnimTime = key.time;
		}

		// Sort in ascentding order
		std::sort(positionKeyFrameList.begin(), positionKeyFrameList.end(), std::less<KeyFrame>());

	}

	void AddRotationKey(const Vector3& newRotation, float newTime, EasingType newEaseType = EasingType::Linear)
	{
		KeyFrame& key = rotationKeyFrameList.emplace_back();
		key.keyData = newRotation;
		key.time = newTime;
		key.easeType = newEaseType;

		if (key.time > totalAnimTime)
		{
			totalAnimTime = key.time;
		}

		// Sort in ascentding order
		std::sort(positionKeyFrameList.begin(), positionKeyFrameList.end(), std::less<KeyFrame>());
	}

	void AddRotationKey(const Quaternion& newRotation, float newTime, EasingType newEaseType = EasingType::Linear)
	{
		KeyFrame& key = rotationKeyFrameList.emplace_back();
		key.keyData = GetEularAnglesDegrees(newRotation);
		key.time = newTime;
		key.easeType = newEaseType;

		if (key.time > totalAnimTime)
		{
			totalAnimTime = key.time;
		}

		// Sort in ascentding order
		std::sort(positionKeyFrameList.begin(), positionKeyFrameList.end(), std::less<KeyFrame>());
	}

	void AddScaleKey(const Vector3& newScale, float newTime, EasingType newEaseType = EasingType::Linear)
	{
		KeyFrame& key = scaleKeyFrameList.emplace_back();
		key.keyData = newScale;
		key.time = newTime;
		key.easeType = newEaseType;

		if (key.time > totalAnimTime)
		{
			totalAnimTime = key.time;
		}
		// Sort in ascentding order
		std::sort(positionKeyFrameList.begin(), positionKeyFrameList.end(), std::less<KeyFrame>());
	}



	void AddAnimationEvent(float newTime, std::function<void()> function)
	{
		EventKey& key = eventKeyList.emplace_back();
		key.animeventCallback = function;
		key.time = newTime;

		if (key.time > totalAnimTime)
		{
			totalAnimTime = key.time;
		}

		// Sort in ascentding order
		std::sort(eventKeyList.begin(), eventKeyList.end(), std::less<EventKey>());
	}


	float framePerSecond = 60.0f;
	float time = 0.0f;
	float totalAnimTime = 0.0f;
	bool loop = false;



};

struct AnimationComponent
{

	std::unordered_map<std::string, SharedPtr<Animation>> animations;

	std::string currentAnim = "";

	// Will overwite if the name matches anything stored previously
	SharedPtr<Animation>& AddAnimation(const std::string& name)
	{
		// Create a new Animation at the given Key
		
		animations.emplace(name, MakeShared<Animation>());

		return animations.find(name)->second;
	}

	void RemoveAnimation(const std::string& name)
	{
		animations.erase(name);
	}

	SharedPtr<Animation> GetAnimationByName(const std::string& name)
	{
		auto itr = animations.find(name);

		if (itr == animations.end())
		{
			currentAnim = "";
			LOG_WARN("Animation [{0}] not found", name);
			return nullptr;
		}

		return itr->second;
	}

	//May need to have a actuall nullcheck here incase it doesnt exist
	SharedPtr<Animation> GetCurrentAnimation()
	{
		if (currentAnim == "") { return nullptr; }
		
		return animations.find(currentAnim)->second;
	}
	
	void PlayAnimation(const std::string& name)
	{
	
		auto itr = animations.find(name);

		if (itr  == animations.end())
		{
			currentAnim = "";
			LOG_WARN("Animation [{0}] not found", name);
			return;
		}

		currentAnim = name;

	}
};

