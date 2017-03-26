#ifndef COMPONENT_H
#define COMPONENT_H

class Component {
public:
   Component(std::string id = "Component")
   : id_(id)
   {
   }

   virtual ~Component() {}

   const std::string& id() const {
      return this->id_;
   }

   virtual void init() = 0;
   virtual void reset() = 0;

protected:
   void id(std::string id) {
      this->id_ = id;
   }

private:
   std::string id_;
};

#endif
