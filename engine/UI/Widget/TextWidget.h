#ifndef TEXT_WIDGET_H
#define TEXT_WIDGET_H

#include "Widget.h"

class TextWidget
: public Widget
{
public:
   TextWidget(std::string id, const std::string& text, sf::Vector2f pos = sf::Vector2f(0, 0), sf::Font* font = nullptr);
   virtual ~TextWidget();

   void set_text(const std::string& text);

   // widget interface
   virtual const sf::Vector2f& get_position();
   virtual void set_position(const sf::Vector2f& pos);

   virtual sf::Vector2f get_size();
   virtual void set_size(const sf::Vector2f& size);

   virtual bool intersects(const sf::Vector2i& other);
   virtual bool intersects(const sf::Vector2f& other);
   virtual bool intersects(const sf::FloatRect& other);
   virtual bool intersects(const SceneObject& other);

   // serializable interface
   virtual std::string serialize(Serializer& s);
   virtual void deserialize(Serializer& s, Game& g, std::string d);

protected:
   std::string raw_text_;

   sf::Vector2f size_;
   Graphic* text_;

   void word_wrap();

   // scene graph interface hooks
   virtual void do_draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);
};

#endif
