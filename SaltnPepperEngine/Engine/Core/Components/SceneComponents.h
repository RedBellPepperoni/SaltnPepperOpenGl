#ifndef SCENECOMPONENTS_H
#define SCENECOMPONENTS_H

#include <string>
#include "Engine/Core/Rendering/Geometry/Mesh.h"
#include "Engine/Core/Rendering/Geometry/Model.h"
#include "Engine/Core/Rendering/Geometry/SkinnedModel.h"
#include "Engine/Core/Rendering/Lights/Light.h"
#include "Engine/Core/Animation/SkinnedAnimator.h"
#include "Engine/Core/Rendering/Material/Material.h"
#include "Engine/Core/Rendering/Camera/CameraController.h"

//#include "Engine/Core/Physics/PhysicsSystem/RigidBody/RigidBody.h"
#include "Engine/Core/Physics/PhysicsSystem/Colliders/BoxCollider.h"
#include "Engine/Core/Physics/PhysicsSystem/Colliders/SphereCollider.h"
#include "Engine/Core/Physics/PhysicsSystem/Colliders/CapsuleCollider.h"
#include "Engine/Core/Physics/PhysicsSystem/Colliders/CylinderCollider.h"

#include "Engine/Utils/UniqueId/UniqueId.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Utils/Logging/Log.h"
#include <cereal/cereal.hpp>


#include "Engine/Core/AudioSystem/AudioSource.h"
#include "Engine/Core/AudioSystem/AudioListener.h"



namespace SaltnPepperEngine
{

	namespace Physics
	{
		struct PhysicsProperties;
		class RigidBody3D;
		class RigidBody;
	}

	namespace Audio
	{
		class AudioSource;
		class AudioListener;
	}

	using Physics::RigidBody3D;
	using Physics::PhysicsProperties;

	namespace Components
	{


		struct IdComponent
		{
			// The Unique Identifier
			UniqueId ID;

			//Can Add saving and loading here
			template <typename Archive>
			void save(Archive& archive) const
			{
				uint64_t uuid = (uint64_t)ID.GetId();
				archive(uuid);
			}

			template <typename Archive>
			void load(Archive& archive)
			{
				uint64_t uuid;
				archive(uuid);

				ID = UniqueId(uuid);
			}
		};



		struct ActiveComponent
		{
			template <typename Archive>
			void serialize(Archive& archive)
			{
				archive(cereal::make_nvp("Active", active));
			}

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
			template <typename Archive>
			void serialize(Archive& archive)
			{
				archive(cereal::make_nvp("Name", name));
			}

			std::string name = "";

			
		};


		struct MeshComponent
		{


			MeshComponent();
			MeshComponent(const SharedPtr<Mesh>& meshRef);
			const SharedPtr<Mesh>& GetMesh();
			

			SharedPtr<Mesh> m_handle = nullptr;
			bool isVisible = true;
			bool castsShadow = true;

		};

		struct SkinnedModelComponent
		{
			SkinnedModelComponent();
			SkinnedModelComponent(const std::string& filePath, bool duplicateLoad = false);
			~SkinnedModelComponent();

			SharedPtr<SkinnedModel> m_handle = nullptr;


		};

		struct AnimatorComponent
		{
			AnimatorComponent();
			~AnimatorComponent();

			SkinnedAnimator* GetAnimator();

			SharedPtr<SkinnedAnimator> m_animator = nullptr;
		};

		struct ModelComponent
		{
			ModelComponent();
			ModelComponent(const std::string& filePath);
			ModelComponent(const SharedPtr<Model>& modelRef);
			ModelComponent(PrimitiveType type);


			void LoadLibraryModel(const std::string filePath);

			void LoadPrimitive(PrimitiveType type);


			SharedPtr<Model> m_handle;
			
			template <typename Archive>
			void save(Archive& archive) const
			{
				if (!m_handle || m_handle->GetMeshes().size() == 0)
					return;
				{
					std::string newPath;

					if (m_handle->GetPrimitiveType() == PrimitiveType::External)
						newPath = m_handle->GetFilePath();
					else
						newPath = "Primitive";

					
					//auto material = std::unique_ptr<Material>(m_handle->GetMeshes().front()->GetMaterial().get());
					//archive(cereal::make_nvp("PrimitiveType", m_handle->GetPrimitiveType()), cereal::make_nvp("FilePath", newPath), cereal::make_nvp("Material", material));
					//material.release();

					archive(cereal::make_nvp("PrimitiveType", m_handle->GetPrimitiveType()), cereal::make_nvp("FilePath", newPath));
				}
			}

