#ifndef TEXT_H
#define TEXT_H

#include "Graphic.h"

// ----------------------------------------------------------------------------
// Text
//
// This is a primitive Graphic class that allows you to create renderable
// text.
// ----------------------------------------------------------------------------
class Text : public Graphic2 {
public:
   Text();
   Text(const std::string& id, const std::string& text, sf::Font* font = nullptr, unsigned int size = 12);
   virtual ~Text();

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

   virtual void color(const sf::Color& color);
   virtual const sf::Color& color() const;

   virtual const sf::Transform& transform() const;

   // graphic visitor interface
   virtual void accept(GraphicVisitor& visitor);

   // text interface
   void string(const std::string& s);
   const std::string& string() const;

   void font(sf::Font* font);
   const sf::Font* font() const;

   void font_size(unsigned int size);
   unsigned int font_size() const;

   void style(sf::Text::Style style);
   sf::Text::Style style() const;

   unsigned int em_width() const;

private:
   std::string string_;
   sf::Text* drawable_;

   sf::Vector2f position_;
};

#endif
