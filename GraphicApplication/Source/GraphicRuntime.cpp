#include "SaltnPepperEngine.h"



class GraphicRuntime : public Application
{
	void OnInit()
	{

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