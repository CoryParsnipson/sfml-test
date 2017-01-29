#ifndef BUTTON_WIDGET_H
#define BUTTON_WIDGET_H

#include "Widget.h"
#include "Command.h"

class ButtonWidget
: public Widget
{
public:
   ButtonWidget(std::string id, const sf::Vector2f& pos, const sf::Vector2f& size, Command* action = nullptr, Widget* parent = nullptr);
   ~ButtonWidget();

   void set_background(Graphic* background);

   // widget interface
   virtual const sf::Vector2f& get_position();
   virtual void set_position(const sf::Vector2f& pos);

   virtual sf::Vector2f get_size();
   virtual void set_size(const sf::Vector2f& size);

   virtual bool intersects(const sf::Vector2i& other);
   virtual bool intersects(const sf::Vector2f& other);
   virtual bool intersects(const sf::FloatRect& other);
   virtual bool intersects(const SceneObject& other);

   // mouse interaction interface 
   virtual void on_mouse_in();
   virtual void on_mouse_out();

   // serializable interface
   virtual std::string serialize(Serializer& s);
   virtual void deserialize(Serializer& s, Scene& scene, std::string& d);

protected:
   Graphic* background_;

   // scene graph interface hooks
   virtual void do_draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);
};

#endif
