#include "SceneManager.h"
#include "Engine/Core/Scene/Scene.h"
#include "Engine/Core/System/Application/Application.h"


namespace SaltnPepperEngine
{
	SceneManager::SceneManager()
		: m_sceneIndex(0)
		, m_currentScene(nullptr)
	{
	}

	SceneManager::~SceneManager()
	{
		m_sceneIndex = 0;

		if (m_currentScene)
		{
			LOG_INFO("SceneManager : Exiting Scene : {0}", m_currentScene->GetName());

			m_currentScene->CleanUp();
		}

		m_sceneList.clear();
	}

	void SceneManager::NextScene()
	{
		SwitchScene((m_sceneIndex + 1) % m_sceneList.size());
	}

	void SceneManager::SwitchScene(int sceneIndex)
	{
		m_queuedSceneIndex = sceneIndex;
		m_isSwitchingScenes = true;
	}

	void SceneManager::SwitchScene(const std::string& sceneName)
	{
		bool sceneFound = false;
		m_isSwitchingScenes = true;

		uint32_t index = 0;

		for (uint32_t i = 0; !sceneFound && i < m_sceneList.size(); i++)
		{
			if (m_sceneList[i]->GetName() == sceneName)
			{
				sceneFound = true;
				index = i;
				break;
			}

			if (sceneFound)
			{
				SwitchScene(index);
			}
			else
			{
				LOG_ERROR("SceneManager : No Scene found with teh name :[ {0} ]", sceneName);
			}


		}

	}

	void SceneManager::ApplySceneSwitch()
	{
		if (m_isSwitchingScenes == false)
		{
			if (m_currentScene)
				return;

			if (m_sceneList.empty())
			{
				m_sceneList.push_back(MakeShared<Scene>("NewScene"));
			}

			m_queuedSceneIndex = 0;

		}

		if (m_queuedSceneIndex < 0 || m_queuedSceneIndex >= static_cast<int>(m_sceneList.size()))
		{
			LOG_WARN("SceneManager : Invalid Scene Index : [{0}]", m_queuedSceneIndex);
			m_queuedSceneIndex = 0;
		}

		Application& application = Application::GetCurrent();

		if (m_currentScene)
		{
			LOG_INFO("SceneManager : Exiting Scene : [{0}]", m_currentScene->GetName());
			application.StartPhysics(false);

			m_currentScene->CleanUp();
			
		}

		m_sceneIndex = m_queuedSceneIndex;
		m_currentScene = m_sceneList[m_queuedSceneIndex].get();


		application.StartPhysics(true);

		// Work on this

		std::string physicalPath;

		//if()
	
	}

	void SceneManager::EnqueueScene(const std::string& name)
	{
		// T* scene = new T(name);
		SharedPtr<Scene> newScene = MakeShared<Scene>(name);
		m_sceneList.push_back(newScene);
		LOG_INFO("[SceneManager] - Enqueued scene : {0}", name);
	}

	void SceneManager::EnqueueScene(SharedPtr<Scene>& scene)
	{
		m_sceneList.push_back(scene);
		LOG_INFO("[SceneManager] - Enqueued scene : {0}", scene->GetName());
	}

	std::vector<std::string> SceneManager::GetSceneNames()
	{
		std::vector<std::string> names;

		for (SharedPtr<Scene>& scene : m_sceneList)
		{
			names.emplace_back(scene->GetName());
		}

		return names;
	}

	bool SceneManager::ContainsScene(const std::string& filePath)
	{
		for (uint32_t i = 0; i < m_sceneFilePaths.size(); ++i)
		{
			if (m_sceneFilePaths[i] == filePath)
			{
				return true;
			}
		}

		return false;
	}

	int SceneManager::EnqueSceneFromFile(const std::string& filePath)
	{
		for (uint32_t i = 0; i < m_sceneFilePaths.size(); ++i)
		{
			if (m_sceneFilePaths[i] == filePath)
			{
				return i;
			}
		}

		m_sceneFilePaths.emplace_back(filePath);

		std::string name = FileSystem::GetFileName(filePath);
		SharedPtr<Scene> scene = MakeShared<Scene>(name);
		EnqueueScene(scene);

		return int(m_sceneList.size()) - 1;
	}

	
	const std::vector<std::string>& SceneManager::GetSceneFilePaths()
	{
		return m_sceneFilePaths;
	}

	void SceneManager::LoadCurrentList()
	{
		for (auto& filePath : m_sceneFilePathsToLoad)
		{
			EnqueSceneFromFile(filePath);
		}

		m_sceneFilePathsToLoad.clear();
	}
}
