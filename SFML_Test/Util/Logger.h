#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <map>
#include <time.h>

class Logger {
public:
   enum CATEGORY {
      INFO,
      WARNING,
      ERROR 
   };

   typedef std::map<CATEGORY, std::string> category_map_t;
   typedef std::map<std::string, bool> tag_map_t;

   Logger();
   virtual ~Logger();

   // public interface
   virtual void msg(std::string tag, CATEGORY c, std::string msg) = 0;

   // static helper methods
   static std::string get_category_string(CATEGORY c);
   static void set_tag(std::string tag, bool tag_enable = true);
   
   // helper methods
   virtual std::string get_time();
   virtual bool msg_enabled(std::string tag);

protected:
   static tag_map_t tag_map;
   static category_map_t category_string;
};

#endif
