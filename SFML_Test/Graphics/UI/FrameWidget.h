#ifndef FRAME_WIDGET_H
#define FRAME_WIDGET_H

#include "Widget.h"
#include "Graphic.h"

class FrameWidget
: public Widget
{
public:
   typedef std::map<std::string, Graphic*> BorderList;

   FrameWidget(const sf::Vector2f& size, Widget* parent = nullptr);
   virtual ~FrameWidget();

   // draw interface
   virtual void draw(Graphics& graphics);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

protected:
   BorderList border_;
   Graphic* box_;
};

#endif
