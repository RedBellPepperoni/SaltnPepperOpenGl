#ifndef SCENELOADER_H
#define SCENELOADER_H

#include "EntitySetup.h"

struct ObjectData;

class SceneLoader
{
public:
	SceneLoader();
	~SceneLoader() = default;


	void LoadModel(const std::string& name, const std::string& modelpath);
	void LoadTexture(const std::string& name, const std::string& path);
	void SpawnObject(const ObjectData& data);


private:

	SharedPtr<ModelLibrary> modelLibrary = nullptr;
	SharedPtr<TextureLibrary> textureLibrary = nullptr;

	Scene* sceneRef = nullptr;

	int entityCount = 0;

};
#endif // !SCENELOADER_H

