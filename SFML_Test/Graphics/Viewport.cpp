#include "Viewport.h"
#include "Graphics.h"
#include "Layer.h"

Viewport::Viewport(sf::Vector2f size)
: size_(size)
{
   this->add("main"); // add default layer
}

Viewport::~Viewport() {
   LayerList::const_iterator it;
   for (it = this->layers_.begin(); it != this->layers_.end(); ++it) {
      delete (*it);
   }
   this->layers_.clear();
}

void Viewport::set_viewport(const sf::FloatRect& viewport) {
   LayerList::const_iterator it;
   for (it = this->layers_.begin(); it != this->layers_.end(); ++it) {
      (*it)->set_viewport(viewport);
   }
}

sf::FloatRect Viewport::get_viewport() {
   return (*this->layers_.begin())->get_viewport();
}

Viewport::LayerList Viewport::layers() {
   return this->layers_;
}

Layer* Viewport::add(std::string id) {
   Layer* l = new Layer(id, this->size_);
   this->layers_.push_back(l);

   return l;
}

Layer* Viewport::get(std::string id) {
   LayerList::const_iterator it;
   for (it = this->layers_.begin(); it != this->layers_.end(); ++it) {
      if((*it)->id().compare(id) == 0) {
         return *it;
      }
   }

   return nullptr;
}

void Viewport::insert(std::string id, int idx) {
   if (idx < 0) {
      idx = 0;
   }

   LayerList::const_iterator it = this->layers_.begin() + idx;
   this->layers_.insert(it, new Layer(id, this->size_));
}

void Viewport::move(std::string id, int idx) {
   throw std::runtime_error("Implement me!");
}

void Viewport::remove(std::string id) {
   LayerList::const_iterator it;
   for (it = this->layers_.begin(); it != this->layers_.end(); ++it) {
      if((*it)->id().compare(id) == 0) {
         delete *it;

         this->layers_.erase(it);
         return;
      }
   }
}

void Viewport::resize(sf::Vector2f size) {
   LayerList::const_iterator it;
   for (it = this->layers_.begin(); it != this->layers_.end(); ++it) {
      (*it)->set_size(size);
   }
}

void Viewport::recenter(sf::Vector2f center) {
   LayerList::const_iterator it;
   for (it = this->layers_.begin(); it != this->layers_.end(); ++it) {
      (*it)->set_center(center);
   }
}

void Viewport::reset() {
   LayerList::const_iterator it;
   for (it = this->layers_.begin(); it != this->layers_.end(); ++it) {
      // reset zoom and pan for all viewports
      (*it)->reset_pan();
      (*it)->reset_zoom();
   } 
}

void Viewport::draw(Graphics& graphics) {
   LayerList::const_iterator it;
   for (it = this->layers_.begin(); it != this->layers_.end(); ++it) {
      (*it)->draw(graphics);
   }
}
