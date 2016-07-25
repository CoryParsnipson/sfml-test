#ifndef MAP_H
#define MAP_H

#include "dependencies.h"

#include "SceneObject.h"

class Grid;
class Entity;
class Serializer;

class Map
: public SceneObject
{
public:
   Map();
   virtual ~Map();

   virtual std::string to_string();
   virtual void serialize(Serializer& serializer);

   Grid* grid();

   using SceneObject::add;
   void add(Grid* grid);

   // scene graph interface
   virtual bool intersects(const sf::Vector2i& other);
   virtual bool intersects(const sf::Vector2f& other);
   virtual bool intersects(const sf::FloatRect& other);
   virtual bool intersects(const SceneObject& other);

   // scene graph visitor interface
   virtual void accept(SceneGraphVisitor& visitor);

protected:
   Grid* grid_;
};

#endif
