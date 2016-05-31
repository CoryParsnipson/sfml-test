#include "Game.h"
#include "Scene.h"
#include "Entity.h"
#include "KeyboardGamepad.h"
#include "Command.h"
#include "InputEvent.h"
#include "CloseInputEvent.h"
#include "ResizeInputEvent.h"
#include "KeyPressInputEvent.h"
#include "MouseMoveInputEvent.h"
#include "MouseWheelInputEvent.h"
#include "MouseButtonInputEvent.h"

KeyboardGamepad::KeyboardGamepad(std::string id /* = "KeyboardGamepad" */)
: PlayerGamepad(id)
{
}

KeyboardGamepad::~KeyboardGamepad() {
   Keybinding::const_iterator it;
   for (it = this->config_.begin(); it != this->config_.end(); ++it) {
      delete it->second;
   }
   this->config_.clear();
}

void KeyboardGamepad::bind(Key code, Command* command) {
   if (this->config_[code]) {
      // delete the old command if it exists
      delete this->config_[code];
      this->config_.erase(code);
   }
   this->config_[code] = command;
}

void KeyboardGamepad::update(Game& game, Scene* scene /* = nullptr */, Entity* entity /* = nullptr */) {
}

void KeyboardGamepad::process(CloseInputEvent& e) {}
void KeyboardGamepad::process(ResizeInputEvent& e) {}
void KeyboardGamepad::process(KeyPressInputEvent& e) {}
void KeyboardGamepad::process(MouseMoveInputEvent& e) {}
void KeyboardGamepad::process(MouseWheelInputEvent& e) {}
void KeyboardGamepad::process(MouseButtonInputEvent& e) {}
