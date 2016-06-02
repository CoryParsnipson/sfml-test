#ifndef TEXT_WIDGET_H
#define TEXT_WIDGET_H

#include "Widget.h"

class TextWidget
: public Widget
{
public:
   TextWidget(const std::string& text);
   virtual ~TextWidget();

   void set_text(const std::string& text);

   // widget interface
   virtual const sf::Vector2f& get_position();
   virtual void set_position(const sf::Vector2f& pos);

   virtual sf::Vector2f get_size();
   virtual void set_size(const sf::Vector2f& size);

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

protected:
   std::string raw_text_;

   sf::Vector2f size_;
   Graphic* text_;

   void word_wrap();
};

#endif
