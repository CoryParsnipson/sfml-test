#include "Focus.h"

Focus::Focus(const std::string& id /* = "Focus Component" */)
: Component(id)
{
}

Focus::Focus(const Focus& other)
: Component(other.id())
{
}

Focus::~Focus() {
}

Focus& Focus::operator=(const Focus& other) {
   Focus tmp(other);
   this->swap(tmp);
   return *this;
}

void Focus::swap(Focus& other) {
   std::swap(static_cast<Component&>(*this), static_cast<Component&>(other));
}
