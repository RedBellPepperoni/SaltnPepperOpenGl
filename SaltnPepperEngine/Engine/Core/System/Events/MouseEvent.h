#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include "Engine/Core/System/Events/Event.h"
#include "Engine/Core/System/Input/InputKeyCodes.h"


namespace SaltnPepperEngine
{
	// Deals with the event when the mouse is moved






	// ============================== BAse Mouse Input event =====================
	// Parent event class to help with mouse click and release

	class MouseButtonEvent : public EventBase
	{
	public:

		Input::MouseButton GetMouseButton() const { return mouseButton; }

		EVENT_CATEGORY(EventCategoryMouse | EventCategoryInput)

	protected:

		MouseButtonEvent(Input::MouseButton button) : mouseButton(button) {}

		Input::MouseButton mouseButton;

	};


	// ========================= Mouse Button Down =====================================
	// When a mouse button is pressed , this event is triggered

	class MouseButtonDownEvent : public MouseButtonEvent
	{
	public:

		MouseButtonDownEvent(Input::MouseButton button)
			: MouseButtonEvent(button)
		{

		}

		EVENT_TYPE(MouseButtonDown)
	};


	// ============================== Mouse Button Up ========================================
	// Whenever a pressed button is released thsi event fires

	class MouseButtonUpEvent : public MouseButtonEvent
	{
	public:
		MouseButtonUpEvent(Input::MouseButton button) : MouseButtonEvent(button) {}
		EVENT_TYPE(MouseButtonUp)

	};


	class MouseMovedEvent : public EventBase
	{
	public:
		MouseMovedEvent(float axisX, float axisY) : mouseX(axisX), mouseY(axisY) {}


		inline float GetXAxis() { return mouseX; }
		inline float GetYAxis() { return mouseY; }

		EVENT_CATEGORY(EventCategoryMouse | EventCategoryInput)
			EVENT_TYPE(MouseMoved)

	private:
		float mouseX, mouseY;
	};


	class MouseScrolledEvent : public EventBase
	{
	public:

		MouseScrolledEvent(float newXDelta, float newYDelta)
			: xDelta(newXDelta)
			, yDelta(newYDelta)
		{

		}


		float GetXDelta() const { return xDelta; }
		float GetYDelta() const { return yDelta; }

		EVENT_CATEGORY(EventCategoryMouse | EventCategoryInput)
			EVENT_TYPE(MouseScrolled)

	private:

		float xDelta;
		float yDelta;
	};

}

#endif // !MOUSEEVENT_H
