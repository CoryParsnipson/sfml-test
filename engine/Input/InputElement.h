#ifndef INPUT_ELEMENT_H
#define INPUT_ELEMENT_H

#include <string>
#include <climits>
#include <memory>

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class InputElement;
class Axis;
class Button;

// ----------------------------------------------------------------------------
// typedefs
// ----------------------------------------------------------------------------
typedef std::shared_ptr<InputElement> InputElementPtr;
typedef std::shared_ptr<Axis> AxisPtr;
typedef std::shared_ptr<Button> ButtonPtr;

// ----------------------------------------------------------------------------
// InputElement
//
// This defines properties for buttons and joystick axes.
// ----------------------------------------------------------------------------
class InputElement {
public:
   InputElement(const std::string& name) : name_(name), position_(0) {}
   virtual ~InputElement() {}

   const std::string& name() const { return this->name_; }
   void name(const std::string& name) { this->name_ = name; }

   bool is_pressed() const { return this->position_ != 0; }

   int position() const { return this->position_; }
   virtual void position(int position) = 0; 

   virtual std::string to_string() const = 0;

protected:
   void set_position(int position) { this->position_ = position; }

private:
   std::string name_;
   int position_;
};

// ----------------------------------------------------------------------------
// Axis
//
// This defines an input element for an analog element on a joystick. This
// input element will usually be able to have a position between INT_MIN and
// INT_MAX.
// ----------------------------------------------------------------------------
class Axis : public InputElement {
public:
   Axis(const std::string& name) : InputElement(name) {}
   virtual ~Axis() {}

   using InputElement::position;
   virtual void position(int position) {
      this->set_position(position);
   }

   virtual std::string to_string() const {
      return "[Axis - \"" + this->name() + "\" : pos = " + std::to_string(this->position()) + "]";
   }
};

// ----------------------------------------------------------------------------
// Button
//
// This defines an input element for a digital (button) element on a joystick,
// mouse, or keyboard. This input element will usually assume the value 0 or
// INT_MAX.
// ----------------------------------------------------------------------------
class Button : public InputElement {
public:
   Button(const std::string& name) : InputElement(name) {}
   virtual ~Button() {}
   
   using InputElement::position;
   virtual void position(int position) {
      this->set_position(position != 0 ? INT_MAX : 0);
   }

   virtual std::string to_string() const {
      return "[Button - \"" + this->name() + "\" : " + (this->is_pressed() ? "PRESSED" : "- ") + "]";
   }
};

#endif
