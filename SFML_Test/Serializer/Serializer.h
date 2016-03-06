#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "dependencies.h"

// forward declarations
class Entity;

class Serializer {
public:
   typedef std::map<std::string, std::string> SerializedObj;

   Serializer() {};
   virtual ~Serializer() { this->close(); };

   virtual void open(std::string filename) {
      this->file_.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);
   };

   virtual bool is_open() {
      return this->file_.is_open();
   };

   virtual void close() {
      this->file_.close();
   };

   virtual SerializedObj get() = 0;
   virtual bool next() = 0;
   virtual bool prev() = 0;

   virtual void set(Entity* entity) = 0;

protected:
   std::fstream file_;
};

#endif
