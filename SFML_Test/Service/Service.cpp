#include "Service.h"

NullLogger Service::null_logger_ = NullLogger();
NullInputController Service::null_input_ = NullInputController();

// initialize static services
Logger* Service::logger_ = nullptr;
InputController* Service::input_ = nullptr;
