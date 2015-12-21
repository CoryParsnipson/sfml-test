#ifndef SERVICE_H
#define SERVICE_H

#include "NullLogger.h"
#include "NullInputController.h"

// forward declarations
class Logger;
class InputController;

class Service {
public:
   // need to call this at the beginning of your program
   static void init() {
      logger_ = &null_logger_;
      input_ = &null_input_;
   }

   static Logger& get_logger() { return *logger_; }
   static InputController& get_input() { return *input_; }

   static void provide_logger(Logger* logger) {
      // revert to null service
      if (logger == nullptr) {
         logger = &null_logger_;
      }
      logger_ = logger;
   }

   static void provide_input(InputController* input) {
      // revert to null service
      if (input == nullptr) {
         input = &null_input_;
      }
      input_ = input;
   }
   
private:
   static Logger* logger_;
   static NullLogger null_logger_;

   static InputController* input_;
   static NullInputController null_input_;
};

#endif
