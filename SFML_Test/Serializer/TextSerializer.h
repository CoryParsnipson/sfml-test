#ifndef TEXT_SERIALIZER_H
#define TEXT_SERIALIZER_H

#include "Serializer.h"
#include "Serialize.h"

class TextSerializer : public Serializer {
public:
   TextSerializer(Game& game);
   virtual ~TextSerializer();

   virtual Serialize::SerialObj get();
   virtual void set(const Serialize::SerialObj& obj);
   virtual void set(Serialize& serializable) {
      Serialize::SerialObj obj = serializable.serialize();
      this->set(obj);
   }

   virtual void comment(const std::string& comment);

   virtual bool next();
   virtual bool prev();

protected:
   Serialize::SerialObj data_;

   std::string remove_comments(std::string line);
   Serialize::SerialObj tokenize(std::string line);
};

#endif
