#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "Engine/Core/System/Events/Event.h"
#include "Engine/Core/System/Events/KeyboardEvent.h"
#include "Engine/Core/System/Events/MouseEvent.h"
#include "Engine/Utils/Singleton.h"
#include "Engine/Utils/Maths/MathDefinitions.h"

namespace SaltnPepperEngine
{
	namespace Input
	{
		enum MouseState
		{
			Hidden,
			Visible,
			Captured
		};


#define MAX_KEYBOARD_KEYS 350
#define MAX_MOUSE_BUTTONS 16

		class InputSystem : public ThreadSafeSingleton<InputSystem>
		{
		public:
			InputSystem();

			// Virtual since it inherits from singleton
			virtual ~InputSystem() = default;

			void ProcessEvent(EventBase& event);

			// =============== KEYBOARD INPUT ======================

			bool GetKeyDown(Key key) const;
			bool GetKeyHeld(Key key) const;

			void SetKeyDown(Key key, bool pressed);
			void SetKeyHeld(Key key, bool held);


			// =================== MOUSE INPUT ======================

			// Getters for  Mouse Button Clicks/Hold
			bool GetMouseBtnClicked(MouseButton button) const;
			bool GetMouseBtnHeld(MouseButton button) const;


			// Setters for Mouse Button presses, only teh event system should call these ideally
			void SetMouseBtnClicked(MouseButton button, bool clicked);
			void SetMouseBtnHeld(MouseButton button, bool held);


			// ================= MOUSE SCREEN POSITION =======================

			void UpdateMousePosition(Vector2 newPosition);
			const Vector2& GetMousePosition() const;


			MouseState GetMouseState() { return m_mouseState; }
			void SetMouseState(MouseState state) { m_mouseState = state; }

			// ================ SCROLL STUFF ==========================

			void SetScrollDelta(float newOffset);
			const float GetScrollDelta() const;



			// =========== UTILITIES =====================
			void ResetKeyPressed();
			void ResetAll();

		protected:

			// =========== KEYBOARd EVENTS ================

			bool OnKeyDown(KeyDownEvent& event);
			bool OnKeyUp(KeyUpEvent& event);

			// ============= MOUSE CLICK EVENTS ====================

			bool OnMouseDown(MouseButtonDownEvent& event);
			bool OnMouseUp(MouseButtonUpEvent& event);

			// =========== MOUSE MOVE EVENTS ===========

			bool OnMouseMoved(MouseMovedEvent& event);
			bool OnMouseScrolled(MouseScrolledEvent& event);

		protected:

			// Keyboard key Map for storeing the keys that are pressed
			bool m_keyboardKeysPressed[MAX_KEYBOARD_KEYS];

			// Key map for keys that havent been released yet
			bool m_keyboardKeysHeld[MAX_KEYBOARD_KEYS];

			// Mouse Button map for buttons that are pressed
			bool m_mouseKeysPressed[MAX_MOUSE_BUTTONS];

			// Mouse map for repeat pressed button that havent been released
			bool m_mouseKeysHeld[MAX_MOUSE_BUTTONS];

			// The scroff offset needed to check the amount mouse has scrooled 
			float m_scrollDelta = 0.0f;

			// The state of the mouse posinter
			MouseState m_mouseState;

			// Position of the mouse on the GLFW Window 
			Vector2 m_mousePosition;

		};

	}


	using namespace Input;
}


#endif // !INPUTSYSTEM_H


