#ifndef TEXT_SERIALIZER_H
#define TEXT_SERIALIZER_H

#include "Serializer.h"

// forward declarations
class Entity;

class TextSerializer : public Serializer {
public:
   TextSerializer();
   virtual ~TextSerializer();

   virtual Serializer::SerializedObj get();
   virtual bool next();
   virtual bool prev();

   virtual void set(Entity* entity);

protected:
   std::string line_;
   Serializer::SerializedObj data_;

   std::string remove_comments(std::string line);
   Serializer::SerializedObj tokenize(std::string line);
};

#endif
