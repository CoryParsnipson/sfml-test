#ifndef PANEL_WIDGET_H
#define PANEL_WIDGET_H

#include "Widget.h"
#include "Graphic.h"
#include "MouseUtil.h"

class PanelWidget
: public Widget
, public MouseControllable
{
public:
   const float MIN_PANEL_WIDTH = 90.f;
   const float MIN_PANEL_HEIGHT = 90.f;
   typedef std::map<std::string, Graphic*> BorderList;

   PanelWidget(const sf::Vector2f& pos, const sf::Vector2f& size, Widget* parent = nullptr, bool draggable = false, bool resizable = false);
   virtual ~PanelWidget();

   // widget interface
   virtual const sf::Vector2f& get_position();
   virtual void set_position(const sf::Vector2f& pos);
   virtual void move(const sf::Vector2f& move);

   virtual sf::Vector2f get_size();
   virtual void set_size(const sf::Vector2f& size);

   // draw interface
   virtual void draw(Graphics& graphics);

   // mouse control interface
   virtual void drag(MouseButtonCommand& c, sf::Vector2f delta);

   virtual float get_scale();
   virtual void set_scale(float factor);

   virtual void click(MouseButtonCommand& c);

protected:
   bool clicked_;
   bool resized_;

   bool draggable_;
   bool resizable_;

   Graphic* panel_;
   Graphic* resize_handle_;
};

#endif
