#ifndef FULL_LOGGER_H
#define FULL_LOGGER_H

#include "dependencies.h"
#include "Logger.h"

#include "ConsoleLogger.h"
#include "FileLogger.h"

class FullLogger : public Logger {
public:
   typedef std::map<std::string, std::tuple<bool, Logger*> > logger_map_t;

   FullLogger();
   virtual ~FullLogger();

   void console_start();
   void console_enable();
   void console_disable();

   void file_start(std::string filename);
   void file_enable();
   void file_disable();

   // TODO: void start_graphic_logging();

   virtual void msg(std::string tag, CATEGORY c, std::string msg);

protected:
   logger_map_t loggers_;
};

#endif
