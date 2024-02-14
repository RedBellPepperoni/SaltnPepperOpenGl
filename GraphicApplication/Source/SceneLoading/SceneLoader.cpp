#pragma once
#include "SceneLoader.h"
#include "SceneParser.h"



using namespace SaltnPepperEngine;
using namespace SaltnPepperEngine::Physics;

SceneLoader::SceneLoader()
{
	modelLibrary = Application::GetCurrent().GetModelLibrary();
	textureLibrary = Application::GetCurrent().GetTextureLibrary();

	sceneRef = Application::GetCurrent().GetCurrentScene();

	//EntitySpawner::platformId = 0;

}

void SceneLoader::LoadModel(const std::string& name, const std::string& modelpath)
{
	modelLibrary->LoadModel(name, modelpath);
}

void SceneLoader::LoadTexture(const std::string& name, const std::string& path)
{
	textureLibrary->LoadTexture(name, path, TextureFormat::RGBA);
}



void SceneLoader::SpawnObject(const ObjectData& data)
{
	if (!sceneRef)
	{
		LOG_WARN("Scene Reference Empty");
		return;
	}

	if (!modelLibrary)
	{
		LOG_WARN("ModelLibrary Reference Empty");
		return;
	}


	if (data.objectType == "Player")
	{
		// generate the Camera
		Entity mainCamera = EntitySpawner::CreateMainCamera();
		//Genetrate Custom Hand type with animation data
		Entity leftHand = EntitySpawner::CreateHand(true);
		Entity rightHand = EntitySpawner::CreateHand(false);

		// Finally Create the player
		EntitySpawner::CreatePlayerCharacter(mainCamera, leftHand, rightHand,data.position);
	}

	else if (data.objectType == "Platform")
	{
		EntitySpawner::CreatePlatform(data.position, data.scale, data.albedoTexture);
	}

	else if (data.objectType == "DirectionalLight")
	{
		EntitySpawner::CreateDirectionalLight(data.rotation);
	}


	//std::string name = "Entity_" + std::to_string(entityCount);

	//

	//Entity entity = sceneRef->CreateEntity(name);

	//Transform* transform = &entity.GetComponent<Transform>();

	//transform->SetPosition(data.position);
	//transform->SetEularRotation(data.rotation);
	//transform->SetScale(data.scale);
	////transform->SetScale(Vector3(1.0f,1.0f,1.0f));


	//
	//

	//SharedPtr<Mesh> mesh = modelLibrary->GetResource(data.meshName)->GetMeshes()[0];
	////SharedPtr<Mesh> mesh = modelLibrary->GetResource("Ground")->GetMeshes()[0];

	//entity.AddComponent<MeshComponent>(mesh);
	//SharedPtr<Material> material = entity.AddComponent<MeshRenderer>().GetMaterial();

	//material->textureMaps.albedoMap = textureLibrary->GetResource(data.albedoTexture);




	
}

