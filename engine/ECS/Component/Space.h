#ifndef SPACE_H
#define SPACE_H

#include <string>

#include "sfml.h"

#include "Component.h"
#include "ObjectPool.h"

// ----------------------------------------------------------------------------
// Space
// 
// This is a spatial component. Entities need to have one of these components
// to be part of the current Scene's scene graph.
// ----------------------------------------------------------------------------
class Space : public Component {
public:
   explicit Space(const std::string& id = "Space Component");
   Space(const Space& other);
   virtual ~Space();

   Space& operator=(const Space& other);
   void swap(Space& other);

   Handle parent() const;
   void parent(Handle parent);

   Handle add(unsigned int idx) const;
   void add(Handle child, int idx = -1);

   void remove(Handle child);
   void remove(unsigned int idx);

   Handle get(unsigned int idx) const;

   unsigned int num_children() const;

   bool visible() const;
   void visible(bool visible);

   // transform interface
   void position(float x, float y);
   void position(const sf::Vector2f& position);
   const sf::Vector2f& position() const;

   void rotation(float angle);
   float rotation() const;

   void scale(float x, float y);
   void scale(const sf::Vector2f& factors);
   const sf::Vector2f& scale() const;

   void origin(float x, float y);
   void origin(const sf::Vector2f& origin);
   const sf::Vector2f& origin() const;

   void move(float x, float y);
   void move(const sf::Vector2f& offset);
   
   void rotate(float angle);

   const sf::Transform& transform() const;
   const sf::Transform& inverse_transform() const;

   // serialize interface
   virtual std::string serialize(Serializer& s);
   virtual void deserialize(Serializer& s, Scene& scene, std::string& d);

private:
   bool visible_;
   sf::Transformable transform_;

   Handle parent_;
   std::vector<Handle> children_;
};

#endif
