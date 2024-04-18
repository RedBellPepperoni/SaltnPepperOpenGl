#include "GameManager.h"

#include "Engine/Core/Rendering/Renderer/DebugRenderer.h"

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
            m_playerPosition = PlayerComponentView[0].GetComponent<Transform>().GetPosition();
            Transform& playerLook = PlayerLookView[0].GetComponent<Transform>();

            player->OnUpdate(deltaTime, mousePosition, playerLook);
        }


    }

    void GameManager::UpdateEnemies(const float deltaTime, const Vector3& playerPos, const Vector3& buddyPos)
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

            enemy->OnUpdate(deltaTime,  playerPos, buddyPos, enemyTransform, enemyLook);
        }

    }

    void GameManager::UpdateBuddy(const float deltaTime)
    {
        ComponentView buddyCompView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<BuddyComponent>();
        ComponentView buddyLookView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<BuddyLook>();


        if (!buddyCompView.IsEmpty() && !buddyLookView.IsEmpty())
        {
            BuddyCharacter* buddy = buddyCompView[0].GetComponent<BuddyComponent>().GetBuddy();
            Transform& buddyTransform = buddyCompView[0].GetComponent<Transform>();

            m_buddyPosition = buddyTransform.GetPosition();
            Transform& buddyLook = buddyLookView[0].GetComponent<Transform>();

            buddy->OnUpdate(deltaTime, buddyTransform, buddyLook);
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
        EntitySetup::CreatePlayer(Vector3(5.0f, 1.0f, 5.0f), Vector3(0.0f, 20.0f, 0.0f));

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

        
       // PhysicsSystem::SetPaused(true);

        SetupStaticMeshes();
        SetupStaticPhysics();

        EntitySetup::CreateDirectionalLight(Vector3(-11.4192f, 51.3504f, -7.0023f));
      
       
       // EntitySetup::CreatePhysicsFloor(Vector3(-2.0f, -3.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));


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

        EntitySetup::CreateZombie(Vector3(-3.472f, -0.5f, -20.852f), Vector3(0.0f, 0.0f, 0.0f), clipsVar1, ZombieType::WALK)->SetGameManagerRef(this);
        EntitySetup::CreateZombie(Vector3(-1.806f, -0.5f, -7.8213f), Vector3(0.0f, 0.0f, 0.0f), clipsVar1, ZombieType::WALK)->SetGameManagerRef(this);
        EntitySetup::CreateZombie(Vector3(-4.602f, -0.5f, 15.4552f), Vector3(0.0f, 0.0f, 0.0f), clipsVar1, ZombieType::WALK)->SetGameManagerRef(this);
        EntitySetup::CreateZombie(Vector3(-7.972f, -0.5f, 2.6780f), Vector3(0.0f, 0.0f, 0.0f), clipsVar1, ZombieType::WALK)->SetGameManagerRef(this);
       
        EntitySetup::CreateZombie(Vector3(-9.3406f, -0.4f, -4.9650f), Vector3(0.0f, 0.0f, 0.0f), clipsVar1, ZombieType::WALK)->SetGameManagerRef(this);
        EntitySetup::CreateZombie(Vector3(-9.116f, -0.4f, -25.245f), Vector3(0.0f, 0.0f, 0.0f), clipsVar1, ZombieType::WALK)->SetGameManagerRef(this);
        EntitySetup::CreateZombie(Vector3(0.6353f, 3.4f, 7.0485f), Vector3(0.0f, 0.0f, 0.0f), clipsVar1, ZombieType::WALK)->SetGameManagerRef(this);
        EntitySetup::CreateZombie(Vector3(8.381f, 3.4f, 10.1f), Vector3(0.0f, 0.0f, 0.0f), clipsVar1, ZombieType::WALK)->SetGameManagerRef(this);
     


        AudioClip* gunshootClip = Application::GetCurrent().GetAudioLibrary()->GetResource("gunshoot").get();
        AudioClip* gunreloadClip = Application::GetCurrent().GetAudioLibrary()->GetResource("gunreload").get();
        AudioClip* gunemptyClip = Application::GetCurrent().GetAudioLibrary()->GetResource("gunempty").get();


        m_playerRef = EntitySetup::CreatePlayer(Vector3(-4.1f, 0.2f, 4.6f), Vector3(0.0f, 0.0f, 0.0f));
        m_playerRef->SetGameManagerRef(this);

        m_playerRef->SetAudioClipsGun(gunshootClip,gunemptyClip,gunreloadClip);

       
        AudioClip* robookay = Application::GetCurrent().GetAudioLibrary()->GetResource("robo_okay").get();
        AudioClip* robomoving = Application::GetCurrent().GetAudioLibrary()->GetResource("robo_moving").get();
        AudioClip* robogoing = Application::GetCurrent().GetAudioLibrary()->GetResource("robo_going").get();
       
        AudioClip* robodie = Application::GetCurrent().GetAudioLibrary()->GetResource("robo_diecreature").get();
        AudioClip* robokill = Application::GetCurrent().GetAudioLibrary()->GetResource("robo_kill").get();

        AudioClip* robostop = Application::GetCurrent().GetAudioLibrary()->GetResource("robo_stop").get();
        AudioClip* robodontshoot = Application::GetCurrent().GetAudioLibrary()->GetResource("robo_dontshoot").get();

       

        m_buddyRef = EntitySetup::CreateBuddy(Vector3(-4.1f, 0.2f, 12.6f), Vector3(0.0f, 0.0f, 0.0f));
        m_buddyRef->SetGameManagerRef(this);


        std::vector<AudioClip*> moveSpeech{ robookay,robomoving,robogoing };
        std::vector<AudioClip*> killSpeech{ robodie,robokill };
        std::vector<AudioClip*> stopSpeech{ robostop,robodontshoot };
        m_buddyRef->SetSpeechClips(moveSpeech,killSpeech,stopSpeech);

        m_waypointbaseEntity = EntitySetup::CreateWaypointBase(Vector3(0.0f, 0.0f, 0.0f));
        m_waypointarrowEntity = EntitySetup::CreateWaypointArrow(Vector3(0.0f, 0.5f, 0.0f), m_waypointbaseEntity);


    }

    void GameManager::SetupNavMesh()
    {
      

        SharedPtr<Model> navModel = Application::GetCurrent().GetModelLibrary()->GetResource("NavMesh_Main");
        SharedPtr<Mesh> navMesh = navModel->GetMeshes()[0];

        std::vector<Vector3> vertices;
        std::vector<NavTriangle> tris;

        for (const Vertex& vertex : navMesh->GetVertexData())
        {
            vertices.push_back(vertex.position);
        }

        int j = 0;
        for (uint32_t i = 0; i < navMesh->GetIndexCount()/3; i++)
        {
            

            int firstindex = static_cast<int>(navMesh->GetIndexData()[i * 3]);
            int secondindex = static_cast<int>(navMesh->GetIndexData()[i * 3+1]);
            int thirdindex = static_cast<int>(navMesh->GetIndexData()[i * 3+2]);

            NavTriangle triangle{ vertices[firstindex],vertices[secondindex],vertices[thirdindex] };
 
          
            tris.push_back(triangle);
        }


        m_navMesh = MakeShared<NavMesh>(tris);

        m_debugnavmesh = tris;

    }

    void GameManager::SetupPathFinder()
    {
        m_pathFinder = MakeShared<Pathfinder>(m_navMesh.get());
    }

    void GameManager::SetupStaticMeshes()
    {

        // ========================== SUBWAY CEILING =====================
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFORM_CEILING, Vector3(-0.1555f, 1.7249f, -4.4672f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));

        // ========================== SUBWAY PLATFORM =======================================
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM, Vector3(-0.16f, -3.08f, -4.46f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_BACKWALL, Vector3(0.61f, -0.73f, -4.54f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));

        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_RIGHTWALLL, Vector3(-2.54f, 0.99f, -26.91f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_LEFTWALLL, Vector3(-2.54f, 0.466f, 17.94f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));

        // ======================== PLATFORM PILLARS ==========================================
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_PILLAR, Vector3(-4.91f, -0.67f, -23.07f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_PILLAR, Vector3(-4.91f, -0.67f, -11.203f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_PILLAR, Vector3(-4.91f, -0.67f, -5.6f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_PILLAR, Vector3(-4.91f, -0.67f, 0.33f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_PILLAR, Vector3(-4.91f, -0.67f, 6.27f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));

        // ========================== PLATFORM STAIRS ========================================
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_STAIR, Vector3(-2.7689f, 2.0338f, -0.5109f), Vector3(0.0f, 00.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_STAIR_RAIL, Vector3(-1.7303f, 2.497f, -0.5720f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_STAIR_RAIL, Vector3(-3.8027f, 2.497f, -0.5720f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_STAIR_WALL, Vector3(-2.7749f, -0.92337f, -2.6512f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));

        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_STAIR_COVER, Vector3(-2.7054f, 5.326f, -1.1832f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));


        // ============================= PLATFORM PIPE ========================================
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFORM_PIPE, Vector3(0.2766f, 1.2804f, 17.3489f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));

        // ============================= PLATFORM BENCHES =======================================
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_BENCH, Vector3(-2.1826f, -2.2860f, -24.1374f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_BENCH, Vector3(-2.1826f, -2.2860f, -21.0217f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_BENCH, Vector3(-2.1826f, -2.2860f, -15.3211f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_BENCH, Vector3(-2.1826f, -2.2860f, -12.2053f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_BENCH, Vector3(-2.1826f, -2.2860f, 10.2039f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_BENCH, Vector3(-2.1826f, -2.2860f, 14.327f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));



        // ============================= PLATFORM BINS ==========================================
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_BIN, Vector3(-2.1781f, -2.1309f, -18.1267f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_BIN, Vector3(-2.1781f, -2.1309f, 12.214), Vector3(0.0f, 20.0f, 0.0f), Vector3(1.0f));


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

        // ============================= TUNNEL BASE =========================================== 
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_BASE, Vector3(-11.304f, -3.890f, -32.736f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_BASE, Vector3(-11.303f, -3.8905f, -18.609f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_BASE, Vector3(-11.304f, -3.890f, -4.4672f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_BASE, Vector3(-11.303f, -3.8905f, 9.6595f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_BASE, Vector3(-11.304f, -3.890f, 23.7715f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_BASE, Vector3(-11.303f, -3.8905f, 37.898f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));

        // ============================= TUNNEL RAILS =========================================== 
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_RAILS, Vector3(-9.106f, -3.45f, -32.736f), Vector3(0.0f, 00.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_RAILS, Vector3(-9.107f, -3.455f, -18.609f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_RAILS, Vector3(-9.106f, -3.45f, -4.4672f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_RAILS, Vector3(-9.107f, -3.455f, 9.6595f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_RAILS, Vector3(-9.106f, -3.45f, 23.7715f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_RAILS, Vector3(-9.107f, -3.455f, 37.898f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_RAILSSIDE, Vector3(-9.6915f, -3.1774f, 37.9735f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));

        // ============================= TUNNEL WIRES =========================================== 
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_WIRE, Vector3(-6.1329f, -0.0125f, -33.8702f), Vector3(0.0f, 00.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_WIRE, Vector3(-6.1329f, -0.0125f, 25.3593f), Vector3(0.0f, 00.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_WIRE, Vector3(-6.1329f, -0.0125f, 39.486f), Vector3(0.0f, 00.0f, 0.0f), Vector3(1.0f));

        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_WIRE, Vector3(-10.282f, -0.0125f, -32.7359f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_WIRE, Vector3(-10.282f, -0.0125f, -18.6091f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_WIRE, Vector3(-10.282f, -0.0125f, -4.46724f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_WIRE, Vector3(-10.282f, -0.0125f, 9.6595f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_WIRE, Vector3(-10.282f, -0.0125f, 23.7711f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_WIRE, Vector3(-10.282f, -0.0125f, 37.8979f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));

        // ============================== TUNNEL ARCH ==========================================
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_ARCH, Vector3(-7.9223f, -0.5832f, -33.8702f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_ARCH, Vector3(-7.9223f, -0.5832f, 25.3593f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_ARCH, Vector3(-7.9223f, -0.5832f, 39.486f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));


        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_ARCH, Vector3(-8.4925f, -0.5832f, 37.8979f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_ARCH, Vector3(-8.4925f, -0.5832f, 23.7711f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_ARCH, Vector3(-8.4925f, -0.5832f, 9.6595f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_ARCH, Vector3(-8.4925f, -0.5832f, -4.4672f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_ARCH, Vector3(-8.4925f, -0.5832f, -18.6091f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_ARCH, Vector3(-8.4925f, -0.5832f, -32.7359f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));

        // ================================ TUNNEL PIPE , SIDE AND LIGHTS ======================================
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_LIGHTS, Vector3(-10.0414f, 1.3237f, -30.0984f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_PIPE, Vector3(-6.1681, 1.2387f, 25.3593f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUNNEL_RAILSSIDE, Vector3(-9.6915f, -3.1774f, 37.9735f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));

        // ================================   SUBWAY POSTERS =====================================
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::POSTER_ADVERTLEFT, Vector3(0.3670f, -0.7983f, -10.0963f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::POSTER_ADVERTRIGHT, Vector3(0.3670f, -0.7983f, -6.8705f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));

        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::POSTER_LOST, Vector3(0.3670f, -0.798f, -23.3927f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::POSTER_FOOL, Vector3(0.3670f, -0.7983f, -20.167f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));

        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::POSTER_WORK, Vector3(0.3670f, -0.7983f, 1.446f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::POSTER_DINO, Vector3(0.3670f, -0.7983f, 4.6722), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));





        // ========================== TICKET ROOM =================================================
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TICKET_CEILING, Vector3(2.4412f, 5.8751f, 3.6829f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TICKET_WALL_FRONT, Vector3(4.5943f, 4.9877f, 13.366f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TICKET_WALL_BACK, Vector3(4.5943f, 4.8859f, -3.8520f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TICKET_WALL_LEFT, Vector3(15.0921f, 4.8859f, 10.5603f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TICKET_WALL_RIGHT, Vector3(-4.6251f, 4.8859f, 1.1267f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));


        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TICKET_DOOR, Vector3(8.8607f, 3.0987f, -3.4983f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TICKET_DOOR, Vector3(10.183f, 3.0987f, -3.4983f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));

        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TICKET_GRILL, Vector3(9.3551f, 3.5645f, 5.0689f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TICKET_GRILL, Vector3(-1.8865f, 3.5645f, 5.0689f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));

        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TICKET_MACHINE, Vector3(2.8286f, 3.2776, 5.5924f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));


        // ============================ TICKET ROOM POSTERS ========================================
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::POSTER_HOTELLEFT, Vector3(1.2933f, 3.8102f, -3.5021f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::POSTER_HOTELRIGHT, Vector3(4.5190f, 3.8102f, -3.5021f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));

        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::POSTER_INFO, Vector3(11.7927f, 3.8102f, -0.8949f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::POSTER_HOLIDAY, Vector3(11.7927f, 3.8102f, 2.3307f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));


        // ======================== TICKET PILLAR ANd BIN =================================
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_PILLAR, Vector3(8.1288f, 3.940f, 0.05383f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_PILLAR, Vector3(1.9173f, 3.940f, 0.05383f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_PILLAR, Vector3(8.1288f, 3.940f, 8.8039f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_PILLAR, Vector3(1.9173f, 3.940f, 8.8039f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));

        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_BIN, Vector3(5.0379f, 2.5403f, -0.08261f), Vector3(0.0f, 20.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_BIN, Vector3(0.4520f, 2.5403f, 8.8623f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));

        // ======================== BAORDS AND TAGs =========================
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::WALL_TAG, Vector3(0.36703f, -0.007f, 11.9231f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::WALL_TAG, Vector3(0.36703f, -0.007f, -15.111f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));

        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::STAIR_TAG, Vector3(-2.7065f, 0.977f, -5.5541f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));

        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::EXIT_TAG, Vector3(-2.6701f, 0.977f, -18.291f), Vector3(0.0f, 180.0f, 0.0f), Vector3(1.0f));
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::EXIT_TAG, Vector3(-2.6701f, 0.977f, 9.001f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));

        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::TUBE_LIGHT, Vector3(-5.2800f, 1.3542f, -4.4672f), Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f));


        // =============== LIGHTS ==============================
        


        EntitySetup::CreateTunnelLights(Vector3(-4.926f,0.0f, 27.040f), Vector3{1.0f,0.8f,0.7f},8.0f,12.0f);
        EntitySetup::CreateTunnelLights(Vector3(-11.416f,0.0f, 7.137f), Vector3{1.0f,0.8f,0.7f},8.0f,12.0f);
        EntitySetup::CreateTunnelLights(Vector3(-11.416f,0.0f, -7.193f), Vector3{1.0f,0.8f,0.7f},8.0f,12.0f);
        EntitySetup::CreateTunnelLights(Vector3(-11.416f,0.0f, -21.335f), Vector3{1.0f,0.8f,0.7f},8.0f,12.0f);
        EntitySetup::CreateTunnelLights(Vector3(-11.416f,0.0f, -35.81f), Vector3{1.0f,0.8f,0.7f},8.0f,12.0f);

        
        // ============ CRATE AND BARRELS ==============

        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::CRATE, Vector3{ -7.7025f, -2.931f,-27.575f }, Vector3{ 0.0f,-27.0f,0.0f }, Vector3{ 1.0f });
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::CRATE, Vector3{ -6.160f, -2.929f,-28.110f }, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 1.0f });
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::CRATE, Vector3{ -7.340f, -2.929f,-28.7205f }, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 1.0f });
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::CRATE, Vector3{ -9.1098f, -2.929f,-27.422f }, Vector3{ 0.0f,-2.857f,0.0f }, Vector3{0.83f, 1.0f,1.0f });

        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::CRATE, Vector3{ -10.325f, -2.998f,-27.575f }, Vector3{ 0.0f,-27.37f,0.0f }, Vector3{ 0.74f,1.0f ,1.0f});
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::CRATE, Vector3{ -9.109f, -2.998f,-28.720f }, Vector3{ 0.0f,-2.85f,0.0f }, Vector3{ 0.827f,1.0f ,1.0f});
       
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::CRATE_GROUP, Vector3{ -7.289f, -1.9812f,-28.137f }, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 1.0f,1.0f ,1.0f});
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::CRATE_CLOTH, Vector3{ -7.1655f, -1.8518f,-28.507f }, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 1.0f,1.0f ,1.0f });

        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::CRATE_GROUP, Vector3{ -9.45f, -1.9812f,-28.035f }, Vector3{ 0.0f,-150.0f,0.0f }, Vector3{ 1.0f,1.0f ,1.0f});
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::CRATE_CLOTH, Vector3{ -9.373f, -1.8518f,-27.65f }, Vector3{ 0.0f,-150.0f,0.0f }, Vector3{ 1.0f,1.0f ,1.0f});
        
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::WOODPALLET, Vector3{ -6.4791f, -3.0165f,15.626f }, Vector3{ 0.0f,0.0f, 27.0f }, Vector3{ 1.34f});
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::WOODPALLET, Vector3{ -6.4637f, -3.0368f,16.70f }, Vector3{ -28.0f,90.0f, 0.0f }, Vector3{ 1.34f});
        
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::WOODPALLET, Vector3{ -6.1883f, -3.021f,-15.834f }, Vector3{ 0.0f,0.0f, 33.0f }, Vector3{ 1.0f});
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::WOODPALLET, Vector3{ -6.1883f, -3.021f,-12.80f }, Vector3{ 0.0f,0.0f, 33.0f }, Vector3{ 1.0f});
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::WOODPALLET, Vector3{ -6.121f, -2.984f,-14.314f }, Vector3{ -97.438f,90.0f, 116.489f }, Vector3{ 1.5f,1.0f,1.0f});
        
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::BARRELGROUPONE, Vector3{ -6.0111f, -3.455f,-0.49894f }, Vector3{ 0.0f,0.0f, 0.0f }, Vector3{ 1.0f});
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::CRATEGROUPONE, Vector3{ -5.0774f, -2.1827f,9.9088f }, Vector3{ 0.0f,0.0f, 0.0f }, Vector3{ 1.0f});
       
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PALLETGROUPONE, Vector3{ -6.248f, -2.762f,12.365f }, Vector3{ 0.0f,0.0f, 0.0f }, Vector3{ 1.0f});
        
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::BARREL_CLOTH, Vector3{ -4.815f, -2.468f,8.6577f }, Vector3{ 180.0f,11.0f, 180.0f }, Vector3{ 1.0f});
   
       
        
    }

    void GameManager::SetupStaticPhysics()
    {

        BoundingBox platform{ Vector3(-3.3f,-0.85f,-23.3f),Vector3(3.3f,0.85f,23.3f) };
        EntitySetup::CreatePhysicsBox(Vector3(-2.4022f, -3.501f, -4.4798f), Vector3(0.0f), platform, EnvironmentTag::Tag::NAVMESH);

        BoundingBox platform_pillar{ Vector3(-0.26f,-2.15f,-0.29f),Vector3(0.26f,2.15f,0.29f) };
        EntitySetup::CreatePhysicsBox(Vector3(-4.91f, -0.67f, -23.07f), Vector3(0.0f), platform_pillar);
        EntitySetup::CreatePhysicsBox(Vector3(-4.91f, -0.67f, -11.203f), Vector3(0.0f), platform_pillar);
        EntitySetup::CreatePhysicsBox(Vector3(-4.91f, -0.67f, -5.6f), Vector3(0.0f), platform_pillar);
        EntitySetup::CreatePhysicsBox(Vector3(-4.91f, -0.67f, 0.33f), Vector3(0.0f), platform_pillar);
        EntitySetup::CreatePhysicsBox(Vector3(-4.91f, -0.67f, 6.27f), Vector3(0.0f), platform_pillar);

        EntitySetup::CreatePhysicsBox(Vector3(1.9173f, 3.9406f, 0.0538f), Vector3(0.0f), platform_pillar);
        EntitySetup::CreatePhysicsBox(Vector3(8.1288f, 3.9406f, 0.0538f), Vector3(0.0f), platform_pillar);
        EntitySetup::CreatePhysicsBox(Vector3(1.9173f, 3.9406f, 8.8039f), Vector3(0.0f), platform_pillar);
        EntitySetup::CreatePhysicsBox(Vector3(8.1288f, 3.9406f, 8.8039f), Vector3(0.0f), platform_pillar);
       



        BoundingBox platform_stair{ Vector3(-1.05f,-0.25f,-3.7f),Vector3(1.05f,0.25f,3.7f) };
       EntitySetup::CreatePhysicsBox(Vector3(-2.7821f, -0.64509f, -3.3332f), Vector3(-42.0f,0.0f,0.0f), platform_stair, EnvironmentTag::Tag::NAVMESH);

        BoundingBox stair_wallside{ Vector3(-0.05f,-2.5f,-2.5f),Vector3(0.05f,2.5f,2.5f) };
        BoundingBox stair_wallback{ Vector3(-1.1f,-2.0f,-0.55f),Vector3(1.1f,2.0f,0.05f) };
        EntitySetup::CreatePhysicsBox(Vector3(-1.6961f, -0.62406f, -3.1440f), Vector3(0.0f, 0.0f, 0.0f), stair_wallside);
        EntitySetup::CreatePhysicsBox(Vector3(-3.8439f, -0.62406f, -3.1440f), Vector3(0.0f, 0.0f, 0.0f), stair_wallside);
        EntitySetup::CreatePhysicsBox(Vector3(-2.7542f, -0.62406f, -0.33856f), Vector3(0.0f, 0.0f, 0.0f), stair_wallback);


        BoundingBox platform_railbase{ Vector3(-3.0f,-0.25f,-43.0f),Vector3(3.0f,0.25f,43.0f) };
        EntitySetup::CreatePhysicsBox(Vector3(-8.5482f, -3.6971f, 2.1050f), Vector3(0.0f, 0.0f, 0.0f), platform_railbase,  EnvironmentTag::Tag::NAVMESH);

      /*  BoundingBox platform_rail{ Vector3(-0.1f,-0.05f,-43.0f),Vector3(0.1f,0.05f,43.0f) };
        EntitySetup::CreatePhysicsBox(Vector3(-6.7852f, -3.33f, 2.1050f), Vector3(0.0f, 0.0f, 0.0f), platform_rail, 1.0f);
        EntitySetup::CreatePhysicsBox(Vector3(-8.5591f, -3.33f, 2.1050f), Vector3(0.0f, 0.0f, 0.0f), platform_rail, 1.0f);*/
        BoundingBox platform_wall_long{ Vector3(-0.25f,-2.5f,-43.0f),Vector3(0.25f,2.5f,43.0f) };
        EntitySetup::CreatePhysicsBox(Vector3(-10.954f, -1.0f, 2.5347f), Vector3(0.0f), platform_wall_long);

        BoundingBox rail_side{ Vector3(-0.15f,-0.5f,-43.0f),Vector3(0.15f,0.5f,43.0f) };
        EntitySetup::CreatePhysicsBox(Vector3(-9.686f, -3.1434f, 2.5347f), Vector3(0.0f), rail_side);

    
        BoundingBox tunnel_long{ Vector3(-0.25f,-2.0f,-22.0f),Vector3(0.25f,2.0f,22.0f) };
        EntitySetup::CreatePhysicsBox(Vector3(0.6478f, -1.0f, -4.3531f), Vector3(0.0f), tunnel_long);

        BoundingBox tunnel_side{ Vector3(-0.25f,-2.0f,-14.0f),Vector3(0.25f,2.0f,14.0f) };
        EntitySetup::CreatePhysicsBox(Vector3(-5.323f, -1.0f, 32.15f), Vector3(0.0f), tunnel_side);
        
        BoundingBox tunnel_Left{ Vector3(-3.25f,-2.0f,-0.25f),Vector3(3.25f,2.0f,0.25f) };
        EntitySetup::CreatePhysicsBox(Vector3(-2.598f, -0.62403f, -26.908f), Vector3(0.0f), tunnel_Left);
        EntitySetup::CreatePhysicsBox(Vector3(-2.598f, -0.62403f, 17.937f), Vector3(0.0f), tunnel_Left);

        BoundingBox bench{ Vector3(-0.3f,-0.25f,-1.3f),Vector3(0.3f,0.25f,1.3f) };
        EntitySetup::CreatePhysicsBox(Vector3(-2.1854f, -2.1978f, 14.323f), Vector3(0.0f), bench);
        EntitySetup::CreatePhysicsBox(Vector3(-2.1854f, -2.1978f, 10.211f), Vector3(0.0f), bench);
        EntitySetup::CreatePhysicsBox(Vector3(-2.1854f, -2.1978f, -12.208f), Vector3(0.0f), bench);
        EntitySetup::CreatePhysicsBox(Vector3(-2.1854f, -2.1978f, -15.321f), Vector3(0.0f), bench);
        EntitySetup::CreatePhysicsBox(Vector3(-2.1854f, -2.1978f, -21.013f), Vector3(0.0f), bench);
        EntitySetup::CreatePhysicsBox(Vector3(-2.1854f, -2.1978f, -24.145f), Vector3(0.0f), bench);

        BoundingCylinder bin{ 1.0f,0.3f,0.3f ,BoundingCylinder::Axis::Y};
        EntitySetup::CreatePhysicsCylinder(Vector3(-2.1772f, -2.144f,12.2153f),Vector3(0.0f),bin);
        EntitySetup::CreatePhysicsCylinder(Vector3(-2.1772f, -2.144f, -18.127f),Vector3(0.0f),bin);
        EntitySetup::CreatePhysicsCylinder(Vector3(5.040f, 2.5224f, -0.07405f),Vector3(0.0f),bin);
        EntitySetup::CreatePhysicsCylinder(Vector3(0.4490f, 2.5224f, 8.86561f),Vector3(0.0f),bin);

        BoundingBox floorShort{ Vector3(-6.0f,-0.25f,-2.0f),Vector3(6.0f,0.25f,2.0f)};
        EntitySetup::CreatePhysicsBox(Vector3(4.3482f, 1.793f, -2.7210f), Vector3(0.0f), floorShort, EnvironmentTag::Tag::NAVMESH);

        BoundingBox floorLong{ Vector3(-9.0f,-0.25f,-8.0f),Vector3(9.0f,0.25f,8.0f) };
        EntitySetup::CreatePhysicsBox(Vector3(3.347f, 1.793f, 7.2647f), Vector3(0.0f), floorLong, EnvironmentTag::Tag::NAVMESH);

        
        BoundingBox wallback{ Vector3(-8.0f,-2.0f,-0.1f),Vector3(8.0f,2.0f,0.1f) };
        EntitySetup::CreatePhysicsBox(Vector3(3.820f, 4.1096f, -3.575f), Vector3(0.0f), wallback);

        BoundingBox wallfrnt{ Vector3(-16.0f,-2.0f,-0.1f),Vector3(16.0f,2.0f,0.1f) };
        EntitySetup::CreatePhysicsBox(Vector3(1.056f, 4.1096f, 13.138), Vector3(0.0f), wallfrnt);

        BoundingBox wallleft{ Vector3(-0.1f,-2.0f,-7.0f),Vector3(0.1f,2.0f,7.0f) };
        EntitySetup::CreatePhysicsBox(Vector3(11.8916, 4.1096f, 3.5832f), Vector3(0.0f), wallleft);

        BoundingBox wallright{ Vector3(-0.1f,-2.0f,-6.0f),Vector3(0.1f,2.0f,6.0f) };
        EntitySetup::CreatePhysicsBox(Vector3(-4.4169f, 4.1096f, 4.801f), Vector3(0.0f), wallright);

        BoundingBox grillLeft{ Vector3(-4.5f,-1.5f,-0.1f),Vector3(4.5f,1.5f,0.1f) };
        EntitySetup::CreatePhysicsBox(Vector3(7.2157f, 3.6373f, 5.0764f), Vector3(0.0f), grillLeft);


        BoundingBox grillRight{ Vector3(-2.5f,-1.5f,-0.1f),Vector3(2.5f,1.5f,0.1f) };
        EntitySetup::CreatePhysicsBox(Vector3(-2.0096f, 3.6373f, 5.0764f), Vector3(0.0f), grillRight);

        BoundingBox stairCover{ Vector3(-0.2f,-2.0f,-2.5f),Vector3(0.2f,2.0f,2.5f) };
        EntitySetup::CreatePhysicsBox(Vector3(-4.0176f, 4.173f, -1.1015f), Vector3(0.0f), stairCover);
        EntitySetup::CreatePhysicsBox(Vector3(-1.443f, 4.173f, -1.1015f), Vector3(0.0f), stairCover);

        BoundingBox railRamp{ Vector3(-1.0f,-0.25f,-1.25f),Vector3(1.0f,0.25f,1.25f) };
        EntitySetup::CreatePhysicsBox(Vector3(-6.4311f, -3.3262f, 16.19f), Vector3(0.0f,0.0f,27.0f), railRamp, EnvironmentTag::Tag::NAVMESH);

        BoundingBox railRampMid{ Vector3(-0.7f,-0.25f,-2.1f),Vector3(0.7f,0.25f,2.1f) };
        EntitySetup::CreatePhysicsBox(Vector3(-6.0378f, -3.2885f, -14.296), Vector3(0.0f, 0.0f, 40.0f), railRampMid, EnvironmentTag::Tag::NAVMESH);


        BoundingBox tunnelblockright{ Vector3(-2.5f,-1.0f,-1.0f),Vector3(2.5f,1.0f,1.0f) };
        EntitySetup::CreatePhysicsBox(Vector3(-8.222f, -2.1889f, -27.602f), Vector3(0.0f, 0.0f, 0.0f), tunnelblockright);

        BoundingBox platformblockLeft{ Vector3(-1.1f,-0.7f,-6.5f),Vector3(1.1f,0.7f,6.5f) };
        EntitySetup::CreatePhysicsBox(Vector3(-5.7543f, -2.5f, 6.4871f), Vector3(0.0f, 0.0f, 0.0f), platformblockLeft);
    }

    void GameManager::OnUpdateTestScene(const float deltaTime)
    {
        // Toggle Physics Simulation
        if (Input::InputSystem::GetInstance().GetKeyDown(Input::Key::G))
        {
            PhysicsSystem::SetPaused(!PhysicsSystem::GetPaused());
        }
    

        UpdateEnemies(deltaTime, m_playerPosition,m_buddyPosition);

        if (Application::GetCurrent().GetEditorActive())
        {
            Application::GetCurrent().GetAppWindow().SetMouseHidden(false);
            return;
        }


        UpdatePlayer(deltaTime);

       
    }

    void GameManager::OnUpdateMainScene(const float deltaTime)
    {

        UpdateMarkerAnim(deltaTime);

        // Toggle Physics Simulation
        if (Input::InputSystem::GetInstance().GetKeyDown(Input::Key::G))
        {
            PhysicsSystem::SetPaused(!PhysicsSystem::GetPaused());
        }

        UpdateEnemies(deltaTime, m_playerPosition,m_buddyPosition);

        UpdateBuddy(deltaTime);

        CheckandUpdateMarkedEnemy(deltaTime);
    
        ComponentView enemymarkView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<EnemyMark>();

        

        for (Entity entity : enemymarkView)
        {
            if (!entity.IsActive()) continue;

            Transform& transform = entity.GetComponent<Transform>();

            Vector3 direction = (transform.GetWorldPosition() - m_playerPosition);
            direction.y = 0;
            Quaternion rot = LookAtRotation(direction,Vector3(0.0f,1.0f,0.0f));


            transform.SetRotation(rot);
            break;

        }

        if (Application::GetCurrent().GetEditorActive())
        {
            Application::GetCurrent().GetAppWindow().SetMouseHidden(false);
            return;
        }


        UpdatePlayer(deltaTime);




    }

    void GameManager::UpdateMarkerAnim(const float deltaTime)
    {
        if (!m_waypointbaseEntity.IsActive()) { return; }


       Transform& transform = m_waypointarrowEntity.GetComponent<Transform>();

       sincounter +=  60 * deltaTime;

       if (sincounter > 360.0f)
       {
           sincounter = 0.0f;
       }

       const float yoffset = Sin(sincounter) * 0.2f + 0.7f;

       transform.SetPosition(Vector3(0.0f, yoffset, 0.0f));


       rotcounter += 45 * deltaTime;

       if (rotcounter > 180.0f) { rotcounter = 0.0f; }

       transform.SetEularRotation(Vector3(0.0f, rotcounter, 0.0f));

    }

    void GameManager::DebugNavmesh()
    {
      
        for (const NavTriangle& tri : m_debugnavmesh)
        {
            Vector3 vertexOne = tri.vertices[0];
            Vector3 vertexTwo = tri.vertices[1];
            Vector3 vertexThree = tri.vertices[2];

            Rendering::DebugRenderer::DrawLine(vertexOne, vertexTwo, Vector4(0.0f, 1.0f, 0.2f, 1.0f));
            Rendering::DebugRenderer::DrawLine(vertexTwo, vertexThree, Vector4(0.0f, 1.0f, 0.2f, 1.0f));
            Rendering::DebugRenderer::DrawLine(vertexThree, vertexOne, Vector4(0.0f, 1.0f, 0.2f, 1.0f));
        }

       
        if (m_simplifiedpath.empty())
        {
            //LOG_ERROR("NO NAVIGATION PATH");
            return;
        }

        for (int i = 0; i < m_simplifiedpath.size(); i++)
        {
            Vector3 curPos = m_simplifiedpath[i];
            Rendering::DebugRenderer::DebugDrawSphere(0.05f, curPos, Vector4(1.0f, 1.0f, 0.0f, 1.0f));

            if (i < m_simplifiedpath.size() - 1)
            {
                Vector3 nextpos = m_simplifiedpath[i + 1];

                Rendering::DebugRenderer::DrawLine(curPos, nextpos, Vector4(1.0f, 1.0f, 0.0f, 1.0f));
            }


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

       // OnInitTestScene();

        OnInitMainScene();
        SetupNavMesh();
        SetupPathFinder();

      
       


	}


	void GameManager::OnUpdate(const float deltaTime)
	{
        //OnUpdateTestScene(deltaTime);

        OnUpdateMainScene(deltaTime);
        DebugNavmesh();

	}

    void GameManager::MoveBuddyTo(const Vector3& position, RigidBody* markedEnemy)
    {
       
        m_markedEnemy = markedEnemy;

        ComponentView enemymarkView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<EnemyMark>();
        
        for (Entity enemy : enemymarkView)
        {
            enemy.SetActive(false);
        }

        if (m_markedEnemy != nullptr)
        {
            EnemyCharacter* enemy = m_markedEnemy->GetEntityId().GetComponent<EnemyComponent>().GetEnemy();

            const Vector3 position = m_markedEnemy->GetEntityId().GetComponent<Transform>().GetPosition() - Vector3{ 0.0f,0.8f,0.0f };


            
           
           enemy->MarkforBuddy(true);
               // const Vector3 position = m_markedEnemy->GetEntityId().GetComponent<Transform>().GetPosition() - Vector3{ 0.0f,0.8f,0.0f };

           m_waypointbaseEntity.SetActive(false);
           m_buddyRef->SetMarkedEnemy(position, true);
           
           
        }

        else
        {
            m_waypointbaseEntity.GetComponent<Transform>().SetPosition(position);
            m_waypointbaseEntity.SetActive(true);
            m_buddyRef->SetMarkedEnemy(Vector3{200.0f}, false);

        }




        m_simplifiedpath = m_pathFinder->FindSimplfiedPath(m_buddyPosition,position);
        m_buddyRef->MarkTargetandPath(position, m_simplifiedpath);

    }

    void GameManager::CheckandUpdateMarkedEnemy(const float& deltatime)
    {
        if (m_markedEnemy)
        {
      
            const Vector3 position = m_markedEnemy->GetEntityId().GetComponent<Transform>().GetPosition() - Vector3{ 0.0f,0.8f,0.0f };
            m_buddyRef->SetMarkedEnemy(position, true);
        }
        else
        {
            m_buddyRef->SetMarkedEnemy(Vector3{ 200.0f }, false);

        }

        //if (m_markedEnemy == nullptr)
        //{
        //    markenemycounter = 0.0f;
        //    return;
        //}

        //markenemycounter += deltatime;

        //if (markenemycounter > m_markRefresh)
        //{
        //    markenemycounter = 0.0f;
        //    //m_markedEnemy->GetEntityId().GetComponent<EnemyCharacter>().get

        //    const Vector3 enemyPosition = m_markedEnemy->GetEntityId().GetComponent<Transform>().GetPosition() - Vector3{ 0.0f,0.8f,0.0f };
        //    
        //    MoveBuddyTo(enemyPosition,m_markedEnemy);
        //}
        //

       
    }

    void GameManager::HideMarker()
    {
        m_waypointbaseEntity.SetActive(false);
    }


    void GameManager::SetEnemyDeath(RigidBody* enemyRef)
    {
        if (m_markedEnemy == enemyRef)
        {
            m_markedEnemy = nullptr;

            std::string name = enemyRef->GetEntityId().GetComponent<NameComponent>().name;

            LOG_WARN("ENEMY KILLED : {0}", name);

            m_buddyRef->SetMarkedEnemy(Vector3{200.0f}, false);
        }

       
    }
}