#ifndef TEXT_SERIALIZER_H
#define TEXT_SERIALIZER_H

#include "Serializer.h"

// forward declarations
class Entity;

class TextSerializer : public Serializer {
public:
   TextSerializer();
   virtual ~TextSerializer();

   virtual Serializer::data_t get();
   virtual bool next();
   virtual bool prev();

   virtual void set(Entity* entity);
protected:
   std::string line_;
   data_t data_;

   std::string remove_comments(std::string line);
   Serializer::data_t tokenize(std::string line);
};

#endif
