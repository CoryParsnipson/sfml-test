#ifndef ORTHOGRAPHIC_GRID_H
#define ORTHOGRAPHIC_GRID_H

#include <list>
#include <string>

#include "sfml.h"
#include "Grid.h"

class Graphic;

class OrthographicGrid
: public Grid
{
public:
   typedef std::list<Graphic*> GridlineList;
   typedef std::list<Graphic*> TextMarkerList;

   OrthographicGrid(const std::string& id);
   OrthographicGrid(const std::string& id, int tile_size);
   OrthographicGrid(const std::string& id, const sf::Vector2f& tile_size);
   virtual ~OrthographicGrid();

   virtual void origin(const sf::Vector2f& origin);
   virtual const sf::Vector2f& origin();

   virtual void tile_width(int width);
   virtual int tile_width();

   virtual void tile_height(int height);
   virtual int tile_height();

   virtual sf::Vector2f coord_to_screen(const sf::Vector2f& coord);

   using Grid::size;
   virtual void size(sf::Vector2f size);

   // moveable interface
   virtual void move(const sf::Vector2f& delta);

   virtual void set_scale(float factor);
   virtual void set_position(const sf::Vector2f& pos);

   virtual sf::Vector2f floor(const sf::Vector2f& pos);
   virtual sf::Vector2f round(const sf::Vector2f& pos);
   virtual sf::Vector2f ceil(const sf::Vector2f& pos);

   // serializable interface
   virtual std::string serialize(Serializer& s);
   virtual void deserialize(Serializer& s, Game& g, std::string& d);

   // scene graph interface
   virtual bool intersects(const sf::Vector2i& other);
   virtual bool intersects(const sf::Vector2f& other);
   virtual bool intersects(const sf::FloatRect& other);
   virtual bool intersects(const SceneObject& other);

   // scene graph visitor interface
   virtual void accept(SceneGraphVisitor& visitor);

protected:
   Graphic* origin_dot_;
   GridlineList grid_cols_;
   GridlineList grid_rows_;
   TextMarkerList text_markers_;

   void create_origin_dot();

   void create_gridlines();
   void clear_gridlines();

   void create_text_markers();
   void clear_text_markers();

   // scene graph interface hooks 
   virtual void do_draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);
};

#endif
