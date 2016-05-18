#ifndef WIDGET_H
#define WIDGET_H

#include "dependencies.h"
#include "Draw.h"
#include "Update.h"
#include "Composite.h"

class Widget
: public Draw
, public Update
, public Composite<Widget>
{
public:
   enum Position {
      ABSOLUTE,
      RELATIVE,
      STATIC
   };

   Widget();
   virtual ~Widget();

   // widget interface
   void set_positioning(Widget::Position position);
   Widget::Position get_positioning();

   virtual const sf::Vector2f& get_position() = 0;
   virtual void set_position(const sf::Vector2f& pos);
   virtual void move(const sf::Vector2f& delta);

   virtual sf::Vector2f get_size() = 0;
   virtual void set_size(const sf::Vector2f& size) = 0;

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

protected:
   Position position_;
};

#endif
