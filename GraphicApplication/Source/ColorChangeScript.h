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
	ModelComponent* modelCompRef = nullptr;
	

	/*void AddSpacemanCallBack(EventKey& callback)
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
	}*/

	void ChangeModelMan()
	{
		Vector4 oldRefColor = Vector4(1.0f);
		SharedPtr<Material> materialRef = modelCompRef->m_handle->GetMeshes()[0]->GetMaterial();
		oldRefColor = materialRef ? materialRef->albedoColour : oldRefColor;

		modelCompRef->LoadLibraryModel("LegoSpaceman");


		materialRef = modelCompRef->m_handle->GetMeshes()[0]->GetMaterial();
		materialRef->albedoColour = oldRefColor;
	}

	void ChangeModelSkeleton()
	{
		Vector4 oldRefColor = Vector4(1.0f);
		SharedPtr<Material> materialRef = modelCompRef->m_handle->GetMeshes()[0]->GetMaterial();
		oldRefColor = materialRef ? materialRef->albedoColour : oldRefColor;

		modelCompRef->LoadLibraryModel("LegoSkeleton");


		materialRef = modelCompRef->m_handle->GetMeshes()[0]->GetMaterial();
		materialRef->albedoColour = oldRefColor;
	}

	void ChangeColorWhite()
	{
		SharedPtr<Material> materialRef = modelCompRef->m_handle->GetMeshes()[0]->GetMaterial();
		if (materialRef)
		{
			materialRef->albedoColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	void ChangeColorRed()
	{
		SharedPtr<Material> materialRef = modelCompRef->m_handle->GetMeshes()[0]->GetMaterial();
		if (materialRef)
		{
			materialRef->albedoColour = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		}
	}

	void ChangeColorYellow()
	{
		SharedPtr<Material> materialRef = modelCompRef->m_handle->GetMeshes()[0]->GetMaterial();
		if (materialRef)
		{
			materialRef->albedoColour = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
		}
	}

	void ChangeColorGreen()
	{
		SharedPtr<Material> materialRef = modelCompRef->m_handle->GetMeshes()[0]->GetMaterial();
		if (materialRef)
		{
			materialRef->albedoColour = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		}
	}
};