#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include <SFML/Graphics.hpp>

#include "ObjectPool.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Entity;

// ----------------------------------------------------------------------------
// SceneNode
// ----------------------------------------------------------------------------
class SceneNode {
public:
   SceneNode();
   ~SceneNode();

   Handle entity() const;
   void entity(Handle entity);

   SceneNode* parent() const;
   void parent(SceneNode* parent);

   void add(SceneNode* child, int idx = -1);

   void remove(SceneNode* child);
   void remove(unsigned int idx);

   SceneNode* get_child(unsigned int idx) const;
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
   void scale(const sf::Vector2f& scale);
   const sf::Vector2f& scale() const;

   void origin(float x, float y);
   void origin(const sf::Vector2f& origin);
   const sf::Vector2f& origin() const;

   void move(float x, float y);
   void move(const sf::Vector2f& offset);
   
   void rotate(float angle);

   const sf::Transform& transform() const;
   const sf::Transform& inverse_transform() const;

private:
   bool visible_;
   Handle entity_;
   sf::Transformable transform_;

   SceneNode* parent_;
   std::vector<SceneNode*> children_;
};

#endif
