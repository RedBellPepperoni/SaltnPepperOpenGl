#ifndef  ENTITYSETUP_H
#define  ENTITYSETUP_H

#include "SaltnPepperEngine.h"
#include "Engine/Core/Rendering/Geometry/SkinnedMesh.h"

namespace SaltnPepperEngine 
{


	void LoadAllModels();


	void LoadAllTextures();
	

	Entity CreateMainCamera(Vector3 Position = Vector3{ 0.0f }, Vector3 Rotation = Vector3{ 0.0f });
	

	Entity CreateParentedEntity(Vector3 Position = Vector3{ 0.0f }, Vector3 Rotation = Vector3{ 0.0f });
	


	Entity CreateDirectionalLight();
	
	Entity SpawnSkinnedCharacter(const Vector3& position);
}
// Asteroid Stuff

#endif //  EntitySetup

