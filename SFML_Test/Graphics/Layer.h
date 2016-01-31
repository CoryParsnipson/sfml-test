#ifndef LAYER_H
#define LAYER_H

#include "dependencies.h"

#include "Draw.h"
#include "MouseUtil.h"

class Layer
: public Draw
, public MouseControllable
{
public:
   static const float ZOOM_FACTOR_MIN;
   static const float ZOOM_FACTOR_MAX;

   typedef std::list<Draw*> DrawableList;

   Layer(sf::Vector2f size);
   virtual ~Layer();

   void set_size(sf::Vector2f size);
   void set_center(sf::Vector2f center);

   sf::Vector2f get_size();
   sf::Vector2f get_center();

   // draw interface
   virtual void draw(Graphics& graphics, sf::View* view = nullptr);

   // mouse control interface
   virtual void drag(MouseButtonCommand& c, sf::Vector2f delta);

   virtual float get_scale();
   virtual void set_scale(float factor);

   virtual void click(MouseButtonCommand& c);

protected:
   float zoom_factor_;

   sf::View* view_;
   DrawableList drawables_;
};

#endif
