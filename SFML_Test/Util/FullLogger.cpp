#include "FullLogger.h"

FullLogger::FullLogger() {
}

FullLogger::~FullLogger() {
   logger_map_t::const_iterator it;

   // delete logger pointers
   for (it = this->loggers_.begin(); it != this->loggers_.end(); it++) {
      delete std::get<1>(it->second);
   }
}

void FullLogger::console_start() {
   logger_map_t::iterator it = this->loggers_.find("console");
   if (it != this->loggers_.end()) {
      delete std::get<1>(it->second);
   }

   this->loggers_["console"] = std::make_tuple(true, new ConsoleLogger());
}

void FullLogger::console_enable() {
   logger_map_t::iterator it = this->loggers_.find("console");

   if (it != this->loggers_.end()) {
      std::get<0>(it->second) = true;
      return;
   }

  std::cout << "FullLogger::console_enable (ERROR): console logger not initialized!" << std::endl;
}

void FullLogger::console_disable() {
   logger_map_t::iterator it = this->loggers_.find("console");

   if (it != this->loggers_.end()) {
      std::get<0>(it->second) = false;
      return;
   }

  std::cout << "FullLogger::console_disable (ERROR): console logger not initialized!" << std::endl;
}

void FullLogger::file_start(std::string filename) {
   logger_map_t::iterator it = this->loggers_.find("file");
   if (it != this->loggers_.end()) {
      delete std::get<1>(it->second);
   }

   this->loggers_["file"] = std::make_tuple(true, new FileLogger(filename));
}

void FullLogger::file_enable() {
   logger_map_t::iterator it = this->loggers_.find("file");

   if (it != this->loggers_.end()) {
      std::get<0>(it->second) = true;
      return;
   }

  std::cout << "FullLogger::file_enable (ERROR): file logger not initialized!" << std::endl;
}

void FullLogger::file_disable() {
   logger_map_t::iterator it = this->loggers_.find("file");

   if (it != this->loggers_.end()) {
      std::get<0>(it->second) = false;
      return;
   }

  std::cout << "FullLogger::file_disable (ERROR): file logger not initialized!" << std::endl;
}

void FullLogger::msg(VERBOSITY v, CATEGORY c, std::string msg) {
   logger_map_t::const_iterator it;
   
   for (it = this->loggers_.begin(); it != this->loggers_.end(); it++) {
      if (!std::get<0>(it->second)) {
         continue;
      }

      std::get<1>(it->second)->msg(v, c, msg);
   }
}
