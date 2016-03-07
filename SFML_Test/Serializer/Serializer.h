#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "dependencies.h"

class Grid;
class Entity;

class Serializer {
public:
   typedef std::map<std::string, std::string> SerializedObj;

   Serializer(Game& game) : game_(game) {};
   virtual ~Serializer() { this->close(); };

   virtual void open(std::string filename) {
      this->file_.open(filename, std::fstream::in | std::fstream::out);
   };

   virtual bool is_open() {
      return this->file_.is_open();
   };

   virtual void close() {
      this->file_.close();
   };

   virtual SerializedObj get() = 0;
   virtual void set(Entity* entity) = 0;

   virtual bool next() = 0;
   virtual bool prev() = 0;

   virtual SerializedObj serialize(Entity& entity) = 0;
   virtual SerializedObj serialize(Grid& grid) = 0;
   //virtual SerializedObj serialize(Layer& layer) = 0;

   virtual void deserialize(Serializer::SerializedObj& obj, Entity*& entity) = 0;
   virtual void deserialize(Serializer::SerializedObj& obj, Grid*& grid) = 0;

protected:
   Game& game_;
   std::fstream file_;
};

#endif
