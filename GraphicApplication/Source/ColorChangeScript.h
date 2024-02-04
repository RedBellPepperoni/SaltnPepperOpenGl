#pragma once
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Core/Rendering/Material/Material.h"

using namespace SaltnPepperEngine;
using namespace Rendering;

enum class AnimColor
{
	White	 = 0, // Linear
	Red		 = 1, // EaseIn
	Yellow	 = 2, // Ease Out
	Green    = 3  // Ease In Out
};


struct ColorChangeScript
{
	SharedPtr<Material> materialRef;

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
			materialRef->albedoColour = Vector4(1.0f, 0.0f, 1.0f, 1.0f);
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