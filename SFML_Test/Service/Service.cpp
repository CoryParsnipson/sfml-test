#include "Service.h"

NullLogger Service::null_logger_ = NullLogger();
NullInput Service::null_input_ = NullInput();

// initialize static services
Logger* Service::logger_ = &Service::null_logger_;
Input* Service::input_ = &Service::null_input_;
