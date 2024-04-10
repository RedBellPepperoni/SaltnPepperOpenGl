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
        
        const int viewSize = static_cast<int>(enemyView.Size());

        for(int i = 0; i < viewSize; i++)
        {
            EnemyCharacter* enemy = enemyView[i].GetComponent<EnemyComponent>().GetEnemy();
            Transform& enemyTransform = enemyView[i].GetComponent<Transform>();
            Transform& enemyLook = enemyLookView[i].GetComponent<Transform>();

            enemy->OnUpdate(deltaTime,  playerPos, enemyTransform, enemyLook);
        }

    }

    void GameManager::OnInitTestScene()
    {

        AudioClip* idleclip = Application::GetCurrent().GetAudioLibrary()->GetResource("zombieidle1").get();
        AudioClip* attackclip = Application::GetCurrent().GetAudioLibrary()->GetResource("zombieattack1").get();
        AudioClip* death1clip = Application::GetCurrent().GetAudioLibrary()->GetResource("zombiedeath1").get();
        AudioClip* death2clip = Application::GetCurrent().GetAudioLibrary()->GetResource("zombiedeath2").get();
        AudioClip* alertclip = Application::GetCurrent().GetAudioLibrary()->GetResource("zombiealert1").get();




        std::vector<Audio::AudioClip*> clipsVar1;
        clipsVar1.push_back(idleclip);
        clipsVar1.push_back(attackclip);
        clipsVar1.push_back(alertclip);
        clipsVar1.push_back(death1clip);






        EntitySetup::CreateDirectionalLight(Vector3(-11.4192f, 51.3504f, -7.0023f));
        body = EntitySetup::CreatePlayer(Vector3(5.0f, 1.0f, 5.0f), Vector3(0.0f, 20.0f, 0.0f));

        EntitySetup::CreateZombie(Vector3(10.0f, 1.0f, 10.0f), Vector3(0.0f, 0.0f, 0.0f), clipsVar1, ZombieType::WALK);
        EntitySetup::CreateZombie(Vector3(15.0f, 1.0f, 15.0f), Vector3(0.0f, 0.0f, 0.0f), clipsVar1, ZombieType::WALK);
        EntitySetup::CreateZombie(Vector3(22.0f, 1.0f, 10.0f), Vector3(0.0f, 0.0f, 0.0f), clipsVar1, ZombieType::WALK);
        EntitySetup::CreateZombie(Vector3(0.0f, 1.0f, 10.0f), Vector3(0.0f, 0.0f, 0.0f), clipsVar1, ZombieType::WALK);
        EntitySetup::CreateZombie(Vector3(10.0f, 1.0f, 15.0f), Vector3(0.0f, 0.0f, 0.0f), clipsVar1, ZombieType::WALK);


        EntitySetup::CreatePhysicsFloor(Vector3(0.0f, -0.5f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));

       

    }

    void GameManager::OnInitMainScene()
    {

    
        // TEST SETUP
       // EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TEST, Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));



        // ========================== SUBWAY PLATFORM =======================================
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM, Vector3(-0.16f, -3.08f, -4.46f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_BACKWALL, Vector3(0.61f, -0.73f, -4.54f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));

        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_RIGHTWALLL, Vector3(-2.54f, 0.99f, -26.91f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_LEFTWALLL, Vector3(-2.54f, 0.466f, 17.94f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));

        // ======================== PILLARS ==========================================
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_PILLAR, Vector3(-4.91f, -0.67f, -23.07f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_PILLAR, Vector3(-4.91f, -0.67f, -11.203f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_PILLAR, Vector3(-4.91f, -0.67f, -5.6f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_PILLAR, Vector3(-4.91f, -0.67f, 0.33f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_PILLAR, Vector3(-4.91f, -0.67f, 6.27f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));


        // ============================ TUNNEL WALLS ===================================
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_WALL, Vector3(-5.11f, -2.45f, -33.87f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_WALL, Vector3(-5.11f, -2.45f, 25.35f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_WALL, Vector3(-5.11f, -2.45f, 39.48f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
       

        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_WALL, Vector3(-11.304f, -2.455f, -32.736f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_WALL, Vector3(-11.304f, -2.45f, -18.609f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_WALL, Vector3(-11.304f, -2.445f, -4.4672f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_WALL, Vector3(-11.304f, -2.45f, 9.6595f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_WALL, Vector3(-11.304f, -2.445f, 23.7715f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_WALL, Vector3(-11.304f, -2.45f, 37.898f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));

        // ============================= TUNNEL PILLARS =====================================
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_PILLAR, Vector3(-5.2186f, -1.638f, -36.126f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_PILLAR, Vector3(-5.2186f, -1.638f, 23.103f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_PILLAR, Vector3(-5.2186f, -1.638f, 37.23f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_PILLAR, Vector3(-11.196f, -1.638f, -30.48f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_PILLAR, Vector3(-11.196f, -1.638f, -16.353f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_PILLAR, Vector3(-11.196f, -1.638f, -2.2113f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_PILLAR, Vector3(-11.196f, -1.638f, 11.915f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_PILLAR, Vector3(-11.196f, -1.638f, 26.027f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_PILLAR, Vector3(-11.196f, -1.638f, 40.154f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));




        EntitySetup::CreateDirectionalLight(Vector3(-11.4192f, 51.3504f, -7.0023f));
        body = EntitySetup::CreatePlayer(Vector3(-4.1f, 1.0f, 4.6f), Vector3(0.0f, 0.0f, 0.0f));
        EntitySetup::CreatePhysicsFloor(Vector3(0.0f, -3.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
    }

    void GameManager::OnUpdateTestScene(const float deltaTime)
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

    void GameManager::OnUpdateMainScene(const float deltaTime)
    {

        if (Application::GetCurrent().GetEditorActive())
        {
            Application::GetCurrent().GetAppWindow().SetMouseHidden(false);
            return;
        }


        UpdatePlayer(deltaTime);

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

       // OnInitTestScene();

        OnInitMainScene();


	}


	void GameManager::OnUpdate(const float deltaTime)
	{
        //OnUpdateTestScene(deltaTime);

        OnUpdateMainScene(deltaTime);
       

	}
}