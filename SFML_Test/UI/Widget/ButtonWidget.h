#ifndef BUTTON_WIDGET_H
#define BUTTON_WIDGET_H

#include "Widget.h"
#include "Command.h"
#include "MouseControllable.h"

class ButtonWidget
: public Widget
, public MouseControllable
{
public:
   ButtonWidget(std::string id, const sf::Vector2f& pos, const sf::Vector2f& size, Command* action = nullptr, Widget* parent = nullptr);
   ~ButtonWidget();

   void set_background(Graphic* background);

   // button widget interface
   void action(Command* action);
   Command* action();

   // widget interface
   virtual const sf::Vector2f& get_position();
   virtual void set_position(const sf::Vector2f& pos);

   virtual sf::Vector2f get_size();
   virtual void set_size(const sf::Vector2f& size);

   virtual bool intersects(const sf::Vector2f& other);

   // widget event interface
   virtual void on_click();

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   // mouse control interface
   virtual void drag(MouseButton button, sf::Vector2f pos, sf::Vector2f delta);

   virtual float get_scale();
   virtual void set_scale(float factor);

   virtual void click(MouseButton button, ButtonState state, sf::Vector2f pos);

protected:
   Graphic* background_;
   Command* action_;
};

#endif
