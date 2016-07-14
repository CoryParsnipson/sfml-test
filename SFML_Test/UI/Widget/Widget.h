#ifndef WIDGET_H
#define WIDGET_H

#include "dependencies.h"
#include "UI.h"
#include "SceneGraphNode.h"

// -----------------------------------------------------------------------------
// Widget class
//
// This is the base class for all UI elements. This happens to be a child
// class of SceneGraphNode.
// -----------------------------------------------------------------------------
class Widget
: public SceneGraphNode
{
public:
   Widget(std::string id = "Widget");
   virtual ~Widget();

   // scene graph visitor interface
   virtual void accept(SceneGraphVisitor& visitor);

   // widget interface
   void set_positioning(UI::Positioning position);
   UI::Positioning get_positioning();

   virtual const sf::Vector2f& get_position() = 0;
   virtual void set_position(const sf::Vector2f& pos) = 0;

   void move(const sf::Vector2f& delta);

   virtual sf::Vector2f get_size() = 0;
   virtual void set_size(const sf::Vector2f& size) = 0;

   virtual bool intersects(const sf::Vector2f& other) = 0;

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr);

   operator std::string() const;

private:
   friend std::ostream& operator<<(std::ostream& stream, const Widget& widget);
   std::string to_string() const;

protected:
   std::string id_;
   UI::Positioning positioning_;
};

#endif
