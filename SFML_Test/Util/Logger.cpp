#include "Logger.h"

// initialize static member variables
Logger::category_map_t Logger::category_string = Logger::category_map_t();

Logger::Logger()
: enabled_(true)
{
   this->category_string[INFO] = "INFO";
   this->category_string[ALWAYS] = "ALWAYS";
   this->category_string[WARNING] = "WARNING (!!)";
   this->category_string[ERROR] = "***ERROR***";
}

Logger::~Logger() {
}

void Logger::enable() {
   this->enabled_ = true;
}

void Logger::disable() {
   this->enabled_ = false;
}

std::string Logger::get_category_string(CATEGORY c) {
   return Logger::category_string[c];
}

void Logger::set_tag(std::string tag, bool tag_enable /* = true */) {
   this->tag_map[tag] = tag_enable;
}

void Logger::disable_all_tags() {
   this->set_all_tags(false);
   this->disable();
}

void Logger::enable_all_tags() {
   this->set_all_tags(true);
   this->enable();
}

std::string Logger::get_time() {
   char output_time[25];
   time_t raw_time = time(0);
   struct tm* time_info = localtime(&raw_time);

   strftime(output_time, 25, "%a %b %d %r", time_info);
   return output_time;
}

bool Logger::msg_enabled(std::string tag) {
   tag_map_t::const_iterator it = this->tag_map.find(tag);
   if (it == this->tag_map.end()) {
      // add unknown tags to tag map and set enabled to logger enable state
      this->set_tag(tag, this->enabled_);
      return this->enabled_;
   }

   return it->second && this->enabled_;
}

void Logger::set_all_tags(bool enabled) {
   tag_map_t::iterator it;
   for (it = this->tag_map.begin(); it != this->tag_map.end(); it++) {
      it->second = enabled;
   }
}
