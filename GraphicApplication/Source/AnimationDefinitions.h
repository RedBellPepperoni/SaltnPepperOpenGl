#pragma once

#include "Engine/Utils/Maths/MathDefinitions.h"
#include <vector>
#include <functional>
#include <algorithm>

using namespace SaltnPepperEngine::Math;

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
};


struct AnimationComponent
{

	std::vector<KeyFrame> positionKeyFrameList;
	std::vector<KeyFrame> rotationKeyFrameList;
	std::vector<KeyFrame> scaleKeyFrameList;
	std::vector<EventKey> eventKeyList;



	inline void AddPositionKey(const Vector3& newPosition, float newTime, EasingType newEaseType = EasingType::Linear)
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


	template< class F, class... Args >
	void AddAnimationEvent(float newTime, F&& f, Args&&... args)
	{
		//EventKey& key = eventKeyList.emplace_back();
		//key.animeventCallback = std::bind(f, args);
		//key.time = newTime;

		//if (key.time > totalAnimTime)
		//{
		//	totalAnimTime = key.time;
		//}

		//// Sort in ascentding order
		//std::sort(positionKeyFrameList.begin(), positionKeyFrameList.end(), std::less<KeyFrame>());
	}



	float framePerSecond = 60.0f;
	float time = 0.0f;
	float totalAnimTime = 0.0f;
	bool loop = false;
	
};

