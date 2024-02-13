#ifndef FIRSTPERSONCAMERACONTROLLER_H
#define FIRSTPERSONCAMERACONTROLLER_H

#include "CameraController.h"

namespace SaltnPepperEngine
{
	class FirstPersonCameraController : public CameraController
	{
	public:

		FirstPersonCameraController();
		virtual ~FirstPersonCameraController() override;

		virtual void MouseInput(Components::Transform& transform, Vector2 mousePosition, float deltaTime) override;
		virtual void KeyboardInput(Components::Transform& transform, float deltaTime) override;	
		void UpdateScroll(Transform& transform, float offset, float deltaTime) override;

	};

}


#endif // !FIRSTPERSONCAMERACONTROLLER_H



