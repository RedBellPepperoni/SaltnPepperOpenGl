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

        
        PhysicsSystem::SetPaused(true);

        SetupStaticMeshes();
        SetupStaticPhysics();

        EntitySetup::CreateDirectionalLight(Vector3(-11.4192f, 51.3504f, -7.0023f));
        body = EntitySetup::CreatePlayer(Vector3(-4.1f, 3.0f, 4.6f), Vector3(0.0f, 0.0f, 0.0f));
       
       // EntitySetup::CreatePhysicsFloor(Vector3(-2.0f, -3.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));

       



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
        EntitySetup::CreateStaticEntity(EntitySetup::SubwayModel::PLATFROM_BIN, Vector3(0.4520f, 2.5403f, 8.8623f), Vector3(0.0f, 20.0f, 0.0f), Vector3(1.0f));

    }

    void GameManager::SetupStaticPhysics()
    {

        BoundingBox platform{ Vector3(-3.3f,-0.85f,-23.3f),Vector3(3.3f,0.85f,23.3f) };
        EntitySetup::CreatePhysicsBox(Vector3(-2.4022f, -3.501f, -4.4798f), Vector3(0.0f), platform);

        

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

        // Toggle Physics Simulation
        if (Input::InputSystem::GetInstance().GetKeyDown(Input::Key::G))
        {
            PhysicsSystem::SetPaused(!PhysicsSystem::GetPaused());
        }

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