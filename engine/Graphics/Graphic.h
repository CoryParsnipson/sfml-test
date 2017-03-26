#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <string>

#include "sfml.h"

#include "Draw.h"
#include "Component.h"
#include "GraphicVisitor.h"

// ----------------------------------------------------------------------------
// Graphic
//
// Base class for drawable assets. This also happens to be a type of component
// for use in the Entity Component System framework.
// ----------------------------------------------------------------------------
class Graphic2
: public Draw
, public Component
{
public:
   Graphic2(std::string id) : Component(id) {}
   virtual ~Graphic2() {}

   virtual sf::FloatRect local_bounds() const = 0;
   virtual sf::FloatRect global_bounds() const = 0;

   virtual void position(float x, float y) = 0;
   virtual void position(const sf::Vector2f& pos) { this->position(pos.x, pos.y); }
   virtual const sf::Vector2f& position() const = 0;

   virtual void move(float x, float y) = 0;
   virtual void move(const sf::Vector2f& offset) { this->move(offset.x, offset.y); }

   virtual void rotation(float angle) = 0;
   virtual float rotation() const = 0;

   virtual void scale(float x, float y) = 0;
   virtual void scale(const sf::Vector2f& scale) { this->scale(scale.x, scale.y); }
   virtual const sf::Vector2f& scale() const = 0;

   virtual void origin(float x, float y) = 0;
   virtual void origin(const sf::Vector2f& factor) { this->origin(factor.x, factor.y); }
   virtual const sf::Vector2f& origin() const = 0;

   virtual void color(const sf::Color& color) = 0;
   virtual const sf::Color& color() const = 0;

   virtual const sf::Transform& transform() const = 0;

   // graphic visitor interface
   virtual void accept(GraphicVisitor& visitor) = 0;
};

#endif
