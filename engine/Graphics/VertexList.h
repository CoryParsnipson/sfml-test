#ifndef VERTEX_LIST_H
#define VERTEX_LIST_H

#include "Graphic.h"

// ----------------------------------------------------------------------------
// VertexList
//
// This is another type of primitive Graphic class that lets you create a
// renderable graphic by specifying a list of vertices.
// ----------------------------------------------------------------------------
class VertexList : public Graphic2 {
public:
   VertexList(const std::string& id, sf::PrimitiveType primitive = sf::TrianglesStrip, unsigned int size = 0);
   virtual ~VertexList();

   virtual void init();
   virtual void reset();

   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

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

   // vertex manipulation interface
   void resize(unsigned int idx);
   
   void vertex(unsigned int idx, sf::Vertex vert);
   sf::Vertex& vertex(unsigned int idx);
   const sf::Vertex& vertex(unsigned int idx) const;

   void vertex_color(unsigned int idx, const sf::Color& color);
   const sf::Color& vertex_color(unsigned int idx) const;

   void vertex_position(unsigned int idx, const sf::Vector2f& pos);
   const sf::Vector2f& vertex_position(unsigned int idx) const;

   void vertex_texture_coord(unsigned int idx, const sf::Vector2f& texture_coord);
   const sf::Vector2f& vertex_texture_coord(unsigned int idx) const;

private:
   float rotation_;
   sf::Vector2f size_;
   sf::Vector2f position_;
   sf::Vector2f scale_;
   sf::RenderStates render_states_;

   sf::VertexArray* drawable_;
};

#endif
