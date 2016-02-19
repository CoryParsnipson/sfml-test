#ifndef LAYER_H
#define LAYER_H

#include "dependencies.h"

#include "MouseUtil.h"

class Graphics;
class Draw;

class Layer
: public MouseControllable
{
public:
   static const float ZOOM_FACTOR_MIN;
   static const float ZOOM_FACTOR_MAX;
   
   typedef std::vector<Draw*> DrawableList;

   Layer(std::string id, sf::Vector2f size);
   virtual ~Layer();

   std::string id();
   std::string to_string();

   sf::View& get_view();

   // view interface
   void set_size(sf::Vector2f size);
   void set_center(sf::Vector2f center);
   void set_viewport(const sf::FloatRect& viewport);

   sf::Vector2f get_size();
   sf::Vector2f get_center();
   sf::FloatRect get_viewport();
   sf::Transform get_transform();
   
   void reset_pan();
   void reset_zoom();

   sf::Vector2f get_pan_delta();

   // draw management interface
   void add(Draw* drawable);
   void remove(Draw* drawable);

   virtual void draw(Graphics& graphics);

   // mouse control interface
   virtual void drag(MouseButtonCommand& c, sf::Vector2f delta);

   virtual float get_scale();
   virtual void set_scale(float factor);

   virtual void click(MouseButtonCommand& c);

protected:
   std::string id_;
   float zoom_factor_;
   sf::Vector2f original_center_;

   sf::View* view_;

   DrawableList drawables_;
};

#endif
