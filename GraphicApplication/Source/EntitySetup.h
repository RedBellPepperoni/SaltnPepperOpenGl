#ifndef  ENTITYSETUP_H
#define  ENTITYSETUP_H

#include "SaltnPepperEngine.h"

static int cameraCount = 0;
namespace SaltnPepperEngine {

	void LoadAllModels();


	void LoadAllTextures();


	Entity CreateMainCamera(Vector3 Position = Vector3{ 0.0f }, Vector3 Rotation = Vector3{ 0.0f });
	

	Entity CreateParentedEntity(Vector3 Position = Vector3{ 0.0f }, Vector3 Rotation = Vector3{ 0.0f });


	Entity CreateDirectionalLight();


	Entity CreateWall(const Vector3& position);
	

	Entity CreateFloor(const Vector3& position);

}
// Asteroid Stuff

#endif //  EntitySetup

