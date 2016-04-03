#ifndef TEXT_WIDGET_H
#define TEXT_WIDGET_H

#include "Widget.h"
#include "Graphic.h"

class TextWidget
: public Widget
{
public:
   TextWidget(const std::string& text);
   virtual ~TextWidget();

   void set_text(const std::string& text);

   // widget interface
   virtual const sf::Vector2f& get_position();
   virtual void move(const sf::Vector2f& delta);

   // draw interface
   virtual void draw(Graphics& graphics);

protected:
   Graphic* text_;
};

#endif
