#ifndef ITERATOR_H
#define ITERATOR_H

template <class T>
class Iterator {
public:
   Iterator(T& iterable) : iterable_(&iterable) {}
   virtual ~Iterator() {}

   virtual void begin() = 0;
   virtual void end() = 0;

   Iterator<T>& operator++() = 0; // prefix
   Iterator<T>& operator++(int) = 0; // postfix

   void set_iterable(T& iterable) {
      this->iterable_ = &iterable;

      this->reset();
   }

   // operator --
   void reset() = 0;

protected:
   T* iterable_;
};

#endif