			template <typename Archive>
			void load(Archive& archive)
			{
				//auto material = std::unique_ptr<Material>();

				std::string filePath;
				PrimitiveType primitiveType;

				//archive(cereal::make_nvp("PrimitiveType", primitiveType), cereal::make_nvp("FilePath", filePath), cereal::make_nvp("Material", material));
				archive(cereal::make_nvp("PrimitiveType", primitiveType), cereal::make_nvp("FilePath", filePath));

				if (primitiveType != PrimitiveType::External)
				{
					m_handle = MakeShared<Model>(primitiveType);
					m_handle->GetMeshes().back()->SetMaterial(MakeShared<Material>());
					//material.release();
				}
				else
				{
					LoadLibraryModel(filePath);
				}
			}

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

		struct PrefabComponent
		{
			PrefabComponent(const std::string& path)
			{
				Path = path;
			}

			std::string Path;
			
		};

		struct CameraControllerComponent
		{
			enum class ControllerType : uint8_t
			{
				FLYCAM,
				THIRDPERSON,
				FIRSTPERSON,
				CUSTOM
			};


			CameraControllerComponent();
			CameraControllerComponent(ControllerType type);
		

			void SetControllerType(ControllerType type);

			SharedPtr<CameraController>& GetController();
			const ControllerType& GetControllerType();

			static std::string CameraControllerTypeToString(ControllerType type)
			{
				switch (type)
				{
				case ControllerType::FLYCAM: return "Fly";
					break;
				case ControllerType::THIRDPERSON: return "ThirdPerson";
					break;
				case ControllerType::FIRSTPERSON: return "FirstPerson";
					break;
				case ControllerType::CUSTOM: return "Custom";
					break;
				}
				
				return "Custom";
			}

			static ControllerType StringToControllerType(const std::string& type)
			{
				if(type == "Fly")
				{
					return ControllerType::FLYCAM; 
				}
				else if (type == "ThirdPerson")
				{
					return ControllerType::THIRDPERSON;
				}
				else if (type == "FirstPerson")
				{
					return ControllerType::FIRSTPERSON;
				}


				LOG_ERROR("Unsupported Camera controller {0}", type);
				return ControllerType::CUSTOM;
			}

			template <typename Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("ControllerType", m_type));
			}

			template <typename Archive>
			void load(Archive& archive)
			{
				ControllerType controllertype;
				archive(cereal::make_nvp("ControllerType", controllertype));
				SetControllerType(controllertype);
			}

		
		private:

			ControllerType m_type = ControllerType::CUSTOM;
			SharedPtr<CameraController> m_controllerRef;


		};


		class AudioSourceComponent
		{
		public:
			AudioSourceComponent();
			~AudioSourceComponent() = default;

			Audio::AudioSource* GetSource();

		private:

			SharedPtr<Audio::AudioSource> m_handle = nullptr;
		};


		class AudioListenerComponent
		{
		public:
			AudioListenerComponent();
			~AudioListenerComponent() = default;

			Audio::AudioListener* GetListener();

		private:

			SharedPtr<Audio::AudioListener> m_handle = nullptr;
		};


		using Physics::RigidBody;

		class RigidBodyComponent
		{
		public:

			RigidBodyComponent(const Transform& ecstransform, btCollisionShape* shape);
			~RigidBodyComponent() = default;

			RigidBody* GetRigidBody();

		private:

			SharedPtr<RigidBody> m_rigidBody = nullptr;

		};

		using namespace Physics;

		class BoxColliderComponent
		{
		public:
			BoxColliderComponent();
			BoxColliderComponent(const BoundingBox& box);
			~BoxColliderComponent() = default;
		
			BoxCollider* GetCollider();
		private:
			SharedPtr<BoxCollider> m_collider = nullptr;
		};


		class LightComponent
		{
		public :

			LightComponent();
			~LightComponent();

			Light* GetLightData();

		private:

			SharedPtr<Light> m_light = nullptr;
		};
	}

}



#endif // !SCENECOMPONENTS_H
