#ifndef SERVICE_H
#define SERVICE_H

#include "NullLogger.h"
#include "NullBroadcaster.h"
#include "NullInputController.h"

class Service {
public:
   // need to call this at the beginning of your program
   static void init() {
      logger_ = &null_logger_;
      global_events_ = &null_events_;
      input_ = &null_input_;
   }

   static Logger& get_logger() { return *logger_; }
   static Broadcaster& get_events() { return *global_events_; }
   static InputController& get_input() { return *input_; }

   static void provide_logger(Logger* logger) {
      // revert to null service
      if (logger == nullptr) {
         logger = &null_logger_;
      }
      logger_ = logger;
   }

   static void provide_events(Broadcaster* events) {
      // revert to null service
      if (events == nullptr) {
         events = &null_events_;
      }
      global_events_ = events;
   }

   static void provide_input(InputController* input) {
      // revert to null service
      if (input == nullptr) {
         input = &null_input_;
      }
      input_ = input;
   }

private:
   static NullLogger null_logger_;
   static NullBroadcaster null_events_;
   static NullInputController null_input_;

   static Logger* logger_;
   static Broadcaster* global_events_;
   static InputController* input_;
};

#endif
