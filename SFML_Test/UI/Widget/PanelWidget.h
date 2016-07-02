#ifndef PANEL_WIDGET_H
#define PANEL_WIDGET_H

#include "Widget.h"
#include "MouseControllable.h"
#include "SubCanvas.h"

class PanelWidget
: public Widget
, public MouseControllable
{
public:
   const float MIN_PANEL_WIDTH = 90.f;
   const float MIN_PANEL_HEIGHT = 90.f;

   PanelWidget(std::string id, const sf::Vector2f& pos, const sf::Vector2f& size, bool draggable = false, bool resizable = false);
   virtual ~PanelWidget();

   // widget interface
   virtual const sf::Vector2f& get_position();
   virtual void set_position(const sf::Vector2f& pos);

   virtual sf::Vector2f get_size();
   virtual void set_size(const sf::Vector2f& size);

   virtual bool intersects(const sf::Vector2f& other);

   // widget event interface
   virtual void on_click();
   virtual void on_release();

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   // mouse control interface
   virtual void drag(MouseButton button, sf::Vector2f pos, sf::Vector2f delta);

   virtual float get_scale();
   virtual void set_scale(float factor);

   virtual void click(MouseButton button, ButtonState state, sf::Vector2f pos);

protected:
   bool clicked_;
   bool resized_;

   bool draggable_;
   bool resizable_;

   Graphic* panel_;
   Graphic* resize_handle_;

   SubCanvas surface_; // intermediate drawing surface to clip contents
};

#endif
