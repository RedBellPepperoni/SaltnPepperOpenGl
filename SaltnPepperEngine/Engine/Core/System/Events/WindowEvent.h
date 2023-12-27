#ifndef WINDOWEVENT_H
#define WINDOWEVENT_H

#include "Engine/Core/System/Events/Event.h"

namespace SaltnPepperEngine
{
	//---------------------------- WINDOW ENTER AND EXIT EVENTS ----------------------------

	class WindowMouseEnterEvent : public EventBase
	{
		WindowMouseEnterEvent() {}

		EVENT_TYPE(WindowMouseEnter)
			EVENT_CATEGORY(EventCategoryApplication)
	};

	class WindowMouseExitEvent : public EventBase
	{
		WindowMouseExitEvent() {}

		EVENT_TYPE(WindowMouseExit)
			EVENT_CATEGORY(EventCategoryApplication)
	};

	//---------------------------- WINDOW CLOSE EVENT ----------------------------

	class WindowCloseEvent : public EventBase
	{
	public:
		WindowCloseEvent() {}

		EVENT_TYPE(WindowClose)
			EVENT_CATEGORY(EventCategoryApplication)
	};


	//---------------------------- WINDOW RESIZE EVENTS ----------------------------

	class WindowResizeEvent : public EventBase
	{
	public:
		WindowResizeEvent(Vector2Int oldSize, Vector2Int newSize) : oldDimensions(oldSize), newDimensions(newSize) {}


		inline Vector2Int GetOldSize() const { return oldDimensions; }
		inline Vector2Int GetNewSize() const { return newDimensions; }

		EVENT_TYPE(WindowResize)
			EVENT_CATEGORY(EventCategoryApplication)

	private:
		Vector2Int oldDimensions, newDimensions;

	};


	class WindowFocusEvent : public EventBase
	{
	public:
		WindowFocusEvent() {}

		EVENT_TYPE(WindowFocus)
			EVENT_CATEGORY(EventCategoryApplication)

	};

	class WindowLostFocusEvent : public EventBase
	{
	public:
		WindowLostFocusEvent() {}

		EVENT_TYPE(WindowLostFocus)
			EVENT_CATEGORY(EventCategoryApplication)
	};

	class RenderEvent : public EventBase
	{
	public:
		RenderEvent() {}

		EVENT_TYPE(AppRenderEvent)
			EVENT_CATEGORY(EventCategoryApplication | EventCategoryRenderer)
	};


}


#endif // !WINDOWEVENT_H

