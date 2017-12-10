#ifndef INPUT_H
#define INPUT_H

#include <map>
#include <string>
#include <memory>
#include <typeinfo>
#include <typeindex>

#include "Component.h"
#include "InputDevice.h"
#include "InputMap.h"
#include "Intent.h"

// ----------------------------------------------------------------------------
// Input
//
// This is an input component. It contains a mapping between Intents and
// elements from InputDevices.
// ----------------------------------------------------------------------------
class Input : public Component {
public:
   using IntentMapping = std::map<std::type_index, IntentPtr>;

   Input(const std::string& id = "InputComponent") : Component(id) {}
   Input(const Input& other) : id_(other.id_), map_(other.map_) {}
   virtual ~Input() {}

   Input& operator=(const Input& other) {
      Input tmp(other);
      this->swap(tmp);
      return *this;
   }

   void swap(Input& other) {
      Component::swap();

      std::swap(this->map_, other.map_);
   }

   template <
      typename IntentT,
      typename std::enable_if<std::is_base_of<Intent, MsgT>::value>::type* = nullptr
   >
   InputElementPtr get() {
      IntentMapping::const_iterator it = this->map_.find(std::type_index(typeid(IntentT)));

      if (it != this->map_.end()) {
         assert(it->second != nullptr);
         return it->second->element();
      }
      return intentptr;
   }

   template <
      typename IntentT,
      typename std::enable_if<std::is_base_of<Intent, MsgT>::value>::type* = nullptr,
      typename... IntentConstructorArgs
   >
   void set(IntentConstructorArgs&&... intent_args) {
      this->map_[std::type_index(typeid(IntentT))] =
         std::make_shared<IntentT>(std::forward<IntentConstructorArgs>(intent_args)...);
   }

private:
   IntentMapping map_;
};

#endif
