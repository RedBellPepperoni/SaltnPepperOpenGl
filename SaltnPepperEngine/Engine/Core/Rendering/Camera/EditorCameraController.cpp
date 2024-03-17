#include "EditorCameraController.h"
#include "Engine/Core/Components/Transform.h"
#include "Engine/Core/System/Application/Application.h"
#include "Engine/Core/System/Input/InputSystem.h"
#include "Engine/Core/System/Window/Window.h"
#include "Engine/Core/Rendering/Camera/Camera.h"

namespace SaltnPepperEngine
{

	using namespace Components;

	EditorCameraController::EditorCameraController()
	{
		m_velocity = Vector3{ 0.0f };
		m_focalPoint = Vector3{ 0.0f };

		m_previousCursorPosition = Vector3{ 0.0f };
		m_rotationalVelocity = Vector2{ 0.0f };

		m_sensitivity = 0.00001f;
		m_zoomDampening = 0.00001f;
		m_linearDampening = 0.00001f;
		m_rotationalDampening = 0.0000001f;
		m_cameraMode = EditorCameraMode::FLYCAM;

	}

	EditorCameraController::~EditorCameraController()
	{
       
	}

	void EditorCameraController::MouseInput(Transform& transform, Vector2 mousePosition, float deltaTime)
	{
		//return;

		m_distance = Distance(transform.GetPosition(), m_focalPoint);
		
		if (m_cameraMode == EditorCameraMode::TWODIM)
		{
			//static bool mouseHeld = false;

			// Work on this Later
		}

		else
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
				else if (Input::InputSystem::GetInstance().GetMouseBtnHeld(Input::MouseButton::Middle))
				{
					MousePan(transform, Vector2((mousePosition.x - m_previousCursorPosition.x),(mousePosition.y - m_previousCursorPosition.y)));
				}

			}

