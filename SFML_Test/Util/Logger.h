#ifndef LOGGER_H
#define LOGGER_H

#include "dependencies.h"
#include <time.h>

class Logger {
public:
   enum VERBOSITY {
      NONE,
      LOW,
      MEDIUM,
      HIGH
   };
   
   enum CATEGORY {
      INFO,
      DEBUG,
      WARNING,
      ERROR 
   };

   Logger()
   : verbosity_level(HIGH)
   {
      this->category_string[INFO] = "INFO";
      this->category_string[DEBUG] = "DEBUG";
      this->category_string[WARNING] = "WARNING";
      this->category_string[ERROR] = "***ERROR***";
   };

   virtual ~Logger() {};

   virtual void msg(VERBOSITY v, CATEGORY c, std::string msg) = 0;
   
   // helper methods
   virtual std::string get_time() {
      char output_time[25];
      time_t raw_time = time(0);
      struct tm* time_info = localtime(&raw_time);

      strftime(output_time, 25, "%a %b %d %r", time_info);
      return output_time;
   }

   virtual std::string get_category_string(CATEGORY c) {
      return this->category_string[c];
   }

   virtual void set_verbosity(VERBOSITY v) {
      this->verbosity_level = v;
   }

   virtual bool check_verbosity(VERBOSITY v) {
      // for messages with verbosity NONE, always print
      return (v == NONE) || (v >= this->verbosity_level);
   }

protected:
   VERBOSITY verbosity_level;
   std::map<CATEGORY, std::string> category_string;
};

#endif
