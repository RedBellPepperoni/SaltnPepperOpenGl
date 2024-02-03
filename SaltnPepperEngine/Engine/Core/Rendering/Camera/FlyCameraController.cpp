#include "FlyCameraController.h"
#include "Engine/Core/Components/Transform.h"
#include "Engine/Core/System/Application/Application.h"
#include "Engine/Core/System/Input/InputSystem.h"
#include "Engine/Core/System/Window/Window.h"

namespace SaltnPepperEngine
{

	using namespace Components;

	FlyCameraController::FlyCameraController()
	{
		m_velocity = Vector3{ 0.0f };
		m_focalPoint = Vector3{ 0.0f };

		m_previousCursorPosition = Vector3{ 0.0f };
		m_rotationalVelocity = Vector2{ 0.0f };



	}

	FlyCameraController::~FlyCameraController()
	{
       
	}

	void FlyCameraController::MouseInput(Transform& transform, Vector2 mousePosition, float deltaTime)
	{
      //  m_distance = glm::distance(transform.GetPosition(), m_focalPoint);


		static bool mouseHeld = false;

		if (Input::InputSystem::GetInstance().GetMouseBtnClicked(Input::MouseButton::Right))
		{
			mouseHeld = true;
			Application::GetCurrent().GetAppWindow().SetMouseHidden(true);

			m_storedCursorPosition = mousePosition;
			m_previousCursorPosition = m_storedCursorPosition;



		}

		if (Input::InputSystem::GetInstance().GetMouseBtnHeld(Input::MouseButton::Right))
		{
			m_sensitivity = 0.0002f;
			m_rotationalVelocity = Vector2((mousePosition.x - m_previousCursorPosition.x), (mousePosition.y - m_previousCursorPosition.y)) * m_sensitivity * 10.0f;

		}
		else
		{
			if (mouseHeld)
			{
				mouseHeld = false;
				// INFINITIE MOUse on x and Y no edge stopping
				Application::GetCurrent().GetAppWindow().SetMouseHidden(false);
				Application::GetCurrent().GetAppWindow().SetMousePosition(m_storedCursorPosition);
			}

		}

		if (Length(m_rotationalVelocity) > 0.0001f || m_PitchDelta > 0.0001f || m_YawDelta > 0.0001f)
		{


			Quaternion rotation = transform.GetRotation();


			Quaternion Pitch = glm::angleAxis(-m_rotationalVelocity.y, Vector3(1.0f, 0.0f, 0.0f));
			Quaternion Yaw = glm::angleAxis(-m_rotationalVelocity.x, Vector3(0.0f, 1.0f, 0.0f));



			

			rotation = Yaw * rotation;
			rotation = rotation * Pitch;

			transform.SetRotation(rotation);
			//LOG_INFO("{0}, {1}, {2}, {3}", rotation.x, rotation.y, rotation.z, rotation.w);

			m_previousCursorPosition = mousePosition;



		}
		/*else
		{
			m_rotationalVelocity = Vector3(0.0f);
		}*/



		m_rotationalVelocity = m_rotationalVelocity * pow(m_rotationalDampening, deltaTime);

		UpdateCameraView(transform, deltaTime);
	}

	void FlyCameraController::KeyboardInput(Transform& transform, float deltaTime)
	{
		// Defalut speed
		m_cameraSpeed = 80.0f * deltaTime;

		// Way more if needed
		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::LeftShift))
		{
			m_cameraSpeed = 1600.0f * deltaTime;
		}




		// ============ FORWARD CAMERA MOVEMENT =================

		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::W))
		{
			m_velocity += transform.GetForwardVector() * m_cameraSpeed;


		}

		// ================ REVERSE CAMERA MOVEMENT =====================

		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::S))
		{
			m_velocity -= transform.GetForwardVector() * m_cameraSpeed;

		}

		// ================ RIGHT CAMERA MOVEMENT =====================

		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::A))
		{
			m_velocity -= transform.GetRightVector() * m_cameraSpeed;
		}

		// ================ LEFT CAMERA MOVEMENT =====================


		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::D))
		{
			m_velocity += transform.GetRightVector() * m_cameraSpeed;
		}

		// ================ UP CAMERA MOVEMENT =====================


		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::Space))
		{
			m_velocity += Vector3(0.0f, 1.0f, 0.0f) * m_cameraSpeed;
		}

		// ================ DOWN CAMERA MOVEMENT =====================

		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::LeftControl))
		{
			m_velocity -= Vector3(0.0f, 1.0f, 0.0f) * m_cameraSpeed;
		}


		// If there is velocity, move the Object

		if (m_velocity.length() > 0.00001f)
		{
			Vector3 position = transform.GetPosition();
			position += m_velocity * deltaTime;
			transform.SetPosition(position);
			m_velocity = m_velocity * pow(m_linearDampening, deltaTime);
		}

		else
		{
			m_velocity = Vector3(0.0f);

		}
	}

	void FlyCameraController::UpdateCameraView(Transform& transform, float delta)
	{
		const float yawSign = transform.GetUpVector().y < 0 ? -1.0f : 1.0f;

		// Extra step to handle the problem when the camera direction is the same as the up vector
		const float cosAngle = glm::dot(transform.GetForwardVector(), transform.GetUpVector());
		if (cosAngle * yawSign > 0.99f)
			m_PitchDelta = 0.f;

		// damping for smooth camera
		m_YawDelta *= pow(m_linearDampening, delta);
		m_PitchDelta *= pow(m_linearDampening, delta);
		m_PositionDelta *= pow(m_linearDampening, delta);
	}
}
