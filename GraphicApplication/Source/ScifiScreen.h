#ifndef SCIFISCREEN_H
#define SCIFISCREEN_H
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Core/Rendering/Textures/Texture.h"
#include "Engine/Core/Rendering/Material/Material.h"
#include <vector>
namespace SaltnPepperEngine
{
	using namespace Rendering;

	class ScifiScreen
	{
	public:

		ScifiScreen() {};
		ScifiScreen(const float sTime)
		{
			switchTime = sTime;
		}
		~ScifiScreen() {};

		void Update(const float& deltaTime)
		{
			

			if (screenMat == nullptr) { return; }

			if (switchedOff)
			{
				timeCounter = switchTime + 1;
				return;
			}

			timeCounter += deltaTime;
			if (timeCounter < switchTime) { return; }

			timeCounter = 0.0f;
			textureindex++;

			if (textureindex >= screenTextureList.size())
			{
				textureindex = 0;
			}

			screenMat->textureMaps.albedoMap = (screenTextureList[textureindex]);
		}

		void AddTexture(SharedPtr<Texture> texture)
		{
			screenTextureList.push_back(texture);
		}

		void SetMaterialRef(SharedPtr<Material> mat)
		{
			screenMat = mat;
		}

		void SwitchActive(bool on)
		{
			switchedOff = !on;

			if (switchedOff)
			{
				screenMat->SetAlbedoTexture("Black");
				screenMat->SetMetallicTexture("Black");
			}
			else
			{
				screenMat->SetMetallicTexture("noise");
			}
		}
		
	private:



		std::vector<SharedPtr<Texture>> screenTextureList;
		SharedPtr<Material> screenMat;
		float switchTime = 4.0f;
		float timeCounter = 0.0f;

		int textureindex = 0;
		bool switchedOff = false;
	};
}


#endif // !SCIFISCREEN_H
