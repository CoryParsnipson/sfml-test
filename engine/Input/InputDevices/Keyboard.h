#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <string>
#include <vector>

#include "KeyPressInputEvent.h"
#include "KeyReleaseInputEvent.h"
#include "InputManager.h"

// ----------------------------------------------------------------------------
// Keyboard
//
// This is an input device to handle keyboard events.
// ----------------------------------------------------------------------------
class Keyboard : public InputDevice {
public:
   Keyboard(const std::string& name = "Keyboard") : InputDevice(name) {
      this->add_button("A");
      this->add_button("B");
      this->add_button("C");
      this->add_button("D");
      this->add_button("E");
      this->add_button("F");
      this->add_button("G");
      this->add_button("H");
      this->add_button("I");
      this->add_button("J");
      this->add_button("K");
      this->add_button("L");
      this->add_button("M");
      this->add_button("N");
      this->add_button("O");
      this->add_button("P");
      this->add_button("Q");
      this->add_button("R");
      this->add_button("S");
      this->add_button("T");
      this->add_button("U");
      this->add_button("V");
      this->add_button("W");
      this->add_button("X");
      this->add_button("Y");
      this->add_button("Z");
      this->add_button("Num0");
      this->add_button("Num1");
      this->add_button("Num2");
      this->add_button("Num3");
      this->add_button("Num4");
      this->add_button("Num5");
      this->add_button("Num6");
      this->add_button("Num7");
      this->add_button("Num8");
      this->add_button("Num9");
      this->add_button("Escape");
      this->add_button("LCtrl");
      this->add_button("LShift");
      this->add_button("LAlt");
      this->add_button("LSystem");
      this->add_button("RCtrl");
      this->add_button("RShift");
      this->add_button("RAlt");
      this->add_button("RSystem");
      this->add_button("Menu");
      this->add_button("LBracket");
      this->add_button("RBracket");
      this->add_button("SemiColon");
      this->add_button("Comma");
      this->add_button("Period");
      this->add_button("Quote");
      this->add_button("Slash");
      this->add_button("BackSlash");
      this->add_button("Tilde");
      this->add_button("Equal");
      this->add_button("Dash");
      this->add_button("Space");
      this->add_button("Return");
      this->add_button("Backspace");
      this->add_button("Tab");
      this->add_button("PageUp");
      this->add_button("PageDown");
      this->add_button("End");
      this->add_button("Home");
      this->add_button("Insert");
      this->add_button("Delete");
      this->add_button("Add");
      this->add_button("Subtract");
      this->add_button("Multiply");
      this->add_button("Divide");
      this->add_button("Left");
      this->add_button("Right");
      this->add_button("Up");
      this->add_button("Down");
      this->add_button("Numpad0");
      this->add_button("Numpad1");
      this->add_button("Numpad2");
      this->add_button("Numpad3");
      this->add_button("Numpad4");
      this->add_button("Numpad5");
      this->add_button("Numpad6");
      this->add_button("Numpad7");
      this->add_button("Numpad8");
      this->add_button("Numpad9");
      this->add_button("F1");
      this->add_button("F2");
      this->add_button("F3");
      this->add_button("F4");
      this->add_button("F5");
      this->add_button("F6");
      this->add_button("F7");
      this->add_button("F8");
      this->add_button("F9");
      this->add_button("F10");
      this->add_button("F11");
      this->add_button("F12");
      this->add_button("F13");
      this->add_button("F14");
      this->add_button("F15");
      this->add_button("Pause");
   }

   virtual ~Keyboard() {}

   // input event processing
   virtual void process(KeyPressInputEvent& e) {
      this->get(InputManager::KeyStr[static_cast<int>(e.key)])->position(1);
   }

   virtual void process(KeyReleaseInputEvent& e) {
      this->get(InputManager::KeyStr[static_cast<int>(e.key)])->position(0);
   }
};

#endif
