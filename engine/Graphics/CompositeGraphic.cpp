#include <cassert>

#include "CompositeGraphic.h"
#include "RenderSurface.h"
#include "Game.h"
#include "Scene.h"
#include "VertexList.h"
#include "Sprite.h"
#include "Text.h"
#include "Rectangle.h"
#include "Circle.h"

CompositeGraphic::CompositeGraphic(const std::string& id)
: Graphic2(id)
, game_(nullptr)
{
}

CompositeGraphic::~CompositeGraphic() {
   GraphicList::iterator it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      delete *it;
   }
}

void CompositeGraphic::init() {
   // TODO: implement for pooling
}

void CompositeGraphic::reset() {
   // TODO: implement for pooling
}

void CompositeGraphic::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   GraphicList::iterator it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      (*it)->draw(surface, render_states);
   }
}

void CompositeGraphic::update(Game& game) { 
   this->game_ = &game;

   GraphicList::iterator it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      (*it)->accept(*this);
   }
}

sf::FloatRect CompositeGraphic::local_bounds() const {
   return this->placeholder_.getLocalBounds();
}

sf::FloatRect CompositeGraphic::global_bounds() const {
   return this->placeholder_.getGlobalBounds();
}

void CompositeGraphic::position(float x, float y) {
   this->move(sf::Vector2f(x, y) - this->position());
}

const sf::Vector2f& CompositeGraphic::position() const {
   return this->placeholder_.getPosition();
}

void CompositeGraphic::move(float x, float y) {
   GraphicList::iterator it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      (*it)->move(x, y);
   }

   this->placeholder_.move(x, y);
}

void CompositeGraphic::rotation(float angle) {
   GraphicList::iterator it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      (*it)->rotation(angle);
   }

   this->placeholder_.setRotation(angle);
}

float CompositeGraphic::rotation() const {
   return this->placeholder_.getRotation();
}

void CompositeGraphic::scale(float x, float y) {
   GraphicList::iterator it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      (*it)->scale(x, y);
   }

   this->placeholder_.setScale(x, y);
}

const sf::Vector2f& CompositeGraphic::scale() const {
   return this->placeholder_.getScale();
}

void CompositeGraphic::origin(float x, float y) {
   GraphicList::iterator it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      (*it)->origin(x, y);
   }

   this->placeholder_.setOrigin(x, y);
}

const sf::Vector2f& CompositeGraphic::origin() const {
   return this->placeholder_.getOrigin();
}

void CompositeGraphic::color(const sf::Color& color) {
   GraphicList::iterator it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      (*it)->color(color);
   }

   this->placeholder_.setFillColor(color);
}

const sf::Color& CompositeGraphic::color() const {
   return this->placeholder_.getFillColor();
}

const sf::Transform& CompositeGraphic::transform() const {
   return this->placeholder_.getTransform();
}

void CompositeGraphic::accept(GraphicVisitor& visitor) {
   visitor.visit(this);
}

Graphic2* CompositeGraphic::child(int idx) const {
   if (idx < 0 || idx >= this->num_children()) {
      return nullptr;
   }
   return this->children_[idx];
}

int CompositeGraphic::num_children() const {
   return this->children_.size();
}

void CompositeGraphic::add(Graphic2* child) {
   if (child == nullptr) {
      return;
   }

   // first, update bounding boxes
   sf::FloatRect bounds;

   if (this->num_children() == 0) {
      // if this is the first child, just make the placeholder bounds == child bounds
      bounds = child->global_bounds();
   } else {
      bounds = this->bounding_union(bounds, child->global_bounds());
   }

   // expand placeholder graphic to account for new graphic
   this->placeholder_.setSize(sf::Vector2f(bounds.width, bounds.height));
   this->placeholder_.setPosition(bounds.left, bounds.top);

   // add child to this graphic
   this->children_.push_back(child);
}

void CompositeGraphic::insert(int idx, Graphic2* child) {
   if (child == nullptr) {
      return;
   }

   // first, update bounding boxes
   sf::FloatRect bounds;

   if (this->num_children() == 0) {
      bounds = child->global_bounds();
   } else {
      bounds = this->bounding_union(bounds, child->global_bounds());
   }

   // expand placeholder graphic to account for new graphic
   this->placeholder_.setSize(sf::Vector2f(bounds.width, bounds.height));
   this->placeholder_.setPosition(bounds.left, bounds.top);

   // add child to this graphic
   GraphicList::iterator it;

   if (idx <= 0) {
      it = this->children_.begin();
   } else if (idx >= (signed int)this->children_.size()) {
      it = this->children_.end();
   } else {
      it = this->children_.begin() + idx;
   }
   this->children_.insert(it, child);
}

void CompositeGraphic::remove(Graphic2* child) {
   // remove child from this graphic
   GraphicList::iterator it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      if (*it == child) {
         this->children_.erase(it);
         break;
      }
   }

   // update bounding boxes
   sf::FloatRect bounds;

   if (this->num_children() > 0) {
      bounds = this->child(0)->global_bounds();

      // need to rebuild bounding box with remaining children
      GraphicList::iterator it;
      for (it = this->children_.begin(); it != this->children_.end(); ++it) {
         bounds = this->bounding_union(bounds, (*it)->global_bounds());
      }
   }

   // update placeholder graphic without removed graphic
   this->placeholder_.setSize(sf::Vector2f(bounds.width, bounds.height));
   this->placeholder_.setPosition(bounds.left, bounds.top);
}

void CompositeGraphic::remove(int idx) {
   if (idx < 0 || idx >= this->num_children()) {
      return;
   }

   this->remove(this->child(idx));
}

void CompositeGraphic::visit(VertexList* graphic) {}

void CompositeGraphic::visit(Sprite* graphic) {
   assert(this->game_ != nullptr);   
   graphic->update(*this->game_);
}

void CompositeGraphic::visit(Text* graphic) {}

void CompositeGraphic::visit(CompositeGraphic* graphic) {
   assert(this->game_ != nullptr);   
   graphic->update(*this->game_);
}

void CompositeGraphic::visit(Rectangle* graphic) {}
void CompositeGraphic::visit(Circle* graphic) {}

sf::FloatRect CompositeGraphic::bounding_union(sf::FloatRect bounds1, sf::FloatRect bounds2) {
   sf::FloatRect bounds;

   bounds.top = std::min(bounds1.top, bounds2.top);
   bounds.left = std::min(bounds1.left, bounds2.left);
   
   bounds.width = (bounds1.left + bounds1.width > bounds2.left + bounds2.width) ? bounds1.width : bounds2.width;
   bounds.height = (bounds1.top + bounds1.height > bounds2.top + bounds2.height) ? bounds1.height : bounds2.height;
   
   return bounds;
}
