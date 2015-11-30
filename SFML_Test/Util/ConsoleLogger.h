#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H

#include "dependencies.h"
#include "Logger.h"

class ConsoleLogger : public Logger {
public:
   ConsoleLogger();
   virtual ~ConsoleLogger();

   virtual void msg(VERBOSITY v, CATEGORY c, std::string msg);

protected:
   int multiplicity;
   std::string last_line_;
};

#endif
