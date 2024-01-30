#ifndef EDITORCAMERACONTROLLER_H
#define EDITORCAMERACONTROLLER_H
#include "CameraController.h"

namespace SaltnPepperEngine
{
	using namespace Components;

	enum class EditorCameraMode
	{
		NONE,
		FLYCAM,
		ARCBALL,
		TWODIM
	};

	class EditorCameraController : public CameraController
	{
	public:

		EditorCameraController();
		~EditorCameraController();

		virtual void MouseInput(Transform& transform, Vector2 mousePosition, float deltaTime) override;
		virtual void KeyboardInput(Transform& transform, float deltaTime) override;
		void UpdateCameraView(Transform& transform, float delta);


		void MousePan(Transform& transform, const Vector2& delta);
		void MouseRotate(Transform& transform, const Vector2& delta);
		void MouseZoom(Transform& transform, float delta);

		Vector3 CalculatePosition(Transform& transform) const;
		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

		void UpdateScroll(Transform& transform, float offset, float dt) override;

		void StopMovement();
		void SetSpeed(float speed) { m_cameraSpeed = speed; }

		void SetCurrentMode(EditorCameraMode mode) { m_cameraMode = mode; }
		EditorCameraMode GetCurrentMode() const { return m_cameraMode; }


	private:

		EditorCameraMode m_cameraMode = EditorCameraMode::FLYCAM;

		Vector2 m_storedCursorPosition;
		float m_cameraSpeed;

		float m_PitchDelta = 0.0f;
		float m_YawDelta = 0.0f;

		Vector3 m_PositionDelta{};


	};

}


#endif // !FLYCAMERACONTROLLER_H
