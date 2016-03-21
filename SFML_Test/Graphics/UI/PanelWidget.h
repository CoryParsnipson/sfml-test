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
   typedef std::map<std::string, Graphic*> BorderList;

   PanelWidget(const sf::Vector2f& pos, const sf::Vector2f& size, Widget* parent = nullptr);
   virtual ~PanelWidget();

   // draw interface
   virtual void draw(Graphics& graphics);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

   // mouse control interface
   virtual void drag(MouseButtonCommand& c, sf::Vector2f delta);

   virtual float get_scale();
   virtual void set_scale(float factor);

   virtual void click(MouseButtonCommand& c);

protected:
   bool clicked_;
   
   BorderList border_;
   Graphic* box_;
};

#endif
