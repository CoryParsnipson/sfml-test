#ifndef CLICKABLE_H
#define CLICKABLE_H

#include <SFML/System/Vector2.hpp>

#include "Component.h"

// ----------------------------------------------------------------------------
// Clickable
//
// Make this component respond to mouse click and release events via the
// CallbackSystem.
// ----------------------------------------------------------------------------
class Clickable : public Component {
public:
   explicit Clickable(const std::string& id = "Clickable Component", bool is_enabled = true);
   Clickable(const Clickable& other);
   virtual ~Clickable();

   Clickable& operator=(const Clickable& other);
   void swap(Clickable& other);

   bool is_left_clicked() const;
   void is_left_clicked(bool is_clicked);

   bool is_right_clicked() const;
   void is_right_clicked(bool is_clicked);

   bool is_enabled() const;
   void is_enabled(bool is_enabled);

   const sf::Vector2f& left_click_pos() const;
   void left_click_pos(sf::Vector2f pos);

   const sf::Vector2f& right_click_pos() const;
   void right_click_pos(sf::Vector2f pos);

private:
   bool is_left_clicked_;
   bool is_right_clicked_;

   bool is_enabled_;

   sf::Vector2f left_click_pos_;
   sf::Vector2f right_click_pos_;
};

#endif
