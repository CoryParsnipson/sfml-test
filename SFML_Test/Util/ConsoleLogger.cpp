#include "ConsoleLogger.h"

ConsoleLogger::ConsoleLogger()
: multiplicity(1)
{
   this->msg("Logger", ALWAYS, "Console logging enabled ==========\n");
}

ConsoleLogger::~ConsoleLogger() {
   this->msg("Logger", ALWAYS, "Console logging disabled ==========\n");
}

void ConsoleLogger::msg(std::string tag, CATEGORY c, std::string msg) {
   if (!this->msg_enabled(tag) && c != ALWAYS) {
      return;
   }

   if (msg == this->last_line_) {
      this->multiplicity++;

      std::cout << "\r";
   } else {
      this->multiplicity = 1;
      this->last_line_ = msg;
      
      std::cout << "\n";
   }
   
   std::cout << "[" << this->get_time() << "] (" << this->get_category_string(c) << ") [" << tag << "] " << msg;

   if (this->multiplicity > 1) {
      std::cout << " (x" << std::to_string(this->multiplicity) << ")";
      std::cout << std::flush;
   }
}
