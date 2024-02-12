#include "ThirdPersonCameraController.h"
#include "Engine/Core/System/Input/InputSystem.h"
#include "Engine/Core/Components/Transform.h"
#include "Engine/Core/System/Application/Application.h"
#include "Engine/Core/System/Window/Window.h"

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		void ThirdPersonCameraController::MouseInput(Components::Transform& transform, Vector2 mousePosition, float deltaTime)
		{
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
				m_sensitivity = 0.00002f;
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



				MouseRotate(transform, m_rotationalVelocity);
				m_previousCursorPosition = mousePosition;

				Quaternion rotation = transform.GetRotation();


				Quaternion Pitch = glm::angleAxis(-m_PitchDelta, Vector3(1.0f, 0.0f, 0.0f));
				Quaternion Yaw = glm::angleAxis(-m_YawDelta, Vector3(0.0f, 1.0f, 0.0f));


				rotation = Yaw * rotation;
				rotation = rotation * Pitch;

				transform.SetRotation(rotation);

				//m_previousCurserPos = mousePosition;



			}
			else
			{
				m_rotationalVelocity = Vector3(0.0f);
			}



			m_rotationalVelocity = m_rotationalVelocity * pow(m_rotationalDampening, deltaTime);
			UpdateCameraView(transform, deltaTime);

			transform.SetPosition(CalculatePosition(transform));

		}

		void ThirdPersonCameraController::KeyboardInput(Components::Transform& transform, float deltaTime)
		{
			

		}

		void ThirdPersonCameraController::UpdateCameraView(Components::Transform& transform, float delta)
		{
			const float YawSign = transform.GetUpVector().y < 0 ? -1.0f : 1.0f;

			// Extra step to handle the problem when the camera direction is the same as the up vector
			const float cosAngle = glm::dot(transform.GetForwardVector(), transform.GetUpVector());
			if (cosAngle * YawSign > 0.99f)
				m_PitchDelta = 0.f;

			// damping for smooth camera
			m_YawDelta *= pow(m_linearDampening, delta);
			m_PitchDelta *= pow(m_linearDampening, delta);
			m_PositionDelta *= pow(m_linearDampening, delta);
		}

		void ThirdPersonCameraController::SetFocalPoint(const Vector3& point)
		{
			m_focalPoint = point;
		}

		void ThirdPersonCameraController::MouseRotate(Components::Transform& transform, const Vector2& delta)
		{
			const float yawSign = transform.GetUpVector().y < 0.0f ? -1.0f : 1.0f;
			m_YawDelta += yawSign * delta.x * 0.3f;
			m_PitchDelta += delta.y * 0.3f;
		}

		Vector3 ThirdPersonCameraController::CalculatePosition(Components::Transform& transform)
		{
			return m_focalPoint - transform.GetForwardVector() * m_distance;
		}
	}
}