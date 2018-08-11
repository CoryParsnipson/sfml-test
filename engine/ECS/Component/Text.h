#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <memory>
#include <SFML/Graphics.hpp>

#include "Font.h"
#include "Component.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class RenderSurface;

// ----------------------------------------------------------------------------
// Text
//
// This is a primitive Graphic class that allows you to create renderable
// text.
// ----------------------------------------------------------------------------
class Text
: public Component
{
public:
   explicit Text(const std::string& id = "Text", const std::string& text = "", std::shared_ptr<Font> font = nullptr, unsigned int size = 12);
   Text(const Text& other);
   virtual ~Text();

   Text& operator=(const Text& other);
   void swap(Text& other);   

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

   virtual void color(const sf::Color& color);
   virtual const sf::Color& color() const;

   virtual const sf::Transform& transform() const;

   // text interface
   void string(const std::string& s);
   const std::string& string() const;

   void font(std::shared_ptr<Font> font);
   std::shared_ptr<Font> font() const;

   void font_size(unsigned int size);
   unsigned int font_size() const;

   void style(sf::Text::Style style);
   sf::Text::Style style() const;

   unsigned int em_width() const;

   // serialize interface
   virtual std::string serialize(Serializer& s);
   virtual void deserialize(Serializer& s, Scene& scene, std::string& d);

private:
   std::string string_;
   std::shared_ptr<Font> font_;
   sf::Text drawable_;

   sf::Vector2f offset_;
};

#endif
