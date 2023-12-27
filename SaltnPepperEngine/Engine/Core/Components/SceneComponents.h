#ifndef SCENECOMPONENTS_H
#define SCENECOMPONENTS_H

#include <string>
#include "Engine/Utils/UniqueId/UniqueId.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Utils/Logging/Log.h"

namespace SaltnPepperEngine
{

	//Forward Declaration
	namespace Rendering
	{
		class Mesh;
		class Material;
	}

	using Rendering::Mesh;
	using Rendering::Material;

	namespace Components
	{


		struct IdComponent
		{
			// The Unique Identifier
			UniqueId ID;

			//Can Add saving and loading here
		};



		struct ActiveComponent
		{
			ActiveComponent()
			{
				active = true;
			}

			ActiveComponent(bool newActive)
			{
				active = newActive;
			}

			bool active = true;
		};


		struct NameComponent
		{
			std::string name = "";
		};


		struct MeshComponent
		{


			MeshComponent();
			MeshComponent(const SharedPtr<Mesh>& meshRef);
			const SharedPtr<Mesh>& GetMesh();
			

			SharedPtr<Mesh> m_handle;
			bool isVisible = true;
			bool castsShadow = true;

		};


		struct MeshRenderer
		{

			using MaterialRef = SharedPtr<Material>;
			using MaterialList = std::vector<MaterialRef>;




			MeshRenderer();
			MeshRenderer(MaterialRef material);
			MeshRenderer(MaterialList materials);
	

			MaterialRef GetMaterial();
			

			MaterialList GetMaterials() const;
		

			MaterialRef GetMaterial(int index) const;
			

		private:

			MaterialList m_Materials;

		};

	}

}



#endif // !SCENECOMPONENTS_H
