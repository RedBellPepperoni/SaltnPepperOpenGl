#include "SceneComponents.h"

#include "Engine/Core/Resources/ResourceManager.h"
#include "Engine/Core/Rendering/Geometry/Primitives.h"
#include "Engine/Core/System/Application/Application.h"
#include "Engine/Core/Rendering/Camera/FlyCameraController.h"
#include "Engine/Core/Physics/PhysicsEngine/RigidBody3D.h"
#include "Engine/Core/Physics/PhysicsEngine/PhysicsEngine.h"


namespace SaltnPepperEngine
{
	//using namespace Physics;

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

		CameraControllerComponent::CameraControllerComponent()
			: m_type(ControllerType::CUSTOM)
		{
		}

		CameraControllerComponent::CameraControllerComponent(ControllerType type)
		{
			SetControllerType(type);
		}

		void CameraControllerComponent::SetControllerType(ControllerType type)
		{
			m_type = type;

			switch (type)
			{
			case ControllerType::FLYCAM: m_controllerRef = MakeShared<FlyCameraController>(); 
				break;
			case ControllerType::THIRDPERSON:
				break;
			case ControllerType::FIRSTPERSON:
				break;
			case ControllerType::CUSTOM:
				break;
			default:
				break;
			}
		}

		SharedPtr<CameraController>& CameraControllerComponent::GetController()
		{
			return m_controllerRef;
		}

		const CameraControllerComponent::ControllerType& CameraControllerComponent::GetControllerType()
		{
			return m_type;
		}

		//RigidBodyComponent::RigidBodyComponent()
		//{
		//	/*PhysicsProperties properties;
		//	properties.position = Vector3(0.0f);
		//	properties.rotation = Quaternion();
		//	properties.mass = 10.f;
		//	properties.elasticity = */

		//	m_rigidBody = Application::GetCurrent().GetPhysicsEngine()->CreateRigidBody();
		//}

		//RigidBodyComponent::RigidBodyComponent(const RigidBodyComponent& other)
		//{
		//	m_rigidBody = other.m_rigidBody;
		//	m_ownBody = other.m_ownBody;
		//}

		//RigidBodyComponent::RigidBodyComponent(const PhysicsProperties& properties)
		//{
		//	m_rigidBody = Application::GetCurrent().GetPhysicsEngine()->CreateRigidBody(properties);
		//}


		//RigidBodyComponent::~RigidBodyComponent()
		//{
		//}

		//void RigidBodyComponent::OnImgui()
		//{
		//}

		//SharedPtr<RigidBody3D> RigidBodyComponent::GetRigidBody()
		//{
		//	return m_rigidBody;
		//}

		SkinnedModelComponent::SkinnedModelComponent()
		{
			LOG_ERROR("Skinned Model Not loaded since filepath wasnt specified");
		}

		SkinnedModelComponent::SkinnedModelComponent(const std::string& filePath)
		{
			std::string clampedname = FileSystem::GetFileName(filePath);

			m_handle = Application::GetCurrent().GetSkinnedModelLibrary()->GetResource(clampedname);


			if (m_handle == nullptr)
			{
				m_handle = Application::GetCurrent().GetSkinnedModelLibrary()->LoadModel(clampedname, filePath);
			}
		}

		SkinnedModelComponent::~SkinnedModelComponent()
		{

		}

		AnimatorComponent::AnimatorComponent()
		{
			m_animator = MakeShared<SkinnedAnimator>();
		}

		AnimatorComponent::~AnimatorComponent()
		{
		}

		SharedPtr<SkinnedAnimator>& AnimatorComponent::GetAnimator()
		{
			return m_animator;
		}

		RigidBodyComponent::RigidBodyComponent()
		{
			m_rigidBody = MakeShared<RigidBody>();
		}

		SharedPtr<RigidBody>& RigidBodyComponent::GetRigidBody()
		{
			return m_rigidBody;
		}

		BoxColliderComponent::BoxColliderComponent()
		{
			m_collider = MakeShared<BoxCollider>();

			BoundingBox box{ Vector3(-0.5f),Vector3(0.5f) };
			m_collider->Init(box);
		}

		BoxColliderComponent::BoxColliderComponent(const BoundingBox& box)
		{
			m_collider = MakeShared<BoxCollider>();
			m_collider->Init(box);
		}

		BoxCollider* BoxColliderComponent::GetCollider()
		{
			return m_collider.get();
		}

}
}