			if (Length(m_rotationalVelocity) > 0.0001f || m_PitchDelta > 0.0001f || m_YawDelta > 0.0001f)
			{

				if (m_cameraMode == EditorCameraMode::ARCBALL)
				{
					MouseRotate(transform, m_rotationalVelocity);
					m_previousCursorPosition = mousePosition;

					Quaternion rotation = transform.GetRotation();
					Quaternion rotationX = glm::angleAxis(-m_PitchDelta, Vector3(1.0f, 0.0f, 0.0f));
					Quaternion rotationY = glm::angleAxis(-m_YawDelta, Vector3(0.0f, 1.0f, 0.0f));

					rotation = rotationY * rotation;
					rotation = rotation * rotationX;
					transform.SetRotation(rotation);
				}

				else if (m_cameraMode == EditorCameraMode::FLYCAM)
				{
					Quaternion rotation = transform.GetRotation();
					Quaternion Pitch = glm::angleAxis(-m_rotationalVelocity.y, Vector3(1.0f, 0.0f, 0.0f));
					Quaternion Yaw = glm::angleAxis(-m_rotationalVelocity.x, Vector3(0.0f, 1.0f, 0.0f));


					rotation = Yaw * rotation;
					rotation = rotation * Pitch;

					transform.SetRotation(rotation);
					m_previousCursorPosition = mousePosition;

					//LOG_INFO("{0} : {1} : {2} : {3}",rotation.x, rotation.y,rotation.z,rotation.w);
				}



			}
			/*else
			{
				m_rotationalVelocity = Vector3(0.0f);
			}*/



		}

		m_rotationalVelocity = m_rotationalVelocity * pow(m_rotationalDampening, deltaTime);

		if (m_cameraMode == EditorCameraMode::ARCBALL)
		{
			MouseZoom(transform, InputSystem::GetInstance().GetScrollDelta());
			UpdateCameraView(transform, deltaTime);

			transform.SetPosition(CalculatePosition(transform));
		}

		else if (m_cameraMode == EditorCameraMode::FLYCAM)
		{
			UpdateScroll(transform, InputSystem::GetInstance().GetScrollDelta(), deltaTime);
		}


	}

	void EditorCameraController::KeyboardInput(Transform& transform, float deltaTime)
	{
		// Defalut speed
		m_cameraSpeed = 80.0f * deltaTime;

		// Way more if needed
		/*if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::LeftShift))
		{
			m_cameraSpeed = 1600.0f * deltaTime;
		}*/




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


		//if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::Space))
		//{
		//	m_velocity += Vector3(0.0f, 1.0f, 0.0f) * m_cameraSpeed;
		//}

		//// ================ DOWN CAMERA MOVEMENT =====================

		//if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::LeftControl))
		//{
		//	m_velocity -= Vector3(0.0f, 1.0f, 0.0f) * m_cameraSpeed;
		//}


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

		/*Vector3 position = transform.GetPosition();
		LOG_WARN("{0} : {1} : {2}", position.x, position.y, position.z);*/
	}

	void EditorCameraController::UpdateCameraView(Transform& transform, float delta)
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


	void EditorCameraController::MousePan(Transform& transform, const Vector2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_focalPoint -= transform.GetRightVector() * delta.x * xSpeed * m_distance;
		m_focalPoint += transform.GetUpVector() * delta.y * ySpeed * m_distance;
	}


	void EditorCameraController::MouseRotate(Transform& transform, const Vector2& delta)
	{
		const float yawSign = transform.GetUpVector().y < 0.0f ? -1.0f : 1.0f;
		m_YawDelta += yawSign * delta.x * RotationSpeed();
		m_PitchDelta += delta.y * RotationSpeed();
	}


	void EditorCameraController::MouseZoom(Transform& transform, float delta)
	{
		//if (delta == 0.0f)
		//	return;

		//m_distance -= delta * ZoomSpeed();
		//const Vector3 forwardDir = transform.GetForwardVector();
		//// transform.SetLocalPosition(m_FocalPoint - forwardDir * m_Distance);
		//if (m_distance < 1.0f)
		//{
		//	m_focalPoint += forwardDir * m_distance;
		//	m_distance = 1.0f;
		//}

		//m_PositionDelta += delta * ZoomSpeed() * forwardDir;
	}

	Vector3 EditorCameraController::CalculatePosition(Transform& transform) const
	{
		return m_focalPoint + transform.GetForwardVector() * m_distance + m_PositionDelta;
	}
	std::pair<float, float> EditorCameraController::PanSpeed() const
	{
		const float x = Min(float(1920) / 1000.0f, 2.4f); // max = 2.4f
		const float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		const float y = Max(float(1080) / 1000.0f, 2.4f); // max = 2.4f
		const float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCameraController::RotationSpeed() const
	{
		return 0.3f;
	}

	float EditorCameraController::ZoomSpeed() const
	{
		float distance = m_distance * 0.2f;

		distance = Max(distance, 0.0f);

		float speed = distance * distance;

		speed = Min(speed, 50.0f); // 

		return speed * m_cameraSpeed / 10000.0f;

	}


	void EditorCameraController::UpdateScroll(Transform& transform, float offset, float dt)
	{
		/*if (m_cameraMode == EditorCameraMode::TWODIM)
		{
			if (!m_cameraRef)
				return;

			float multiplier = m_cameraSpeed / 10.0f;
			if (InputSystem::GetInstance().GetKeyHeld(Key::LeftShift))
			{
				multiplier = m_cameraSpeed / 2.0f;
			}

			if (offset != 0.0f)
			{
				m_zoomVelocity += dt * offset * multiplier;
			}

			if (!FloatEquals(m_zoomVelocity, 0.0f))
			{
				float scale = m_cameraRef->GetOrthoSize();

				scale -= m_zoomVelocity;

				if (scale < 0.15f)
				{
					scale = 0.15f;
					m_zoomVelocity = 0.0f;
				}
				else
				{
					m_zoomVelocity = m_zoomVelocity * pow(m_zoomDampening, dt);
				}

				m_cameraRef->SetOrthoSize(scale);
			}
		}
		else
		{

			if (offset != 0.0f)
			{
				m_zoomVelocity -= dt * offset * 10.0f;
			}

			if (!FloatEquals(m_zoomVelocity, 0.0f))
			{
				Vector3 pos = transform.GetPosition();
				pos += transform.GetForwardVector() * m_zoomVelocity;
				m_zoomVelocity = m_zoomVelocity * pow(m_zoomDampening, dt);

				transform.SetPosition(pos);
			}
		}*/
	}


	void EditorCameraController::StopMovement()
	{
		m_zoomVelocity = 0.0f;
		m_velocity = Vector3(0.0f);
		m_rotationalVelocity = Vector2(0.0f);
	}
}
