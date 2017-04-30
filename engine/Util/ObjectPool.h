// -----------------------------------------------------------------------------
// ObjectPool.h
//
// This is (an attempt at) a generic implmementation of an Object Pool in C++.
//
// Cory Parsnipson
// -----------------------------------------------------------------------------
#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <stdexcept>

// -----------------------------------------------------------------------------
// ObjectPool handle
//
// Use this to reference objects from inside the ObjectPool
// -----------------------------------------------------------------------------
class Handle {
public:
   Handle(unsigned int index, unsigned int version) : index_(index), version_(version) {}
   Handle(unsigned int raw) : index_(raw >> 8), version_(raw & 0xFF) {}

   Handle& operator=(const Handle& other) {
      if (&other == this) {
         return *this;
      }

      this->index_ = other.index_;
      this->version_ = other.version_;

      return *this;
   }

   void reset() {
      this->index_ = 0;
      this->version_ = 0;
   }

   unsigned int index() const { return this->index_; }
   unsigned int version() const { return this->version_; }

private:
   unsigned int index_ : 24;
   unsigned int version_ : 8;
};

// -----------------------------------------------------------------------------
// ObjectPool base
//
// To create a pool, derive from this class and specify the object that should
// be pooled.
//
// This implementation requires the pooled object to either be a primitive
// data type or have a default constructor.
// -----------------------------------------------------------------------------
template <typename ObjectType>
class ObjectPool {
public:
   static const unsigned int default_size = 1000;

   ObjectPool(const std::string& id = "ObjectPool", unsigned int size = ObjectPool::default_size);
   ~ObjectPool();

   void reset();

   const std::string& id() const;

   unsigned int size() const;
   unsigned int num_entries_active() const;

   ObjectType* get(Handle& handle);
   bool get(Handle& handle, ObjectType*& out);

   Handle add();
   void remove(const Handle& handle);

   operator std::string() const;

private:
   class Entry {
   public:
      Entry(unsigned int next_free_index = default_size);
      Entry(const Entry& other);
      ~Entry();

      bool active_;
      unsigned int version_;
      unsigned int next_free_index_;
      
      ObjectType data_;
   };

   template <class E>
   friend std::ostream& operator<<(std::ostream& stream, const ObjectPool<E>& pool);

   std::string to_string() const;

   std::string id_;
   unsigned int first_free_index_;
   unsigned int num_entries_active_;

   std::vector<Entry> pool_;
};

// -----------------------------------------------------------------------------
// ObjectPool::Entry implementation
//
// This structure contains metadata (freelist info, etc) for the Object Pool
// items.
// -----------------------------------------------------------------------------
template <typename ObjectType>
ObjectPool<ObjectType>::Entry::Entry(unsigned int next_free_index /* = default_size */)
: active_(false)
, version_(0)
, next_free_index_(next_free_index)
, data_()
{
}

template <typename ObjectType>
ObjectPool<ObjectType>::Entry::Entry(const Entry& other)
: active_(other.active_)
, version_(other.version_)
, next_free_index_(other.next_free_index_)
, data_(other.data_)
{
}

template <typename ObjectType>
ObjectPool<ObjectType>::Entry::~Entry() {
}

// -----------------------------------------------------------------------------
// ObjectPool implementation
// -----------------------------------------------------------------------------
template <typename ObjectType>
ObjectPool<ObjectType>::ObjectPool(const std::string& id /* = "ObjectPool" */, unsigned int size /* = ObjectPool::default_size */)
: id_(id)
, pool_(size)
{
   this->reset();
}

template <typename ObjectType>
ObjectPool<ObjectType>::~ObjectPool() {
}

template <typename ObjectType>
void ObjectPool<ObjectType>::reset() {
   this->first_free_index_ = 0;
   this->num_entries_active_ = 0;

   int next_free_index = 1;
   for (typename std::vector<Entry>::iterator it = this->pool_.begin(); it != this->pool_.end(); ++it) {
      (*it).active_ = false;
      (*it).version_ = 0;
      (*it).next_free_index_ = next_free_index;

      (*it).data_ = ObjectType();

      ++next_free_index;
   }
}

template <typename ObjectType>
const std::string& ObjectPool<ObjectType>::id() const {
   return this->id_;
}

template <typename ObjectType>
unsigned int ObjectPool<ObjectType>::size() const {
   return this->pool_.size();
}

template <typename ObjectType>
unsigned int ObjectPool<ObjectType>::num_entries_active() const {
   return this->num_entries_active_;
}

template <typename ObjectType>
ObjectType* ObjectPool<ObjectType>::get(Handle& handle) {
   ObjectType* out = nullptr;
   if (!this->get(handle, out)) {
      return nullptr;
   }

   return out;
}

template <typename ObjectType>
bool ObjectPool<ObjectType>::get(Handle& handle, ObjectType*& out) {
   try {
      Entry& entry = this->pool_.at(handle.index());
      
      if (handle.version() != entry.version_ || !entry.active_) {
         handle.reset(); // zero out handle since it's invalid
         return false;
      }

      out = &entry.data_;
   } catch (const std::out_of_range& e) {
      handle.reset(); // zero out handle since it's invalid
      return false;
   }

   return true;
}

template <typename ObjectType>
Handle ObjectPool<ObjectType>::add() {
   unsigned int new_index = this->first_free_index_;

   assert(new_index < this->size());
   assert(!this->pool_[new_index].active_);

   this->first_free_index_ = this->pool_[new_index].next_free_index_;

   this->pool_[new_index].active_ = true;
   this->pool_[new_index].version_ += 1;
   this->pool_[new_index].next_free_index_ = 0; // this is set when the entry is removed

   ++this->num_entries_active_;

   // reserve version == 0 for invalid handles only
   if (this->pool_[new_index].version_ == 0) {
      ++this->pool_[new_index].version_;
   }

   // TODO: implement some sort of customizable resize policy
   assert(this->num_entries_active() <= this->size());

   return Handle(new_index, this->pool_[new_index].version_);
}

template <typename ObjectType>
void ObjectPool<ObjectType>::remove(const Handle& handle) {
   try {
      Entry& entry = this->pool_.at(handle.index());

      if (handle.version() != entry.version_ || !entry.active_) {
         return;
      }

      entry.active_ = false;
      entry.next_free_index_ = this->first_free_index_;
      entry.data_ = ObjectType();

      this->first_free_index_ = handle.index();
      --this->num_entries_active_;
   } catch (const std::out_of_range& e) {
      // silently fail
      return;
   }
}

template <typename ObjectType>
ObjectPool<ObjectType>::operator std::string() const {
   return this->to_string();
}

template <typename ObjectType>
std::string ObjectPool<ObjectType>::to_string() const {
   std::string info = "[ObjectPool '" + this->id() + "']\n";

   info += "[first_free_index = " + std::to_string(this->first_free_index_) + ", num_entries_active = " + std::to_string(this->num_entries_active_) + "]\n";

   unsigned int idx = 0;
   for (typename std::vector<Entry>::const_iterator it = this->pool_.begin(); it != this->pool_.end(); ++it) {
      info += "  [Entry " + std::to_string(idx) + " - v." + std::to_string(it->version_);
      info += (it->active_ ? " - active" : " -       ");
      info += " - next_free_index = " + std::to_string(it->next_free_index_) + "]\n";

      ++idx;
   }
   
   return info;
}

// ----------------------------------------------------------------------------
// free function helper methods
// ----------------------------------------------------------------------------
template <typename ObjectType>
std::ostream& operator<<(std::ostream& stream, const ObjectPool<ObjectType>& pool) {
   stream << pool.to_string();
   return stream;
}

#endif
