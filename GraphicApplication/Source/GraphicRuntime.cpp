#include "SaltnPepperEngine.h"



class GraphicRuntime : public Application
{
	void OnInit()
	{
        // Define all the scenes
        m_sceneManager->EnqueSceneFromFile("//Engine//Scene//TestScene.json");
        m_sceneManager->EnqueSceneFromFile("//Engine//Scene//LightScene.json");

        m_sceneManager->NextScene();


        //m_sceneManager->SwitchScene("TestScene");

	}



    

	void OnUpdate(float deltaTime)
	{

	}

};

int main(int argc, char* argv)
{
    // Creating a new Appinstance
    GraphicRuntime* app = new GraphicRuntime();

    //FilePath path = File::GetCurrentPath();

    // Always Initialize the App
    app->Initialize();
    // Running the Application
    app->Run();


    delete app;
}