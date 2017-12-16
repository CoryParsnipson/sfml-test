#ifndef INPUT_BINDING_H
#define INPUT_BINDING_H

#include <map>
#include <string>
#include <memory>
#include <typeinfo>
#include <typeindex>

#include "Intent.h"

// ----------------------------------------------------------------------------
// InputBinding
//
// This class is a mapping between Intent class types and Intents. Use this
// to abstract lower level InputDevice elements (KeyA, Up, MouseClick, etc)
// to higher level "intents" (left, right, jump, shoot, etc).
// ----------------------------------------------------------------------------
class InputBinding {
public:
   using IntentMapping = std::map<std::type_index, IntentPtr>;

   InputBinding(const std::string& id = "InputBinding")
   : id_(id)
   {
   }

   ~InputBinding() {
      this->map_.clear();
   }

   std::string id() const;
   void id(const std::string& id);

   template <
      typename IntentT,
      typename std::enable_if<std::is_base_of<Intent, IntentT>::value>::type* = nullptr
   >
   IntentPtr get() {
      IntentMapping::const_iterator it = this->map_.find(std::type_index(typeid(IntentT)));

      if (it != this->map_.end()) {
         assert(it->second != nullptr);
         return it->second;
      }
      return nullptr;
   }

   template <
      typename IntentT,
      typename std::enable_if<std::is_base_of<Intent, IntentT>::value>::type* = nullptr,
      typename... IntentConstructorArgs
   >
   void set(IntentConstructorArgs&&... intent_args) {
      this->map_[std::type_index(typeid(IntentT))] =
         std::make_shared<IntentT>(std::forward<IntentConstructorArgs>(intent_args)...);
   }

private:
   std::string id_;
   IntentMapping map_;
};

#endif
