#include "SceneComponents.h"

#include "Engine/Core/Resources/ResourceManager.h"
#include "Engine/Core/Rendering/Geometry/Primitives.h"
#include "Engine/Core/System/Application/Application.h"

namespace SaltnPepperEngine
{
	namespace Components
	{


		MeshComponent::MeshComponent()
			:m_handle(Factory<Mesh>::Create())
		{
		}

		MeshComponent::MeshComponent(const SharedPtr<Mesh>& meshRef)
			: m_handle(meshRef)
		{
		}

		const SharedPtr<Mesh>& MeshComponent::GetMesh()
		{	
			return m_handle;		
		}





		MeshRenderer::MeshRenderer()
			:m_Materials(1, Factory<Material>::Create())
		{
		}

		MeshRenderer::MeshRenderer(MaterialRef material)
			: m_Materials(1, material)
		{
		}

		MeshRenderer::MeshRenderer(MaterialList materials)
			: m_Materials(materials)
		{
		}


		MeshRenderer::MaterialRef MeshRenderer::GetMaterial()
		{
			if (m_Materials.empty())
			{
				LOG_ERROR("No Materials Found");
				return nullptr;
			}

			return m_Materials[0];
		}

		MeshRenderer::MaterialList MeshRenderer::GetMaterials() const
		{
			return m_Materials;
		}

		MeshRenderer::MaterialRef MeshRenderer::GetMaterial(int index) const
		{
			if (m_Materials.empty())
			{
				LOG_ERROR("No Materials Found");
				return nullptr;
			}

			if (m_Materials.size() < index)
			{
				LOG_ERROR("Out of Bounds material index specifed");
				return nullptr;
			}

			return m_Materials[index];
		}



		ModelComponent::ModelComponent()
			:m_handle(MakeShared<Model>(PrimitiveType::Cube))
		{
		}

		ModelComponent::ModelComponent(const std::string& filePath)	
		{ 
			LoadLibraryModel(filePath);
		}

		ModelComponent::ModelComponent(const SharedPtr<Model>& modelRef)
			: m_handle(modelRef)
		{
		}

		

		ModelComponent::ModelComponent(PrimitiveType type)
			: m_handle(MakeShared<Model>(type))
		{
		}

		void ModelComponent::LoadLibraryModel(const std::string filePath)
		{
			std::string clampedname = FileSystem::GetFileName(filePath);

			m_handle = Application::GetCurrent().GetModelLibrary()->GetResource(clampedname);


			if (m_handle == nullptr)
			{
				m_handle = Application::GetCurrent().GetModelLibrary()->LoadModel(clampedname, filePath);
			}
		}

		void ModelComponent::LoadPrimitive(PrimitiveType type)
		{
			//m_handle = Factory<Model>::Create(type);
		}

	}
}

