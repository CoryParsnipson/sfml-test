#ifndef NULL_LOGGER_H
#define NULL_LOGGER_H

#include "Logger.h"

class NullLogger : public Logger {
public:
   NullLogger() {};
   virtual ~NullLogger() {};

   virtual void msg(VERBOSITY v, CATEGORY c, std::string msg) {};
};

#endif
