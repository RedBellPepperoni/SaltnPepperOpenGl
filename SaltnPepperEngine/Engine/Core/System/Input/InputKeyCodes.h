#ifndef INPUTKEYCODES_H
#define INPUTKEYCODES_H

#include <map>
#include "Engine/Utils/GLUtils.h"

namespace SaltnPepperEngine
{
	namespace Input
	{
        enum class Key : unsigned int
        {
            // From glfw3.h
            Space = 32,
            Apostrophe = 39, /* ' */
            Comma = 44, /* , */
            Minus = 45, /* - */
            Period = 46, /* . */
            ForwardSlash = 47, /* / */

            num0 = 48, /* 0 */
            num1 = 49, /* 1 */
            num2 = 50, /* 2 */
            num3 = 51, /* 3 */
            num4 = 52, /* 4 */
            num5 = 53, /* 5 */
            num6 = 54, /* 6 */
            num7 = 55, /* 7 */
            num8 = 56, /* 8 */
            num9 = 57, /* 9 */

            Semicolon = 59, /* ; */
            Equal = 61, /* = */

            A = 65,
            B = 66,
            C = 67,
            D = 68,
            E = 69,
            F = 70,
            G = 71,
            H = 72,
            I = 73,
            J = 74,
            K = 75,
            L = 76,
            M = 77,
            N = 78,
            O = 79,
            P = 80,
            Q = 81,
            R = 82,
            S = 83,
            T = 84,
            U = 85,
            V = 86,
            W = 87,
            X = 88,
            Y = 89,
            Z = 90,

            LeftSqBracket = 91, /* [ */
            Backslash = 92, /* \ */
            RightSqBracket = 93, /* ] */
            GraveAccent = 96, /* ` */

            /* Function keys */
            Escape = 256,
            Enter = 257,
            Tab = 258,
            Backspace = 259,
            Delete = 261,


            Right = 262,
            Left = 263,
            Down = 264,
            Up = 265,



            /* NumPad Keys */
            Numpad0 = 320,
            Numpad1 = 321,
            Numpad2 = 322,
            Numpad3 = 323,
            Numpad4 = 324,
            Numpad5 = 325,
            Numpad6 = 326,
            Numpad7 = 327,
            Numpad8 = 328,
            Numpad9 = 329,
            NumpadPeriod = 330,
            NumpadDivide = 331,
            NumpadMultiply = 332,
            NumpadSubtract = 333,
            NumpadAdd = 334,
            NumpadEnter = 335,
            NumpadEqual = 336,

            LeftShift = 340,
            LeftControl = 341,
            LeftAlt = 342,

            RightShift = 344,
            RightControl = 345,
            RightAlt = 346,

        };

        enum class MouseButton : unsigned int
        {
            Left = 0,
            Middle = 1,
            Right = 2,
            NoCursor = 3,
        };



        namespace OpenGLKeys
        {
            using namespace std;
            using namespace Input;

