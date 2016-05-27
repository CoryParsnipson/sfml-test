#include "FullLogger.h"

#include "ConsoleLogger.h"
#include "FileLogger.h"

FullLogger::FullLogger() {
}

FullLogger::~FullLogger() {
   logger_map_t::const_iterator it;

   // delete logger pointers
   for (it = this->loggers_.begin(); it != this->loggers_.end(); it++) {
      delete it->second;
   }
}

void FullLogger::console_start() {
   logger_map_t::iterator it = this->loggers_.find("console");
   if (it != this->loggers_.end()) {
      delete it->second;
   }

   this->loggers_["console"] = new ConsoleLogger();
}

void FullLogger::file_start(std::string filename) {
   logger_map_t::iterator it = this->loggers_.find("file");
   if (it != this->loggers_.end()) {
      delete it->second;
   }

   this->loggers_["file"] = new FileLogger(filename);
}

Logger* FullLogger::get_logger(std::string logger_id) {
   logger_map_t::const_iterator it = this->loggers_.find(logger_id);
   if (it == this->loggers_.end()) {
      return nullptr;
   }
   return it->second;
}

void FullLogger::msg(std::string tag, CATEGORY c, std::string msg) {
   logger_map_t::const_iterator it;
   
   for (it = this->loggers_.begin(); it != this->loggers_.end(); it++) {
      it->second->msg(tag, c, msg);
   }
}
