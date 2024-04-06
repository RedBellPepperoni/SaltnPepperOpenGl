#include "GameManager.h"


namespace SaltnPepperEngine
{
    void GameManager::UpdatePlayer(const float deltaTime)
    {
        Application::GetCurrent().GetAppWindow().SetMouseHidden(true);


        Vector2 mousePosition = InputSystem::GetInstance().GetMousePosition();

        ComponentView PlayerComponentView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<PlayerComponent>();
        ComponentView PlayerLookView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<PlayerLook>();


        if (!PlayerComponentView.IsEmpty() && !PlayerLookView.IsEmpty())
        {
            PlayerCharacter* player = PlayerComponentView[0].GetComponent<PlayerComponent>().GetPlayer();
            playerPosition = PlayerComponentView[0].GetComponent<Transform>().GetPosition();
            Transform& playerLook = PlayerLookView[0].GetComponent<Transform>();

            player->OnUpdate(deltaTime, mousePosition, playerLook);
        }


    }

    void GameManager::UpdateEnemies(const float deltaTime, const Vector3& playerPos)
    {
        ComponentView enemyView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<EnemyComponent>();
        ComponentView enemyLookView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<EnemyLook>();


        if (enemyView.IsEmpty() || enemyLookView.IsEmpty()) { return; }
        
        const float viewSize = enemyView.Size();

        for(int i = 0; i < viewSize; i++)
        {
            EnemyCharacter* enemy = enemyView[i].GetComponent<EnemyComponent>().GetEnemy();
            Transform& enemyTransform = enemyView[i].GetComponent<Transform>();
            Transform& enemyLook = enemyLookView[i].GetComponent<Transform>();

            enemy->OnUpdate(deltaTime,  playerPos, enemyTransform, enemyLook);
        }

    }


    void GameManager::OnInit()
	{
        // Start the Physics
        PhysicsSystem::SetPaused(false);

        // LOad all the Models 
        EntitySetup::LoadAllModels();
        // Load all the textures
        EntitySetup::LoadAllTextures();
        //Load all the audio files

        EntitySetup::LoadAllAudio();

  
  
        glPolygonMode(GL_FRONT, GL_FILL);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        EntitySetup::CreateDirectionalLight(Vector3(-11.4192f, 51.3504f, -7.0023f));
        body = EntitySetup::CreatePlayer(Vector3(5.0f, 1.0f, 5.0f), Vector3(0.0f, 20.0f, 0.0f));

        EntitySetup::CreateZombie(Vector3(10.0f, 1.0f, 10.0f), Vector3(0.0f, 0.0f, 0.0f), ZombieType::WALK);
        EntitySetup::CreateZombie(Vector3(15.0f, 1.0f, 15.0f), Vector3(0.0f, 0.0f, 0.0f), ZombieType::WALK);
        EntitySetup::CreateZombie(Vector3(22.0f, 1.0f, 10.0f), Vector3(0.0f, 0.0f, 0.0f), ZombieType::WALK);
        EntitySetup::CreateZombie(Vector3(0.0f, 1.0f, 10.0f), Vector3(0.0f, 0.0f, 0.0f), ZombieType::WALK);
        EntitySetup::CreateZombie(Vector3(10.0f, 1.0f, 15.0f), Vector3(0.0f, 0.0f, 0.0f), ZombieType::WALK);


        EntitySetup::CreatePhysicsFloor(Vector3(0.0f, -0.5f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));

        EntitySetup::CreateSkinnedCharatcer(Vector3(1.27f, 0.0f, 0.0f), Vector3(0.01f));

       
	}


	void GameManager::OnUpdate(const float deltaTime)
	{
        // Toggle Physics Simulation
        if (Input::InputSystem::GetInstance().GetKeyDown(Input::Key::G))
        {
            PhysicsSystem::SetPaused(!PhysicsSystem::GetPaused());
        }
        UpdateEnemies(deltaTime, playerPosition);

        if (Application::GetCurrent().GetEditorActive())
        {
            Application::GetCurrent().GetAppWindow().SetMouseHidden(false);
            return;
        }


        UpdatePlayer(deltaTime);
       

	}
}