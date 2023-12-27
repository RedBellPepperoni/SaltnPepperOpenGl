#include "InputSystem.h"




namespace SaltnPepperEngine
{
	namespace Input
	{
		// ============== KEYBOARD FUNCTIONS =========================

		bool InputSystem::GetKeyDown(Key key) const
		{
			return m_keyboardKeysPressed[(int)key];
		}

		bool InputSystem::GetKeyHeld(Key key) const
		{
			return m_keyboardKeysHeld[(int)key];
		}

		void InputSystem::SetKeyDown(Key key, bool pressed)
		{
			m_keyboardKeysPressed[(int)key] = pressed;
		}

		void InputSystem::SetKeyHeld(Key key, bool held)
		{
			m_keyboardKeysHeld[(int)key] = held;
		}


		// ============== MOUSE FUNCTIONS =========================

		bool InputSystem::GetMouseBtnClicked(MouseButton button) const
		{
			return m_mouseKeysPressed[(int)button];
		}

		bool InputSystem::GetMouseBtnHeld(MouseButton button) const
		{
			return m_mouseKeysHeld[(int)button];
		}

		void InputSystem::SetMouseBtnClicked(MouseButton button, bool clicked)
		{
			m_mouseKeysPressed[(int)button] = clicked;
		}

		void InputSystem::SetMouseBtnHeld(MouseButton button, bool held)
		{
			m_mouseKeysHeld[(int)button] = held;
		}


		void InputSystem::UpdateMousePosition(Vector2 newPosition)
		{
			m_mousePosition = newPosition;
		}


		const Vector2& InputSystem::GetMousePosition() const
		{
			return m_mousePosition;
		}

		void InputSystem::SetScrollDelta(float newOffset)
		{
			m_scrollDelta = newOffset;
		}

		const float InputSystem::GetScrollDelta() const
		{
			return m_scrollDelta;
		}




		// ============== KEYBOARD EVENTS =========================

		bool InputSystem::OnKeyDown(KeyDownEvent& event)
		{

			SetKeyDown(Key(event.GetKeyCode()), event.GetRepeatCount() < 1);
			SetKeyHeld(Key(event.GetKeyCode()), true);
			return false;
		}


		bool InputSystem::OnKeyUp(KeyUpEvent& event)
		{
			SetKeyDown(Key(event.GetKeyCode()), false);
			SetKeyHeld(Key(event.GetKeyCode()), false);
			return false;
		}



		// ============== MOUSE EVENTS =========================
		bool InputSystem::OnMouseDown(MouseButtonDownEvent& event)
		{
			SetMouseBtnClicked(MouseButton(event.GetMouseButton()), true);
			SetMouseBtnHeld(MouseButton(event.GetMouseButton()), true);
			return false;
		}

		bool InputSystem::OnMouseUp(MouseButtonUpEvent& event)
		{
			SetMouseBtnClicked(MouseButton(event.GetMouseButton()), false);
			SetMouseBtnHeld(MouseButton(event.GetMouseButton()), false);
			return false;
		}


		bool InputSystem::OnMouseMoved(MouseMovedEvent& event)
		{
			UpdateMousePosition(Vector2(event.GetXAxis(), event.GetYAxis()));
			return false;
		}


		bool InputSystem::OnMouseScrolled(MouseScrolledEvent& event)
		{
			// We only need the Up and down scroll
			SetScrollDelta(event.GetYDelta());
			return false;
		}




		InputSystem::InputSystem()
		{
			ResetAll();
		}

		void InputSystem::ProcessEvent(EventBase& event)
		{
			EventDispatcher dispatcher(event);

			dispatcher.dispatch<KeyDownEvent>(BIND_FN(OnKeyDown));
			dispatcher.dispatch<KeyUpEvent>(BIND_FN(OnKeyUp));

			dispatcher.dispatch<MouseButtonDownEvent>(BIND_FN(OnMouseDown));
			dispatcher.dispatch<MouseButtonUpEvent>(BIND_FN(OnMouseUp));

			dispatcher.dispatch<MouseMovedEvent>(BIND_FN(OnMouseMoved));
			dispatcher.dispatch<MouseScrolledEvent>(BIND_FN(OnMouseScrolled));

		}

		void InputSystem::ResetKeyPressed()
		{
			memset(m_keyboardKeysPressed, 0, MAX_KEYBOARD_KEYS);
			memset(m_mouseKeysPressed, 0, MAX_MOUSE_BUTTONS);
		}

		void InputSystem::ResetAll()
		{
			// Add mouse key reset here later
			ResetKeyPressed();

			memset(m_keyboardKeysHeld, 0, MAX_KEYBOARD_KEYS);

			m_mousePosition = Vector2(0.0f);

			memset(m_mouseKeysHeld, 0, MAX_MOUSE_BUTTONS);

			m_scrollDelta = 0.0f;
		}



	}
}