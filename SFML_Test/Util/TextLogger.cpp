#include "TextLogger.h"

TextLogger::TextLogger(std::string filename)
: filename(filename)
{
   this->log.open(filename, std::ofstream::out | std::ofstream::app);
   if (!this->log.is_open()) {
      std::cout << "TextLogger (ERROR): cannot open file " << filename << "..." << std::endl;
   
      // TODO: error handling?
      return;
   } 

   this->msg(NONE, INFO, "Opening log file.");
}

TextLogger::~TextLogger() {
   this->msg(NONE, INFO, "Closing log file.");

   this->log.flush();
   this->log.close();
}

void TextLogger::msg(VERBOSITY v, CATEGORY c, std::string msg) {
   if (!this->log.is_open()) {
      std::cout << "msg failed! (" << msg << ")" << std::endl;
      return;
   }

   this->log << msg << '\n';
}
