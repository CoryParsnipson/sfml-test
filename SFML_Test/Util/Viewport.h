#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "../dependencies.h"
#include "Util/FontConfig.h"

// forward declarations
class Graphics;

// ----------------------------------------------------------------------------
// This class nicely encapsulates an sf::View object and contains all of the
// necessary state for it as well.
// ----------------------------------------------------------------------------
class Viewport {
public:
   static const float ZOOM_FACTOR_MIN;
   static const float ZOOM_FACTOR_MAX;

   Viewport(Graphics& graphics, sf::Vector2f size);
   ~Viewport();

   void draw(sf::Drawable& d);
   void write(std::string msg, sf::Vector2f pos = sf::Vector2f(0, 0), const FontConfig* config = nullptr);
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

   sf::View& get_view();
   Graphics& get_graphics();

protected:
   Graphics& graphics;

   float zoom_factor;

   sf::Vector2f default_size;
   sf::Vector2f default_center;

   sf::View* view;
};

#endif
