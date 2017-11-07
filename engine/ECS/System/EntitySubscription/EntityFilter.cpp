#include "EntityFilter.h"

EntityFilter::EntityFilter(const std::string& id /* = "EntityFilter" */)
: id_(id)
, allow_all_(false)
{
}

EntityFilter::~EntityFilter() {
   this->filter_expressions_.clear();
}

const std::string& EntityFilter::id() const {
   return this->id_;
}

void EntityFilter::id(const std::string& id) {
   this->id_ = id;
}

bool EntityFilter::filter(Entity& e) const {
   bool filter_result = true;

   // if the filter is "empty", and allow all is set, be permissive (return true no matter what)
   if (this->filter_expressions_.empty()) {
      return this->allow_all();
   }

   for (std::set<Expression*>::const_iterator it = this->filter_expressions_.begin(); it != this->filter_expressions_.end(); ++it) {
      filter_result &= (*it)->filter(e);
   }

   return filter_result;
}
