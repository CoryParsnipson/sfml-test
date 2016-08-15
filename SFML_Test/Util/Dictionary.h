#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <iostream>
#include <map>
#include <string>
#include <typeinfo>

#include "dependencies.h"

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

   T* get(Key k) const {
      T* item = nullptr;
      
      this->get_pre(k);

      try {
         item = this->items_.at(k);
         item = this->get_post(k, item);
      } catch (const std::out_of_range& e) {
         return item;
      }
      return item;
   }

   void set(Key k, T* item) {
      // delete pre-existing key if already exists
      delete this->items_[k];
      this->items_[k] = item;
   }

   operator std::string() const {
      return this->to_string();
   }

   std::string id() const {
      return this->id_;
   }

private:
   std::string id_;
   ItemList items_; // ownership (TODO: change to use smart pointers later)

   friend std::ostream& operator<<(std::ostream& stream, const Dictionary& dict) {
      stream << dict.to_string();
      return stream;
   }

protected:
   virtual std::string to_string() const {
     return "[Dictionary: " + this->id() + "<" + typeid(Key).name() + ", " + typeid(T).name() + ">]";
   }

   // hooks for inheriting classes
   void get_pre(Key k) const {}
   T* get_post(Key k, T* item) const { return item; }

   typename ItemList::const_iterator iterator() const {
      return typename ItemList::const_iterator();
   }

   typename ItemList::iterator iterator() {
      return typename ItemList::iterator();
   }

   typename ItemList::iterator begin() {
      return this->items_.begin();
   }
   
   typename ItemList::const_iterator begin() const {
      return this->items_.begin();
   }

   typename ItemList::iterator end() {
      return this->items_.end();
   }

   typename ItemList::const_iterator end() const {
      return this->items_.end();
   }
};

#endif
