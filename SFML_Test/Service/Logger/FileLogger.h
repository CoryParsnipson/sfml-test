#ifndef FILE_LOGGER_H
#define FILE_LOGGER_H

#include "dependencies.h"
#include "Logger.h"
#include <fstream>

class FileLogger : public Logger {
public:
   FileLogger(std::string filename);
   virtual ~FileLogger();

   virtual void msg(std::string tag, CATEGORY c, std::string msg);

private:
   std::string filename;
   std::ofstream log;
};

#endif
