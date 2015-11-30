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

   std::cout << "File logging enabled ============================================";
   this->msg(NONE, INFO, "Opening log file ================================================");
}

TextLogger::~TextLogger() {
   std::cout << "File logging disabled ===========================================";
   this->msg(NONE, INFO, "Closing log file ================================================");

   this->log.flush();
   this->log.close();
}

void TextLogger::msg(VERBOSITY v, CATEGORY c, std::string msg) {
   if (!this->log.is_open()) {
      std::cout << "TextLogger (ERROR): msg failed, log file not opened! (" << msg << ")" << std::endl;
      return;
   }

   if (!this->check_verbosity(v)) {
      return;
   }

   this->log << "[" << this->get_time() << "] (" << this->get_category_string(c) << ") " << msg << '\n';
}
