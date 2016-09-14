#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H

#include <string>

#include "sfml.h"
#include "Logger.h"

class ConsoleLogger : public Logger {
public:
   ConsoleLogger();
   virtual ~ConsoleLogger();

   virtual void msg(std::string tag, CATEGORY c, std::string msg);

protected:
   int multiplicity;
   std::string last_tag_;
   std::string last_line_;
};

#endif
