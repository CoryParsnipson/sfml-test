#include "ComponentManager.h"

void ComponentManager::remove(std::type_index component_type, Handle handle) {
   if (this->components_[component_type]) {
      this->components_[component_type]->remove(handle);
   }
}
