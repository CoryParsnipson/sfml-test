#include "EntityFilter.h"

EntityFilter::EntityFilter(const std::string& id /* = "EntityFilter" */)
: id_(id)
, allow_all_(false)
{
}

EntityFilter::~EntityFilter() {
   this->all_of_.clear();
   this->one_of_.clear();
   this->none_of_.clear();
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
   if (this->all_of_.empty() && this->one_of_.empty() && this->none_of_.empty()) {
      return this->allow_all();
   }

   for (std::set<Expression*>::const_iterator it = this->all_of_.begin(); it != this->all_of_.end(); ++it) {
      filter_result &= (*it)->filter(e);
   }

   for (std::set<Expression*>::const_iterator it = this->one_of_.begin(); it != this->one_of_.end(); ++it) {
      filter_result &= (*it)->filter(e);
   }

   for (std::set<Expression*>::const_iterator it = this->none_of_.begin(); it != this->none_of_.end(); ++it) {
      filter_result &= (*it)->filter(e);
   }

   return filter_result;
}
