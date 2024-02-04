#pragma once
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Core/Rendering/Material/Material.h"
#include "SaltnPepperEngine.h"
#include "AnimationDefinitions.h"

using namespace SaltnPepperEngine;
using namespace Rendering;

enum class AnimColor
{
	White	 = 0, // Linear
	Red		 = 1, // EaseIn
	Yellow	 = 2, // Ease Out
	Green    = 3  // Ease In Out
};


struct FunctionScript
{
	SharedPtr<Material> materialRef;
	ModelComponent* modelCompRef = nullptr;
	

	void AddSpacemanCallBack(EventKey& callback)
	{
		callback.animeventCallback = std::bind(&FunctionScript::ChangeModelMan, this);
	}

	void AddSkeletonCallBack(EventKey& callback)
	{
		callback.animeventCallback = std::bind(&FunctionScript::ChangeModelSkeleton, this);
	}

	void AddEaseInCallBack(EventKey& callback)
	{
		callback.animeventCallback = std::bind(&FunctionScript::ChangeColorRed, this);
	}

	void AddEaseOutCallBack(EventKey& callback)
	{
		callback.animeventCallback = std::bind(&FunctionScript::ChangeColorYellow, this);
	}

	void ChangeModelMan()
	{
		
		modelCompRef->LoadLibraryModel("LegoSpaceman");
		
		Vector4 oldRefColor = Vector4(1.0f);
		oldRefColor = materialRef ? materialRef->albedoColour : oldRefColor;

		materialRef = modelCompRef->m_handle->GetMeshes()[0]->GetMaterial();
		materialRef->albedoColour = oldRefColor;
	}

	void ChangeModelSkeleton()
	{
		modelCompRef->LoadLibraryModel("LegoSkeleton");

		Vector4 oldRefColor = Vector4(1.0f);
		oldRefColor = materialRef ? materialRef->albedoColour : oldRefColor;

		materialRef = modelCompRef->m_handle->GetMeshes()[0]->GetMaterial();
		materialRef->albedoColour = oldRefColor;
	}

	void ChangeColorWhite()
	{
		if (materialRef)
		{
			materialRef->albedoColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	void ChangeColorRed()
	{
		if (materialRef)
		{
			materialRef->albedoColour = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		}
	}

	void ChangeColorYellow()
	{
		if (materialRef)
		{
			materialRef->albedoColour = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
		}
	}

	void ChangeColorGreen()
	{
		if (materialRef)
		{
			materialRef->albedoColour = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		}
	}
};