            static Key GetKeyboardKey(uint32_t openGLKey)
            {
                static map<uint32_t, Key> inputkeyMap =
                {
                    {GLFW_KEY_A, Key::A},
                    {GLFW_KEY_B, Key::B},
                    {GLFW_KEY_C, Key::C},
                    {GLFW_KEY_D, Key::D},
                    {GLFW_KEY_E, Key::E},
                    {GLFW_KEY_F, Key::F},
                    {GLFW_KEY_G, Key::G},
                    {GLFW_KEY_H, Key::H},
                    {GLFW_KEY_I, Key::I},
                    {GLFW_KEY_J, Key::J},
                    {GLFW_KEY_K, Key::K},
                    {GLFW_KEY_L, Key::L},
                    {GLFW_KEY_M, Key::M},
                    {GLFW_KEY_N, Key::N},
                    {GLFW_KEY_O, Key::O},
                    {GLFW_KEY_P, Key::P},
                    {GLFW_KEY_Q, Key::Q},
                    {GLFW_KEY_R, Key::R},
                    {GLFW_KEY_S, Key::S},
                    {GLFW_KEY_T, Key::T},
                    {GLFW_KEY_U, Key::U},
                    {GLFW_KEY_V, Key::V},
                    {GLFW_KEY_W, Key::W},
                    {GLFW_KEY_X, Key::X},
                    {GLFW_KEY_Y, Key::Y},
                    {GLFW_KEY_Z, Key::Z},

                    {GLFW_KEY_0, Key::num0},
                    {GLFW_KEY_1, Key::num1},
                    {GLFW_KEY_2, Key::num2},
                    {GLFW_KEY_3, Key::num3},
                    {GLFW_KEY_4, Key::num4},
                    {GLFW_KEY_5, Key::num5},
                    {GLFW_KEY_6, Key::num6},
                    {GLFW_KEY_7, Key::num7},
                    {GLFW_KEY_8, Key::num8},
                    {GLFW_KEY_9, Key::num9},

                    {GLFW_KEY_KP_0, Key::Numpad0},
                    {GLFW_KEY_KP_1, Key::Numpad1},
                    {GLFW_KEY_KP_2, Key::Numpad2},
                    {GLFW_KEY_KP_3, Key::Numpad3},
                    {GLFW_KEY_KP_4, Key::Numpad4},
                    {GLFW_KEY_KP_5, Key::Numpad5},
                    {GLFW_KEY_KP_6, Key::Numpad6},
                    {GLFW_KEY_KP_7, Key::Numpad7},
                    {GLFW_KEY_KP_8, Key::Numpad8},
                    {GLFW_KEY_KP_9, Key::Numpad9},

                    {GLFW_KEY_KP_ADD, Key::NumpadAdd },
                    {GLFW_KEY_KP_ENTER, Key::NumpadEnter},

                    { GLFW_KEY_UP, Key::Up },
                    { GLFW_KEY_DOWN,Key::Down },
                    { GLFW_KEY_LEFT, Key::Left },
                    { GLFW_KEY_RIGHT, Key::Right },

                    { GLFW_KEY_SPACE, Key::Space },

                    { GLFW_KEY_MINUS, Key::Minus },
                    { GLFW_KEY_DELETE, Key::Delete },

                    { GLFW_KEY_LEFT_SHIFT, Key::LeftShift },
                    { GLFW_KEY_RIGHT_SHIFT, Key::RightShift },
                    { GLFW_KEY_ESCAPE,Key::Escape },

                    { GLFW_KEY_COMMA, Key::Comma },
                    { GLFW_KEY_BACKSPACE, Key::Backspace },
                    { GLFW_KEY_ENTER, Key::Enter },

                    { GLFW_KEY_LEFT_ALT, Key::LeftAlt },
                    { GLFW_KEY_RIGHT_ALT, Key::RightAlt },
                    { GLFW_KEY_LEFT_CONTROL, Key::LeftControl },
                    { GLFW_KEY_RIGHT_CONTROL, Key::RightControl },
                    { GLFW_KEY_TAB, Key::Tab },

                    { GLFW_KEY_GRAVE_ACCENT, Key::GraveAccent}


                };

                return inputkeyMap[openGLKey];
            }

            static MouseButton GetOpenGLMouseButton(uint32_t openGlmousebutton)
            {
                static map<uint32_t, MouseButton> mouseButtonMap =
                {
                    {GLFW_MOUSE_BUTTON_LEFT, MouseButton::Left},
                    {GLFW_MOUSE_BUTTON_RIGHT, MouseButton::Right},
                    {GLFW_MOUSE_BUTTON_MIDDLE, MouseButton::Middle}

                };

                return mouseButtonMap[openGlmousebutton];
            }
        }
	}


	using namespace Input;
}


#endif // !INPUTKEYCODES_H

