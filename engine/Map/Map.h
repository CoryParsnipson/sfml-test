#ifndef MAP_H
#define MAP_H

#include <string>

#include "sfml.h"
#include "SceneObject.h"

class Grid;
class Entity;

class Map
: public SceneObject
{
public:
   Map(std::string id = "map");
   virtual ~Map();

   void id(std::string id);

   virtual std::string to_string();

   // serializable interface
   virtual std::string serialize(Serializer& s);
   virtual void deserialize(Serializer& s, Game& g, std::string& d);

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
   std::string id_;
   Grid* grid_;
};

#endif
