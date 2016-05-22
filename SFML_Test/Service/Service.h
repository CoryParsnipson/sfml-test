#ifndef SERVICE_H
#define SERVICE_H

#include "NullLogger.h"
#include "NullInput.h"

class Service {
public:
   static Logger& get_logger() { return *logger_; }
   static Input& get_input() { return *input_; }

   static void provide_logger(Logger* logger) {
      // revert to null service
      if (logger == nullptr) {
         logger = &null_logger_;
      }
      logger_ = logger;
   }

   static void provide_input(Input* input) {
      // revert to null service
      if (input == nullptr) {
         input = &null_input_;
      }
      input_ = input;
   }

private:
   static NullLogger null_logger_;
   static NullInput null_input_;

   static Logger* logger_;
   static Input* input_;
};

#endif
