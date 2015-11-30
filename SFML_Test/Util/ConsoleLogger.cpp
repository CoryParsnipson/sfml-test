#include "ConsoleLogger.h"

ConsoleLogger::ConsoleLogger()
: multiplicity(1)
{
   this->msg(NONE, INFO, "Console logging enabled =========================================");
}

ConsoleLogger::~ConsoleLogger() {
   this->msg(NONE, INFO, "Console logging disabled ========================================\n");
}

void ConsoleLogger::msg(VERBOSITY v, CATEGORY c, std::string msg) {
   if (!this->check_verbosity(v)) {
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
   
   std::cout << "[" << this->get_time() << "] (" << this->get_category_string(c) << ") " << msg;

   if (this->multiplicity > 1) {
      std::cout << " (x" << std::to_string(this->multiplicity) << ")";
   }
}
