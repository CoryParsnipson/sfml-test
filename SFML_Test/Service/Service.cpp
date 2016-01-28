#include "Service.h"

NullLogger Service::null_logger_ = NullLogger();
NullBroadcaster Service::null_events_ = NullBroadcaster();
NullInputController Service::null_input_ = NullInputController();

// initialize static services
Logger* Service::logger_ = nullptr;
Broadcaster* Service::global_events_ = nullptr;
InputController* Service::input_ = nullptr;
