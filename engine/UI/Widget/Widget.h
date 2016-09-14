#ifndef WIDGET_H
#define WIDGET_H

#include <string>
#include <iostream>

#include "sfml.h"
#include "UI.h"
#include "SceneObject.h"

// -----------------------------------------------------------------------------
// Widget class
//
// This is the base class for all UI elements.
// -----------------------------------------------------------------------------
class Widget
: public SceneObject
{
public:
   Widget(std::string id = "Widget");
   virtual ~Widget();

   // widget interface
   void set_positioning(UI::Positioning position);
   UI::Positioning get_positioning();

   virtual const sf::Vector2f& get_position() = 0;
   virtual void set_position(const sf::Vector2f& pos) = 0;

   void move(const sf::Vector2f& delta);

   virtual sf::Vector2f get_size() = 0;
   virtual void set_size(const sf::Vector2f& size) = 0;

   operator std::string() const;

   // scene graph interface
   virtual bool intersects(const sf::Vector2i& other) = 0;
   virtual bool intersects(const sf::Vector2f& other) = 0;
   virtual bool intersects(const sf::FloatRect& other) = 0;
   virtual bool intersects(const SceneObject& other) = 0;

   // scene graph visitor interface
   virtual void accept(SceneGraphVisitor& visitor);

private:
   friend std::ostream& operator<<(std::ostream& stream, const Widget& widget);
   std::string to_string() const;

protected:
   std::string id_;
   UI::Positioning positioning_;
};

#endif
