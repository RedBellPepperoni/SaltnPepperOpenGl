#ifndef KEYBOARDEVENT_H
#define KEYBOARDEVENT_H

#include "Engine/Core/System/Events/Event.h"
#include "Engine/Core/System/Input/InputKeyCodes.h"


namespace SaltnPepperEngine
{

	// ====================== Base KeyEvent ======================
	// The down and up events are based on this
	class KeyEvent : public EventBase
	{

	public:
		inline Input::Key GetKeyCode() const { return keyCode; }
		EVENT_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:

		KeyEvent(const Input::Key keyCode) : keyCode(keyCode) {}

		Input::Key keyCode;

	};


	// ========================= Key Down Event =================
	// Whenever a Key a key is pressed down this event will get triggered

	class KeyDownEvent : public KeyEvent
	{
	public:
		KeyDownEvent(Input::Key keyCode, int repeatCount) : KeyEvent(keyCode), repeatCount(repeatCount) {}

		inline int GetRepeatCount() { return repeatCount; }

		EVENT_TYPE(KeyDown)

	private:

		int repeatCount;
	};

	// =================== Key Up Event ========================
	// Whenever a pressed key is released this evert will trigger

	class KeyUpEvent : public KeyEvent
	{
	public:
		KeyUpEvent(Input::Key keyCode) : KeyEvent(keyCode) {}

		EVENT_TYPE(KeyUp)
	};

	// ============= Char Typed ======================
	// Whenever a key is pressed and released This event is triggered

	class CharTypedEvent : public KeyEvent
	{
		CharTypedEvent(Input::Key keyCode, char typedCharacter) : KeyEvent(keyCode), character(typedCharacter) {}

		EVENT_TYPE(CharTyped)

			char character;
	};


}

#endif // !1
