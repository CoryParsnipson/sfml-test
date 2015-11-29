#include "Service.h"

// initialize static services
Logger* Service::logger_ = nullptr;
NullLogger Service::null_logger_ = NullLogger();
