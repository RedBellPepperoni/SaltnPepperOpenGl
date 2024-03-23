#include "FirstPersonCameraController.h"
#include "Engine/Core/System/Application/Application.h"
#include "Engine/Core/System/Window/Window.h"
#include "Engine/Core/System/Input/InputSystem.h"
#include "Engine/Core/Components/Transform.h"

namespace SaltnPepperEngine
{
	FirstPersonCameraController::FirstPersonCameraController()
	{

	}

	FirstPersonCameraController::~FirstPersonCameraController()
	{

	}

	void FirstPersonCameraController::MouseInput(Components::Transform& transform, Vector2 mousePosition, float deltaTime)
	{
		// Window is not in focus , don't do the mosue update
		if (!Application::GetCurrent().GetAppWindow().GetFocus())
		{
			return;
		}
		
		m_sensitivity = 0.0002f;
		m_rotationalVelocity = Vector2((mousePosition.x - m_previousCursorPosition.x), (mousePosition.y - m_previousCursorPosition.y)) * m_sensitivity * 10.0f;

		//Application::GetCurrent().GetAppWindow().SetMouseHidden(true);

		if (Length(m_rotationalVelocity) > 0.0001f)
		{
			//Get The Rotation
			Quaternion rotation = transform.GetRotation();

			
			Quaternion Pitch = glm::angleAxis(-m_rotationalVelocity.y, Vector3(1.0f, 0.0f, 0.0f));
			Quaternion Yaw = glm::angleAxis(-m_rotationalVelocity.x, Vector3(0.0f, 1.0f, 0.0f));


			rotation = Yaw * rotation;
			rotation = rotation * Pitch;



			transform.SetRotation(rotation);

			m_previousCursorPosition = mousePosition;





		}
		else
		{
			m_rotationalVelocity = Vector3(0.0f);
		}



		

		m_rotationalVelocity = m_rotationalVelocity * pow(m_rotationalDampening, deltaTime);


	}

	void FirstPersonCameraController::KeyboardInput(Components::Transform& transform, float deltaTime)
	{
		// No need for Keyboardinput of the camera
	}

	void FirstPersonCameraController::UpdateScroll(Transform& transform, float offset, float deltaTime)
	{
		//No need for Scroll
	}

}