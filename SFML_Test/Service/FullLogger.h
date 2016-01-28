#ifndef FULL_LOGGER_H
#define FULL_LOGGER_H

#include "Logger.h"

class FullLogger : public Logger {
public:
   typedef std::map<std::string, Logger*> logger_map_t;

   FullLogger();
   virtual ~FullLogger();

   void console_start();
   void file_start(std::string filename);
   // TODO: void start_graphic_logging();

   Logger* get_logger(std::string logger_id);

   virtual void msg(std::string tag, CATEGORY c, std::string msg);

protected:
   logger_map_t loggers_;
};

#endif
