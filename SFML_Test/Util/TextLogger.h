#ifndef TEXT_LOGGER_H
#define TEXT_LOGGER_H

#include "dependencies.h"
#include "Logger.h"
#include <fstream>

class TextLogger : public Logger {
public:
   TextLogger(std::string filename);
   virtual ~TextLogger();

   virtual void msg(VERBOSITY v, CATEGORY c, std::string msg);

private:
   std::string filename;
   std::ofstream log;
};

#endif
