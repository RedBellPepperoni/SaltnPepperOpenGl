#ifndef CAMERACONTROLLER_H 
#define CAMERACONTROLLER_H 

#include "Engine/Macros.h"
#include "Engine/Utils/Maths/MathDefinitions.h"

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		class Camera;
	}
	
	using Rendering::Camera;

	namespace Components
	{
		class Transform;
	}

	using Components::Transform;

	class CameraController
	{
	public:

		CameraController() = default;
		virtual ~CameraController() = default;

		virtual void KeyboardInput(Transform& transform, float deltaTime) = 0;
		virtual void MouseInput(Transform& transform, Vector2 mousePosition, float deltaTime) = 0;
		virtual void UpdateScroll(Transform& transform, float offset, float deltaTime) {};
		//virtual void SetFocalPoint(const Vector3& point) {};



		void SetCamera(Camera* camera) 
		{
			CHECKNULL(camera);
			m_cameraRef = camera;
		}

		const Vector3& GetVelocity() const { return m_velocity; }

		Camera* GetCamera()
		{
			return m_cameraRef;
		}

		void SetMouseSensitivity(float newSensitivity)
		{
			m_sensitivity = newSensitivity;
		}



	protected:

		Vector3 m_focalPoint{ 0.0f };
		Vector3 m_velocity{ 0.0f };
		Vector2 m_rotationalVelocity{ 0.0f };
		Vector2 m_ArcRotateVelocity {0.0f};

		float m_cameraSpeed = 100.0f;
		float m_distance = 0.0f;
		float m_zoom = 1.0f;
		float m_zoomVelocity = 0.0f;

		Vector2 m_projectionOffset{ 0.0f };
		Vector2 m_previousCursorPosition{ 0.0f };
		float m_sensitivity = 0.001f;

		float m_linearDampening = 0.000001f;
		float m_rotationalDampening = 0.00005f;
		float m_zoomDampening = 0.000001f;

		Camera* m_cameraRef = nullptr;

	};


}

#endif // !CAMERACONTROLLER_H
