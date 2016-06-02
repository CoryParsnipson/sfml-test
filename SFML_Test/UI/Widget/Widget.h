#ifndef WIDGET_H
#define WIDGET_H

#include "UI.h"
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
   Widget();
   virtual ~Widget();

   // composite interface
   virtual void add_post(Widget* child);
   virtual void remove_post(Widget* child);

   // widget interface
   void set_positioning(UI::Positioning position);
   UI::Positioning get_positioning();

   virtual const sf::Vector2f& get_position() = 0;
   virtual void set_position(const sf::Vector2f& pos) = 0;

   void move(const sf::Vector2f& delta);

   virtual sf::Vector2f get_size() = 0;
   virtual void set_size(const sf::Vector2f& size) = 0;

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

private:
   void set_parent(Widget* parent);
   Widget* get_parent();

protected:
   Widget* parent_;
   UI::Positioning positioning_;
};

#endif
