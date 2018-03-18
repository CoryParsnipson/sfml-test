#include "Game.h"
#include "InputManager.h"
#include "InputEvent.h"
#include "CloseInputEvent.h"
#include "LostFocusInputEvent.h"
#include "GainedFocusInputEvent.h"
#include "MouseEnteredInputEvent.h"
#include "MouseLeftInputEvent.h"
#include "ResizeInputEvent.h"
#include "KeyPressInputEvent.h"
#include "KeyReleaseInputEvent.h"
#include "MouseMoveInputEvent.h"
#include "MouseWheelInputEvent.h"
#include "MouseButtonInputEvent.h"

// ----------------------------------------------------------------------------
// static member initialization
// ----------------------------------------------------------------------------
std::vector<std::string> InputManager::KeyStr = {
   "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
   "Num0", "Num1", "Num2", "Num3", "Num4", "Num5", "Num6", "Num7", "Num8", "Num9",
   "Escape", "LCtrl", "LShift", "LAlt", "LSystem", "RCtrl", "RShift", "RAlt", "RSystem", "Menu",
   "LBracket", "RBracket", "SemiColon", "Comma", "Period", "Quote", "Slash", "BackSlash", "Tilde", "Equal", "Dash",
   "Space", "Return", "Backspace", "Tab",
   "PageUp", "PageDown", "End", "Home", "Insert", "Delete", "Add", "Subtract", "Multiply", "Divide",
   "Left", "Right", "Up", "Down",
   "Numpad0", "Numpad1", "Numpad2", "Numpad3", "Numpad4", "Numpad5", "Numpad6", "Numpad7", "Numpad8", "Numpad9",
   "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15",
   "Pause"
};

// ----------------------------------------------------------------------------
// InputManager class constructors
// ----------------------------------------------------------------------------
InputManager::InputManager(std::string id /* = "InputManager" */)
: Subject<InputEvent>(id)
{
}

InputManager::~InputManager() {
   for (InputManager::DeviceMap::const_iterator it = this->devices_.begin(); it != this->devices_.end(); ++it) {
      delete it->second;
   }
   this->devices_.clear();
}

// ----------------------------------------------------------------------------
// InputManager class method implementations 
// ----------------------------------------------------------------------------
void InputManager::poll_event(Game& game) {
   sf::Event sfml_event;
   InputEvent* event = nullptr;

   while (game.poll_event(sfml_event)) {
      switch (sfml_event.type) {
      case sf::Event::Closed:
         Game::logger().msg("InputManager", Logger::INFO, "Received close event.");
         event = new CloseInputEvent();
         break;
      case sf::Event::LostFocus:
         Game::logger().msg("InputManager", Logger::INFO, "Received LostFocus event.");
         event = new LostFocusInputEvent();
         break;
      case sf::Event::GainedFocus:
         Game::logger().msg("InputManager", Logger::INFO, "Received GainedFocus event.");
         event = new GainedFocusInputEvent();
         break;
      case sf::Event::MouseEntered:
         Game::logger().msg("InputManager", Logger::INFO, "Received MouseEntered event.");
         event = new MouseEnteredInputEvent();
         break;
      case sf::Event::MouseLeft:
         Game::logger().msg("InputManager", Logger::INFO, "Received MouseLeft event.");
         event = new MouseLeftInputEvent();
         break;
      case sf::Event::Resized:
         Game::logger().msg("InputManager", Logger::INFO, "Received resize event.");
         event = new ResizeInputEvent(sfml_event.size.width, sfml_event.size.height);
         break;
      case sf::Event::KeyPressed:
         Game::logger().msg("InputManager", Logger::INFO, "Received key press event.");

         // if keycode is unknown, drop event
         if (sfml_event.key.code == sf::Keyboard::Key::Unknown) {
            Game::logger().msg("InputManager", Logger::WARNING, "Dropping key press event with e.key code = -1 (unknown)");
            continue;
         }

         event = new KeyPressInputEvent(
            static_cast<Key>(sfml_event.key.code), // convert from enum sf::Keyboard::Key to our enum class Key
            sfml_event.key.alt, sfml_event.key.control,
            sfml_event.key.shift,
            sfml_event.key.system
         );
         break;
      case sf::Event::KeyReleased:
         Game::logger().msg("InputManager", Logger::INFO, "Received key release event.");

         // if keycode is unknown, drop event
         if (sfml_event.key.code == sf::Keyboard::Key::Unknown) {
            Game::logger().msg("InputManager", Logger::WARNING, "Dropping key release event with e.key code = -1 (unknown)");
            continue;
         }

         event = new KeyReleaseInputEvent(
            static_cast<Key>(sfml_event.key.code), // convert from enum sf::Keyboard::Key to our enum class Key
            sfml_event.key.alt, sfml_event.key.control,
            sfml_event.key.shift,
            sfml_event.key.system
         );
         break;
      case sf::Event::MouseMoved:
         Game::logger().msg("InputManager", Logger::INFO, "Received mouse move event.");
         event = new MouseMoveInputEvent(sfml_event.mouseMove.x, sfml_event.mouseMove.y);
         break;
      case sf::Event::MouseWheelMoved:
         // this event type has been deprecated
         Game::logger().msg("InputManager", Logger::INFO, "Received mouse wheel event.");
         event = new MouseWheelInputEvent(
            sfml_event.mouseWheel.x,
            sfml_event.mouseWheel.y,
            sfml_event.mouseWheel.delta
         );
         break;
      case sf::Event::MouseWheelScrolled:
         // this event is supposed to replace MouseWheelMoved, but delta always returns 0 for some reason
         continue;
      case sf::Event::MouseButtonPressed:
         Game::logger().msg("InputManager", Logger::INFO, "Received mouse button pressed event.");
         event = new MouseButtonInputEvent(
            static_cast<MouseButton>(sfml_event.mouseButton.button),
            ButtonState::Pressed,
            sfml_event.mouseButton.x,
            sfml_event.mouseButton.y
         );
         break;
      case sf::Event::MouseButtonReleased:
         Game::logger().msg("InputManager", Logger::INFO, "Received mouse button released event.");
         event = new MouseButtonInputEvent(
            static_cast<MouseButton>(sfml_event.mouseButton.button),
            ButtonState::Released,
            sfml_event.mouseButton.x,
            sfml_event.mouseButton.y
         );
         break;
      default:
         Game::logger().msg("InputManager", Logger::WARNING, "InputManager received unknown input event. (type = " + std::to_string(sfml_event.type) + ")");
         return;
      }

      this->notify(*event);
   }
}

void InputManager::update(Game& game) {
   for (InputManager::DeviceMap::iterator it = this->devices_.begin(); it != this->devices_.end(); ++it) {
      it->second->update(game);
   }
}

InputDevice::DeviceId InputManager::add_device(InputDevice* device) {
   if (device == nullptr) {
      Game::logger().msg("InputManager", Logger::WARNING, "Received nullptr device, not adding.");
      return 0;
   }

   this->devices_[device->device_id()] = device;
   this->attach(*device);

   return device->device_id();
}

InputDevice* InputManager::get_device(InputDevice::DeviceId device_id) {
   InputManager::DeviceMap::const_iterator it = this->devices_.find(device_id);
   if (it != this->devices_.end()) {
      return it->second;
   }

   return nullptr;
}

InputManager::DeviceMap& InputManager::list_devices() {
   return this->devices_;
}
