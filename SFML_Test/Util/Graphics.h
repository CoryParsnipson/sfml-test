#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "dependencies.h"

#include "GraphicsWriter.h"
#include "Viewport.h"

// SFML graphics interface
class Graphics {
public:
   Graphics();
   Graphics(sf::RenderWindow& window);

   bool is_open();
   void close();
   
   void update();
   void clear();

   void draw(sf::Drawable& d, sf::View* view = nullptr);

   // given a point on the screen, get the absolute world coordinate (apply global transforms)
   sf::Vector2f get_world_coord(const sf::Vector2i& point, sf::View* view = nullptr);

   // given an absolute world coordinate, get its screen coordinate
   sf::Vector2i get_screen_coord(const sf::Vector2f& point, sf::View* view = nullptr);

   sf::RenderWindow& get_window();

   // graphics writer functions
   void write(std::string msg, sf::Vector2f pos = sf::Vector2f(0, 0), const FontConfig* config = nullptr, sf::View* view = nullptr);
   void load_font(std::string font_name, std::string filename);
   
protected:
   sf::RenderWindow* window;
   GraphicsWriter writer;
};

#endif
