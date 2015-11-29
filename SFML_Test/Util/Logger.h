#ifndef LOGGER_H
#define LOGGER_H

#include "dependencies.h"

class Logger {
public:
   enum VERBOSITY {
      NONE,
      LOW,
      MEDIUM,
      HIGH
   };
   
   enum CATEGORY {
      INFO,
      DEBUG,
      WARNING,
      ERROR 
   };

   Logger() {};
   virtual ~Logger() {};

   virtual void msg(VERBOSITY v, CATEGORY c, std::string msg) = 0;
};

#endif
