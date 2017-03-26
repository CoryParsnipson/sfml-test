#ifndef CIRCLE_H
#define CIRCLE_H

#include "Graphic.h"

// ----------------------------------------------------------------------------
// Circle 
//
// This is a primitive Graphic class that allows you to create circles. 
// You can specify the fill color, outline, and other properties.
// ----------------------------------------------------------------------------
class Circle : public Graphic2 {
public:
   Circle();
   Circle(const std::string& id, float radius = 10, float x = 0, float y = 0);
   Circle(const std::string& id, float radius = 10, sf::Vector2f pos = sf::Vector2f(0, 0));
   virtual ~Circle();

   virtual void init();
   virtual void reset();

   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   using Graphic2::position;
   using Graphic2::move;
   using Graphic2::scale;
   using Graphic2::origin;

   virtual sf::FloatRect local_bounds() const;
   virtual sf::FloatRect global_bounds() const;

   virtual void position(float x, float y);
   virtual const sf::Vector2f& position() const;

   virtual void move(float x, float y);

   virtual void rotation(float angle);
   virtual float rotation() const;

   virtual void scale(float x, float y);
   virtual const sf::Vector2f& scale() const;

   virtual void origin(float x, float y);
   virtual const sf::Vector2f& origin() const;

   virtual void color(const sf::Color& color); // gets and sets *fill* color for shapes
   virtual const sf::Color& color() const;

   virtual const sf::Transform& transform() const;

   // graphic visitor interface
   virtual void accept(GraphicVisitor& visitor);

   // circle shape interface
   void radius(float radius);
   float radius() const;

   void outline_color(const sf::Color& color);
   const sf::Color& outline_color() const;

   void outline_thickness(float thickness);
   float outline_thickness() const;

private:
   sf::CircleShape* drawable_;
};

#endif


