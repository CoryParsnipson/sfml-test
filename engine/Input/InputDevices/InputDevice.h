#ifndef INPUT_DEVICE_H
#define INPUT_DEVICE_H

#include <cassert>
#include <string>
#include <memory>
#include <map>

#include "InputElement.h"
#include "InputListener.h"

// ----------------------------------------------------------------------------
// InputDevice
//
// This is the base class for all i/o interfaces to the game engine.
// ----------------------------------------------------------------------------
class InputDevice : public InputListener {
public:
   using DeviceId = unsigned int;
   using ElementId = std::string;
   using ElementMap = std::map<ElementId, InputElementPtr>;

   InputDevice(const std::string& name)
   : name_(name)
   {
      this->device_id_ = InputDevice::next_device_id++;
   }

   virtual ~InputDevice() {
      this->elements_.clear();
   }

   DeviceId device_id() const { return this->device_id_; }
   void device_id(DeviceId device_id) { this->device_id_ = device_id; }

   const std::string& name() const { return this->name_; }
   void name(const std::string& name) { this->name_ = name; }

   std::string to_string() const {
      std::string info = "[InputDevice - \"" + this->name() + "\"]\n";

      for (ElementMap::const_iterator it = this->elements_.begin(); it != this->elements_.end(); ++it) {
         info += "  " + it->second->to_string() + "\n";
      }
      
      info += "[InputDevice - \"" + this->name() + "\"]";
      return info;
   }

   InputElementPtr get(const ElementId& element_name) {
      InputElementPtr element = this->elements_[element_name];

      assert(element != nullptr);
      assert(element->name() == element_name);
      return element;
   }

   // input event processing
   virtual void process(CloseInputEvent& e) {}
   virtual void process(ResizeInputEvent& e) {}
   virtual void process(KeyPressInputEvent& e) {}
   virtual void process(KeyReleaseInputEvent& e) {}
   virtual void process(MouseMoveInputEvent& e) {}
   virtual void process(MouseWheelInputEvent& e) {}
   virtual void process(MouseButtonInputEvent& e) {}

protected:
   void add_button(const ElementId& name) {
      this->elements_[name] = std::make_shared<Button>(name);
   }

   void add_axis(const ElementId& name) {
      this->elements_[name] = std::make_shared<Axis>(name);
   }

private:
   static DeviceId next_device_id;

   DeviceId device_id_;
   std::string name_;

   ElementMap elements_;
};

#endif
