#ifndef PANEL_WIDGET_H
#define PANEL_WIDGET_H

#include "Widget.h"
#include "SubCanvas.h"

class PanelWidget
: public Widget
{
public:
   const float MIN_PANEL_WIDTH = 90.f;
   const float MIN_PANEL_HEIGHT = 90.f;

   PanelWidget(std::string id, const sf::Vector2f& pos, const sf::Vector2f& size);
   virtual ~PanelWidget();

   // widget interface
   virtual const sf::Vector2f& get_position();
   virtual void set_position(const sf::Vector2f& pos);

   virtual sf::Vector2f get_size();
   virtual void set_size(const sf::Vector2f& size);

   virtual bool intersects(const sf::Vector2f& other);

   // widget event interface
   virtual void on_mouse_in();
   virtual void on_mouse_out();

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

protected:
   Graphic* panel_;
   SubCanvas surface_; // intermediate drawing surface to clip contents
};

#endif
