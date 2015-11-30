#include "ConsoleLogger.h"

ConsoleLogger::ConsoleLogger()
{
   this->msg(NONE, INFO, "Console logging enabled =========================================");
}

ConsoleLogger::~ConsoleLogger() {
   this->msg(NONE, INFO, "Console logging disabled ========================================");
}

void ConsoleLogger::msg(VERBOSITY v, CATEGORY c, std::string msg) {
   if (!this->check_verbosity(v)) {
      return;
   }

   std::cout << "[" << this->get_time() << "] (" << this->get_category_string(c) << ") " << msg << '\n';
}
