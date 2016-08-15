#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <iostream>
#include <map>
#include <string>

// ----------------------------------------------------------------------------
// Dictionary
//
// This is basically a thin-wrapper on top of an std::map.
// ----------------------------------------------------------------------------
template <typename Key, class T>
class Dictionary {
public:
   using ItemList = std::map<Key, T*>;

   Dictionary(std::string id = "Dictionary")
   : id_(std::move(id))
   {
   }

   virtual ~Dictionary() {
      for (typename ItemList::const_iterator it = this->items_.begin(); it != this->items_.end(); ++it) {
         delete it->second;
      }
      this->items_.clear();
   }

   T* get(Key k) {
      return this->items_[k];
   }

   void set(Key k, T* item) {
      // delete pre-existing key if already exists
      delete this->items_[k];
      this->items_[k] = item;
   }

   operator std::string() const {
      return this->to_string();
   }

   std::string id() {
      return this->id_;
   }

private:
   std::string id_;
   ItemList items_; // ownership (change to use smart pointers later)

   friend std::ostream& operator<<(std::ostream& stream, const Dictionary& dict) {
      stream << dict.to_string();
      return stream;
   }

   std::string to_string() const {
     return "[Dictionary: " + this->id() + "<" + Key::name() + ", " + T::name() + ">]";
   }
};

#endif
