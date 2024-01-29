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

	std::vector<std::string> SceneManager::GetSceneNames()
	{
		return std::vector<std::string>();
	}
	int SceneManager::EnqueSceneFromFile(const std::string& filePath)
	{
		return 0;
	}

	/*void SceneManager::EnqueScene(Scene* scene)
	{
	}*/

	const std::vector<std::string>& SceneManager::GetSceneFilePaths()
	{
		return m_sceneFilePaths;
	}

	void SceneManager::LoadCurrentList()
	{

	}
}
