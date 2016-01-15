#include "Service.h"

// initialize static services
Logger* Service::logger_ = nullptr;
NullLogger Service::null_logger_ = NullLogger();

InputController* Service::input_ = nullptr;
NullInputController Service::null_input_ = NullInputController();

EventQueue* Service::event_queue_ = nullptr;
NullEventQueue Service::null_event_queue_ = NullEventQueue();
