#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <string>
#include <SFML/Graphics.hpp>

#include "Color.h"
#include "Component.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class RenderSurface;

// ----------------------------------------------------------------------------
// Rectangle 
//
// This is a primitive Graphic class that allows you to create rectangles
// (and squares). You can specify the fill color, outline, and other
// properties.
// ----------------------------------------------------------------------------
class Rectangle
: public Component
{
public:
   explicit Rectangle(const std::string& id = "Rectangle", float x = 0, float y = 0, float width = 100, float height = 100);
   Rectangle(const std::string& id, sf::FloatRect rect = sf::FloatRect(0, 0, 100, 100));
   Rectangle(const Rectangle& other);
   virtual ~Rectangle();
   
   Rectangle& operator=(const Rectangle& other);
   void swap(Rectangle& other);

   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   virtual sf::FloatRect local_bounds() const;
   virtual sf::FloatRect global_bounds() const;

   virtual void rotation(float angle);
   virtual float rotation() const;

   virtual void scale(float x, float y);
   virtual void scale(const sf::Vector2f& scale) { this->scale(scale.x, scale.y); }
   virtual const sf::Vector2f& scale() const;

   virtual void origin(float x, float y);
   virtual void origin(const sf::Vector2f& factor) { this->origin(factor.x, factor.y); }
   virtual const sf::Vector2f& origin() const;

   virtual void offset(float x, float y);
   virtual void offset(const sf::Vector2f& pos) { this->offset(pos.x, pos.y); }
   virtual const sf::Vector2f& offset() const;

   virtual void move_offset(float x, float y);
   virtual void move_offset(const sf::Vector2f& offset) { this->move_offset(offset.x, offset.y); }

   virtual void color(const sf::Color& color); // gets and sets *fill* color for shapes
   virtual const sf::Color& color() const;

   virtual const sf::Transform& transform() const;

   // rectangle shape interface
   void size(float x, float y);
   void size(const sf::Vector2f& size);
   const sf::Vector2f& size() const;

   void outline_color(const sf::Color& color);
   const sf::Color& outline_color() const;

   void outline_thickness(float thickness);
   float outline_thickness() const;

   // serialize interface
   virtual std::string serialize(Serializer& s);
   virtual void deserialize(Serializer& s, Scene& scene, std::string& d);

private:
   sf::RectangleShape drawable_;
};

#endif

