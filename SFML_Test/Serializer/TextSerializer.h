#ifndef TEXT_SERIALIZER_H
#define TEXT_SERIALIZER_H

#include "Serializer.h"

// forward declarations
class Entity;

class TextSerializer : public Serializer {
public:
   TextSerializer(Game& game);
   virtual ~TextSerializer();

   virtual Serializer::SerializedObj get();
   virtual void set(Entity* entity);

   virtual bool next();
   virtual bool prev();

   virtual Serializer::SerializedObj serialize(Entity& entity);
   virtual Serializer::SerializedObj serialize(Grid& grid);
   //virtual SerializedObj serialize(Layer& layer);

   virtual void deserialize(Serializer::SerializedObj& obj, Entity*& entity);
   virtual void deserialize(Serializer::SerializedObj& obj, Grid*& grid);

protected:
   std::string line_;
   Serializer::SerializedObj data_;

   void comment(std::string comment);
   std::string remove_comments(std::string line);
   Serializer::SerializedObj tokenize(std::string line);
};

#endif
