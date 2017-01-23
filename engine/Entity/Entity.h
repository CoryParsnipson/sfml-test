#ifndef ENTITY_H
#define ENTITY_H

#include <map>
#include <string>

#include "sfml.h"

#include "SceneObject.h"
#include "Serializable.h"

// forward declarations
class Part;

class Entity
: public SceneObject
{
public:
   using PartList = std::map<std::string, Part*>;

   Entity(std::string name = "entity");
   virtual ~Entity();

   std::string id();
   std::string to_string();

   void enable_debug_wireframe(bool debug_wireframe);
   bool enable_debug_wireframe();

   void enable_debug_text(bool debug_text);
   bool enable_debug_text();

   void set_position(float x, float y);
   void set_position(const sf::Vector2f& pos);

   void set_size(float width, float height);
   void set_size(const sf::Vector2f& size);

   // part management interface
   void add(Part* part);
   void remove(const std::string& part_id);
   Part* get(const std::string& part_name);

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

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr);

protected:
   bool enable_debug_wireframe_;
   bool enable_debug_text_;

   std::string name_;
   PartList parts_;

   // scene graph interface hooks
   virtual void do_draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);
};

#endif
