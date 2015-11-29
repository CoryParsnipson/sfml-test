#ifndef SERVICE_H
#define SERVICE_H

#include "NullLogger.h"

// forward declarations
class Logger;

class Service {
public:
   // need to call this at the beginning of your program
   static void init() {
      logger_ = &null_logger_;
   }

   static Logger& get_logger() { return *logger_; }

   static void provide_logger(Logger* logger) {
      // revert to null service
      if (logger == nullptr) {
         logger = &null_logger_;
      }

      logger_ = logger;
   }
   
private:
   static Logger* logger_;
   static NullLogger null_logger_;
};

#endif
