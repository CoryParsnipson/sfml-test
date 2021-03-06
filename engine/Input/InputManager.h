#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <string>
#include <vector>
#include <map>

#include <SFML/Window.hpp>

#include "Observer.h"
#include "InputDevice.h"
#include "Updateable.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Game;

// ----------------------------------------------------------------------------
// enum classes
// ----------------------------------------------------------------------------
enum class MouseButton {
   Left,
   Right,
   Middle,
   XButton1,
   XButton2
};

enum class ButtonState {
   Pressed,
   Released
};

enum class Key {
   A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
   Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
   Escape, LCtrl, LShift, LAlt, LSystem, RCtrl, RShift, RAlt, RSystem, Menu,
   LBracket, RBracket, SemiColon, Comma, Period, Quote, Slash, BackSlash, Tilde, Equal, Dash,
   Space, Return, Backspace, Tab,
   PageUp, PageDown, End, Home, Insert, Delete, Add, Subtract, Multiply, Divide,
   Left, Right, Up, Down,
   Numpad0, Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
   F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15,
   Pause
};

// ----------------------------------------------------------------------------
// InputManager
// 
// This class hooks into the SFML input events and converts them to InputEvent
// objects and then broadcasts them to all attached InputEvent.
// ----------------------------------------------------------------------------
class InputManager
: public Subject<InputEventPtr>
, public Updateable
{
public:
   using DeviceMap = std::map<InputDevice::DeviceId, InputDevice*>;

   static std::vector<std::string> KeyStr;

   InputManager(std::string id = "InputManager");
   ~InputManager();

   virtual void poll_event(Game& game);
   virtual void update(Game& game);

   InputDevice::DeviceId add_device(InputDevice* device);
   InputDevice* get_device(InputDevice::DeviceId device_id);
   DeviceMap& list_devices();

private:
   DeviceMap devices_;
};

#endif
