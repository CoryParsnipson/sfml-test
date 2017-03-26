#ifndef COMPOSITE_GRAPHIC_H
#define COMPOSITE_GRAPHIC_H

#include "Update.h"
#include "Graphic.h"
#include "GraphicVisitor.h"

// ----------------------------------------------------------------------------
// CompositeGraphic
//
// This class lets you compose graphics into tree structures to include
// complex graphics made out of smaller primitive type graphics into a graphic
// object that behaves as a single item.
// ----------------------------------------------------------------------------
class CompositeGraphic
: public Graphic2
, public GraphicVisitor
, public Update
{
public:
   using GraphicList = std::vector<Graphic2*>;

   CompositeGraphic(const std::string& id);
   virtual ~CompositeGraphic();

   virtual void init();
   virtual void reset();

   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   virtual void update(Game& game, Scene* scene = nullptr);

   using Graphic2::position;
   using Graphic2::move;
   using Graphic2::scale;
   using Graphic2::origin;

   virtual sf::FloatRect local_bounds() const;
   virtual sf::FloatRect global_bounds() const;

   virtual void position(float x, float y);
   virtual const sf::Vector2f& position() const;

   virtual void move(float x, float y);

   virtual void rotation(float angle);
   virtual float rotation() const;

   virtual void scale(float x, float y);
   virtual const sf::Vector2f& scale() const;

   virtual void origin(float x, float y);
   virtual const sf::Vector2f& origin() const;

   virtual void color(const sf::Color& color);
   virtual const sf::Color& color() const;

   virtual const sf::Transform& transform() const;

   // graphic visitor interface
   virtual void accept(GraphicVisitor& visitor);

   // composite manipulation interface
   Graphic2* child(int idx) const;
   int num_children() const;
  
   virtual void add(Graphic2* child);

   virtual void insert(int idx, Graphic2* child);

   virtual void remove(Graphic2* child);
   virtual void remove(int idx);

private:
   // graphic visitor interface
   virtual void visit(VertexList* graphic);
   virtual void visit(Sprite* graphic);
   virtual void visit(Text* graphic);
   virtual void visit(CompositeGraphic* graphic);
   virtual void visit(Rectangle* graphic);
   virtual void visit(Circle* graphic);

   sf::FloatRect bounding_union(sf::FloatRect bounds1, sf::FloatRect bounds2);

   sf::RectangleShape placeholder_;
   GraphicList children_;
   
   // needed to do update() inside visitor functions (note: don't delete these pointers in the destructor)
   Game* game_;
   Scene* scene_;
};

#endif
