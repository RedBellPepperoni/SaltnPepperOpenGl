#include "SceneParser.h"
//#include "filereadstream.h"
#include <fstream>
#include "Engine/Utils/Loading/FileSystem.h"
#include "Engine/Utils/Logging/Log.h"
#include <iostream>

using namespace SaltnPepperEngine;

SceneParser::SceneParser()
{
}

SceneParser::~SceneParser()
{
}

void SceneParser::ParseScene(const std::string& path)
{
	if (!ReadFromFile(path))
	{
		LOG_WARN("NO json File Found");
		return;
	}

	ParseModelData();
	ParseTextureData();
	ParseObjectData();

}

bool SceneParser::ReadFromFile(const std::string& filePath)
{


	std::string data = FileSystem::ReadFileToString(filePath);

	m_document.Parse(data.c_str());

	if (m_document.IsObject())
	{
		return true;
	}

	return false;

}

void SceneParser::ParseModelData()
{

	const Value& modelArray = m_document["Models"];

	for (Value::ConstValueIterator itr = modelArray.Begin(); itr != modelArray.End(); ++itr)
	{
		const rapidjson::Value& attribute = *itr;

		std::string name;
		std::string path;
		std::string identifier;

		assert(attribute.IsObject()); // each attribute is an object
		for (rapidjson::Value::ConstMemberIterator itr2 = attribute.MemberBegin(); itr2 != attribute.MemberEnd(); ++itr2)
		{	

			identifier = itr2->name.GetString();

			if (identifier == "Name")
			{
				name = itr2->value.GetString();
			}
			
			else if (identifier == "Path")
			{
				path = itr2->value.GetString();
			}

			

						
		}

		ModelList[name] = path;

		
	}



	

}

void SceneParser::ParseTextureData()
{

	const Value& textureArray = m_document["Textures"];

	for (Value::ConstValueIterator itr = textureArray.Begin(); itr != textureArray.End(); ++itr)
	{
		const rapidjson::Value& attribute = *itr;

		std::string name;
		std::string path;
		std::string identifier;

		assert(attribute.IsObject()); // each attribute is an object
		for (rapidjson::Value::ConstMemberIterator itr2 = attribute.MemberBegin(); itr2 != attribute.MemberEnd(); ++itr2)
		{

			identifier = itr2->name.GetString();

			if (identifier == "Name")
			{
				name = itr2->value.GetString();
			}

			else if (identifier == "Path")
			{
				path = itr2->value.GetString();
			}




		}

		TextureList[name] = path;


	}


}

void SceneParser::ParseObjectData()
{
	const Value& modelArray = m_document["Objects"];

	for (Value::ConstValueIterator itr = modelArray.Begin(); itr != modelArray.End(); ++itr)
	{
		const rapidjson::Value& attribute = *itr;

		std::string identifier;
		std::string name;
		Vector3 position = Vector3(0.0f);
		Vector3 rotation = Vector3(0.0f);
		Vector3 scale = Vector3(1.0f);
		std::string albedoTexture = "";
		std::string objectType = "";
		Vector3 color = Vector3(0.0f);
		float opacity = 1.0f;
		float metallic = 0.0f;
		float roughness = 0.0f;




		assert(attribute.IsObject()); // each attribute is an object
		for (rapidjson::Value::ConstMemberIterator itr2 = attribute.MemberBegin(); itr2 != attribute.MemberEnd(); ++itr2)
		{

			identifier = itr2->name.GetString();

			if (identifier == "Mesh")
			{
				name = itr2->value.GetString();
			}

			else if (identifier == "Position")
			{
				GenericArray positionArray = itr2->value.GetArray();

				position.x = positionArray[0].GetFloat();
				position.y = positionArray[1].GetFloat();
				position.z = positionArray[2].GetFloat();
			}
			else if (identifier == "Rotation")
			{
				GenericArray rotationArray = itr2->value.GetArray();

				rotation.x = rotationArray[0].GetFloat();
				rotation.y = rotationArray[1].GetFloat();
				rotation.z = rotationArray[2].GetFloat();
			}

			else if (identifier == "Scale")
			{
				GenericArray scaleArray = itr2->value.GetArray();

				scale.x = scaleArray[0].GetFloat();
				scale.y = scaleArray[1].GetFloat();
				scale.z = scaleArray[2].GetFloat();
			}

			else if (identifier == "Albedo")
			{
				albedoTexture = itr2->value.GetString();
			}

			else if (identifier == "ObjectType")
			{
				objectType = itr2->value.GetString();
			}


		}




		ObjectData& data = ObjectDataList.emplace_back();

		data.meshName = name;
		data.position = position;
		data.rotation = rotation;
		data.scale = scale;
		data.albedoTexture = albedoTexture;
		data.objectType = objectType;
		

	}

}

const std::string SceneParser::GetStringValue(const std::string& key) const
{
	return std::string();
}

const int SceneParser::GetIntValue(const std::string& key) const
{
	return 0;
}

const bool SceneParser::GetBoolValue(const std::string& key) const
{
	return false;
}
