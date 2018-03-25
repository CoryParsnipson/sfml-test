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
#include <functional>

// -----------------------------------------------------------------------------
// ObjectPool handle
//
// Use this to reference objects from inside the ObjectPool
// -----------------------------------------------------------------------------
class Handle {
public:
   Handle() : index_(0), version_(0) {}
   Handle(unsigned int index, unsigned int version) : index_(index), version_(version) {}

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

   bool valid() const {
      return this->version_ != 0;
   }

   unsigned int index() const { return this->index_; }
   unsigned int version() const { return this->version_; }

   bool operator ==(const Handle& other) const {
      return this->index_ == other.index_ && this->version_ == other.version_;
   }
   
   bool operator !=(const Handle& other) const {
      return this->index_ != other.index_ || this->version_ != other.version_;
   }

   std::string str() const {
      return "[Handle - idx " + std::to_string(this->index_) + ", ver " + std::to_string(this->version_) + "]";
   }

private:
   unsigned int index_;
   unsigned int version_;
};

// -----------------------------------------------------------------------------
// Handle class member functions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// ObjectPoolBase
//
// This is a mostly empty base class for ObjectPool for polymorphic storage
// of ObjectPools
// -----------------------------------------------------------------------------
class ObjectPoolBase {
public:
   virtual ~ObjectPoolBase() {
   }
};

// -----------------------------------------------------------------------------
// ObjectPool base
//
// To create a pool, derive from this class and specify the object that should
// be pooled. Use the allocator parameter to specify what kind of default
// object to construct.
// -----------------------------------------------------------------------------
template <typename ObjectType>
class ObjectPool final : public ObjectPoolBase {
public:
   using EntryAllocator = std::function<ObjectType()>;

   static const unsigned int default_size = 1000;

   // TODO: this allocator thing can be done better (rewrite to allow for non-default constructor. Also to allow for object types with private constructors by only having objectpool as friend)
   ObjectPool(const std::string& id = "ObjectPool", unsigned int size = ObjectPool::default_size, const EntryAllocator& allocator = [](){ return ObjectType(); });
   virtual ~ObjectPool();

   void reset();

   const std::string& id() const;

   unsigned int size() const;
   unsigned int num_entries_active() const;

   ObjectType* get(Handle& handle);
   bool get(Handle& handle, ObjectType*& out);

   template <typename... ComponentArgs> Handle add(ComponentArgs&&... args);
   void remove(const Handle& handle);

   std::vector<ObjectType*> get_active_objects() const;
   const std::vector<Handle>& get_active_handles() const;

   void resize(unsigned int size);

   operator std::string() const;

private:
   class Entry {
   public:
      Entry(unsigned int next_free_index = default_size, ObjectType data = ObjectType());
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

   std::vector<Handle> active_handles_;
   std::vector<Entry> pool_;

   EntryAllocator allocator_;
};

// -----------------------------------------------------------------------------
// ObjectPool::Entry implementation
//
// This structure contains metadata (freelist info, etc) for the Object Pool
// items.
// -----------------------------------------------------------------------------
template <typename ObjectType>
ObjectPool<ObjectType>::Entry::Entry(unsigned int next_free_index /* = default_size */, ObjectType data /* = ObjectType() */)
: active_(false)
, version_(0)
, next_free_index_(next_free_index)
, data_(data)
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
ObjectPool<ObjectType>::ObjectPool(const std::string& id /* = "ObjectPool" */, unsigned int size /* = ObjectPool::default_size */, const EntryAllocator& allocator /* = [](){ return ObjectType(); } */)
: ObjectPoolBase()
, id_(id)
, active_handles_()
, pool_(size, Entry(size, allocator()))
, allocator_(allocator)
{
   this->reset();
}

template <typename ObjectType>
ObjectPool<ObjectType>::~ObjectPool() {
}

template <typename ObjectType>
void ObjectPool<ObjectType>::reset() {
   this->first_free_index_ = 0;

   this->active_handles_.clear();

   int next_free_index = 1;
   for (typename std::vector<Entry>::iterator it = this->pool_.begin(); it != this->pool_.end(); ++it) {
      (*it).active_ = false;
      (*it).version_ = 0;
      (*it).next_free_index_ = next_free_index;
      (*it).data_ = this->allocator_();

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
   return this->active_handles_.size();
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
template <typename... ComponentArgs>
Handle ObjectPool<ObjectType>::add(ComponentArgs&&... args) {
   unsigned int new_index = this->first_free_index_;

   assert(new_index < this->size());
   assert(!this->pool_[new_index].active_);

   this->first_free_index_ = this->pool_[new_index].next_free_index_;

   this->pool_[new_index].active_ = true;
   this->pool_[new_index].version_ += 1;
   this->pool_[new_index].next_free_index_ = 0; // this is set when the entry is removed

   // reserve version == 0 for invalid handles only
   if (this->pool_[new_index].version_ == 0) {
      ++this->pool_[new_index].version_;
   }

   // initialize a new object
   this->pool_[new_index].data_.~ObjectType();
   new (&this->pool_[new_index].data_) ObjectType(args...);

   // TODO: implement some sort of customizable resize policy
   assert(this->num_entries_active() <= this->size());

   // add to active handles list
   this->active_handles_.push_back(Handle(new_index, this->pool_[new_index].version_));

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

      this->first_free_index_ = handle.index();

      // remove from active handles list
      for (std::vector<Handle>::const_iterator it = this->active_handles_.begin(); it != this->active_handles_.end(); ++it) {
         if (handle == *it) {
            this->active_handles_.erase(it);
            break;
         }
      }
   } catch (const std::out_of_range& e) {
      // silently fail
      return;
   }
}

template <typename ObjectType>
std::vector<ObjectType*> ObjectPool<ObjectType>::get_active_objects() const {
   std::vector<ObjectType*> active;

   for (std::vector<Handle>::const_iterator it = this->active_handles_.begin(); it != this->active_handles_.end(); ++it) {
      active.push_back(this->get(*it));
   }

   return active;
}

template <typename ObjectType>
const std::vector<Handle>& ObjectPool<ObjectType>::get_active_handles() const {
   return this->active_handles_;
}

template <typename ObjectType>
void ObjectPool<ObjectType>::resize(unsigned int size) {
   // update first free index (if necessary)
   if (size < this->size() && this->first_free_index_ > size) {
      int idx = 0;
      for (typename std::vector<Entry>::const_iterator it = this->pool_.begin(); it != this->pool_.end(); ++it) {
         if (!(*it)->active_) {
            this->first_free_index_ = idx;
            break;
         }
         ++idx;
      }
   }
   
   // update active entries record (if necessary)
   if (size < this->size()) {
      unsigned int idx = size;
      for (typename std::vector<Entry>::const_iterator it = this->pool_.begin() + size; it != this->pool_.end(); ++it) {
         if ((*it)->active_) {
            this->remove(Handle(idx, (*it)->version_));
         }
         ++idx;
      }
   }

   this->pool_.resize(size, this->allocator_());
}

template <typename ObjectType>
ObjectPool<ObjectType>::operator std::string() const {
   return this->to_string();
}

template <typename ObjectType>
std::string ObjectPool<ObjectType>::to_string() const {
   std::string info = "[ObjectPool '" + this->id() + "']\n";

   info += "[first_free_index = " + std::to_string(this->first_free_index_) + ", num_entries_active = " + std::to_string(this->num_entries_active()) + "]\n";

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
