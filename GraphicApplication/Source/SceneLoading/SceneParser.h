#pragma once
//#include <rapidjson.h>
//#include "document.h"'
#include "Engine/Utils/Serialization/Serialization.h"
#include <string>
#include <map>
#include "SaltnPepperEngine.h"

using namespace SaltnPepperEngine;

using namespace rapidjson;

struct ObjectData
{
	std::string meshName;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	std::string albedoTexture;
	std::string objectType;

};


class SceneParser
{
public:

	SceneParser();
	~SceneParser();


	void ParseScene(const std::string& path);
	bool ReadFromFile(const std::string& filePath);


	std::map<std::string, std::string> GetModelList() { return ModelList; }
	std::map<std::string, std::string> GetTextureList() { return TextureList; }
	std::vector<ObjectData> GetObjectList() { return ObjectDataList; }

private:

	void ParseModelData();
	void ParseTextureData();
	void ParseObjectData();

	const std::string GetStringValue(const std::string& key) const;
	const int GetIntValue(const std::string& key) const;
	const bool GetBoolValue(const std::string& key) const;

	std::map<std::string, std::string> ModelList;
	std::map < std::string, std::string> TextureList;

	std::vector<ObjectData> ObjectDataList;

	Document m_document;

};

