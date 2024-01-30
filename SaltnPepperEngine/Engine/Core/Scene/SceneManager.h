#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <string>
#include <vector>
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Utils/Logging/Log.h"

namespace SaltnPepperEngine
{
	
	class Scene;

	class SceneManager
	{
	public:

		SceneManager();
		~SceneManager();

		// Goes to the next scene in the scene list (if at the end scene, goes back to the first scene)
		// calling this when only once scene is in the list will reload the scene
		void NextScene();

		void SwitchScene(int sceneIndex);
		void SwitchScene(const std::string& sceneName);

		void ApplySceneSwitch();

		inline Scene* GetCurrentScene() const
		{
			return m_currentScene;
		}

		inline uint32_t GetCurrentSceneIndex() const
		{
			return m_sceneIndex;
		}

		inline uint32_t SceneCount() const
		{
			return static_cast<uint32_t>(m_sceneList.size());
		}


		const std::vector<SharedPtr<Scene>>& GetScenes() const
		{
			return m_sceneList;
		}

		void SetSwitchScene(bool isSwitching)
		{
			m_isSwitchingScenes = isSwitching;
		}

		const bool GetSwitchingScene() const
		{
			return m_isSwitchingScenes;
		}

	
		void EnqueueScene(const std::string& name);
		void EnqueueScene(SharedPtr<Scene>& scene);


		void AddFileToLoadList(const std::string& filePath)
		{
			m_sceneFilePathsToLoad.push_back(filePath);
		}

		std::vector<std::string> GetSceneNames();

		bool ContainsScene(const std::string& filePath);

		int EnqueSceneFromFile(const std::string& filePath);
		//void EnqueScene(Scene* scene);

		const std::vector<std::string>& GetSceneFilePaths();
		void LoadCurrentList();
		

	protected:

		uint32_t m_sceneIndex;
		Scene* m_currentScene;

		std::vector<SharedPtr<Scene>> m_sceneList;
		std::vector<std::string> m_sceneFilePaths;
		std::vector<std::string> m_sceneFilePathsToLoad;

	private:

		bool m_isSwitchingScenes = false;
		int m_queuedSceneIndex = -1;


	};

}

#endif // !SCENEMANAGER_H


