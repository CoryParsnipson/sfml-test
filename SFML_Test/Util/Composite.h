#ifndef COMPOSITE_H
#define COMPOSITE_H

class Composite;

class Composite
{
public:
   using CompositeList = std::vector<Composite*>;

   Composite() {}
   virtual ~Composite() {}

   // composite interface
   virtual void add(Composite* child) {
      if (!child) {
         Service::get_logger().msg("Composite", Logger::WARNING, "Adding null child to composite.");
         return;
      }
      this->children_->push_back(child);
   }

   virtual Composite* get(int idx) {
      if (idx < 0 || idx >= this->get_num_children()) {
         return nullptr;
      }

      return this->children_[idx];
   };

   virtual void remove(Composite* child) {
      CompositeList::iterator it;
      for (it = this->children_.begin(); it != this->children_.end(); ++it) {
         if (*it == child) {
            delete *it;
            this->children_.erase(it);
            return;
         }
      }
   }

   virtual void remove(int idx) {
      if (idx < 0 || idx >= this->get_num_children()) {
         return;
      }
      this->children_.erase(this->children_.begin() + idx);
   }

   int get_num_children() { return this->children_->size(); };

protected:
   CompositeList children_;
};

class Leaf : public Composite {
public:
   // composite interface
   virtual void add(Composite* child) {}
   virtual Composite* get(int idx) { return nullptr; }
   virtual void remove(Composite* child) {}
   virtual void remove(int idx) {}
};
#endif
