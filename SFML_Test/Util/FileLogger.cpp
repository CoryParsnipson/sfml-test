#include "FileLogger.h"

FileLogger::FileLogger(std::string filename)
: filename(filename)
{
   this->log.open(filename, std::ofstream::out | std::ofstream::app);
   if (!this->log.is_open()) {
      std::cout << "FileLogger (ERROR): cannot open file " << filename << "..." << std::endl;
   
      // TODO: error handling?
      return;
   } 

   std::cout << "[" << this->get_time() << "] (INFO) [Logger] File logging enabled ==========" << std::endl;
   this->msg("Logger", INFO, "Opening log file =========="); 
}

FileLogger::~FileLogger() {
   std::cout << "[" << this->get_time() << "] (INFO) [Logger] File logging disable ==========" << std::endl;
   this->msg("Logger", INFO, "Closing log file =========="); 

   this->log.flush();
   this->log.close();
}

void FileLogger::msg(std::string tag, CATEGORY c, std::string msg) {
   if (!this->log.is_open()) {
      std::cout << "FileLogger (ERROR): msg failed, log file not opened! (" << msg << ")" << std::endl;
      return;
   }

   if (!this->msg_enabled(tag)) {
      return;
   }

   this->log << "[" << this->get_time() << "] (" << this->get_category_string(c) << ") [" << tag << "] " << msg << '\n';
}
