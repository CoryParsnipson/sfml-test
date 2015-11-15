#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "../dependencies.h"

// forward declarations
class Game;

// ----------------------------------------------------------------------------
// This class nicely encapsulates an sf::View object and contains all of the
// necessary state for it as well.
// ----------------------------------------------------------------------------
class Viewport {
public:
   static const float ZOOM_FACTOR_MIN;
   static const float ZOOM_FACTOR_MAX;

   Viewport(Game& game, sf::Vector2f size);
   ~Viewport();

   sf::View& get_view();

   void move(sf::Vector2f delta);

   void set_size(sf::Vector2f size);
   sf::Vector2f get_size();
   
   void set_default_size(sf::Vector2f size);
   sf::Vector2f get_default_size();

   void set_center(sf::Vector2f center);
   sf::Vector2f get_center();

   void set_default_center(sf::Vector2f size);
   sf::Vector2f get_default_center();
   
   float get_zoom_factor();
   void set_zoom_factor(float new_zoom_factor);

protected:
   Game& game;

   float zoom_factor;

   sf::Vector2f default_size;
   sf::Vector2f default_center;

   sf::View* view;
};

#endif
