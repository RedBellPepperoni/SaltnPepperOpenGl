#ifndef EVENT_H
#define EVENT_H
#include <functional>


namespace SaltnPepperEngine
{

	using namespace std;



	enum class EventType
	{
		NONE = 0,

		// Window Events
		WindowMouseEnter,
		WindowMouseExit,
		WindowClose,
		WindowResize,
		WindowFocus,
		WindowLostFocus,

		// Mouse Keyboard Events
		CharTyped,
		KeyDown,
		KeyUp,
		MouseMoved,
		MouseButtonDown,
		MouseButtonUp,
		MouseScrolled,

		// Controller/Joystick Events
		GamepadConnect,
		GamePadDisconnect,


		// Render Event
		AppRenderEvent,
		AppUpdate,
		AppStart,
		AppExit

	};


	enum EventCategory
	{
		EventCategoryApplication = 1 << 0,
		EventCategoryInput = 1 << 1,
		EventCategoryKeyboard = 1 << 2,
		EventCategoryMouse = 1 << 3,
		EventCategoryMouseButton = 1 << 4,
		EventCategoryJoystick = 1 << 5,
		EventCategoryTouchscreen = 1 << 6,
		EventCategoryRenderer = 1 << 7
	};



	class EventBase
	{

	public:

		virtual ~EventBase() = default;



		virtual const char* GetEventName() const = 0;
		virtual EventType GetEventType() const = 0;
		virtual int GetCategoryFlags() const = 0;
		inline bool GetHandled() { return ishandled; }
		inline void SetHandled(bool handled) { ishandled = handled; }

		bool isInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}

	protected:
		bool ishandled = false;
	};

	typedef std::function<void(EventBase&)> EventHandlerFn;

	class EventDispatcher {
	public:

		EventDispatcher(EventBase& event) : m_event(event)
		{
		}


		template<typename T, typename F>
		bool dispatch(const F& func)
		{
			if (m_event.GetEventType() == T::GetStaticType())
			{
				m_event.SetHandled(func(static_cast<T&>(m_event)));
				return true;
			}
			return false;
		}

	protected:
		EventBase& m_event;
	};


	// Reference from Opengl bible for events and various stack overflow posts
	// and a little bit from Arcane Engine
	// https://github.com/Ershany/Arcane-Engine/blob/master/Arcane/src/Arcane/Core/Events/Event.h

#define BIND_FN(fn) [this](auto&&... args) -> decltype(auto) {  return fn(forward<decltype(args)>(args)...); }


#define BIND_GLOBAL_FN(fn) [](auto&&... args) -> decltype(auto) { return fn(forward<decltype(args)>(args)...); }


#define EVENT_TYPE(type)                                                      \
  static EventType GetStaticType() { return EventType::type; }                \
  virtual EventType GetEventType() const override { return GetStaticType(); } \
  virtual const char* GetEventName() const override { return #type; }

#define EVENT_CATEGORY(category)											  \
  virtual int GetCategoryFlags() const override { return category; }




}



#endif