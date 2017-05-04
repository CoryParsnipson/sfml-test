#ifndef ENTITY_H
#define ENTITY_H

#include <map>
#include <string>
#include <cassert>
#include <typeindex>

#include "sfml.h"

#include "ObjectPool.h"
#include "PooledComponent.h"

#include "SceneObject.h"
#include "Serializable.h"

#include "Graphic.h"

// ----------------------------------------------------------------------------
// forward declarations 
// ----------------------------------------------------------------------------
class Part;

// ----------------------------------------------------------------------------
// Entity class
// ----------------------------------------------------------------------------
class Entity
: public SceneObject
{
public:
   using PartList = std::map<std::string, Part*>;
   using ComponentList = std::map<std::type_index, Handle>;

   Entity(std::string id = "entity");
   virtual ~Entity();

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
   Part* get(const std::string& part_id);

   template <typename ComponentType,
             typename = typename std::enable_if<std::is_base_of<PooledComponent<ComponentType>, ComponentType>::value::type> >
   ComponentType* get();

   template <typename ComponentType,
             typename = typename std::enable_if<std::is_base_of<PooledComponent<ComponentType>, ComponentType>::value::type> >
   void set(Handle& handle);

   template <typename ComponentType,
             typename = typename std::enable_if<std::is_base_of<PooledComponent<ComponentType>, ComponentType>::value::type> >
   Handle add();

   template <typename ComponentType,
             typename = typename std::enable_if<std::is_base_of<PooledComponent<ComponentType>, ComponentType>::value::type> >
   void remove();

   // serializable interface
   virtual std::string serialize(Serializer& s);
   virtual void deserialize(Serializer& s, Scene& scene, std::string& d);

   // scene graph interface
   virtual bool intersects(const sf::Vector2i& other);
   virtual bool intersects(const sf::Vector2f& other);
   virtual bool intersects(const sf::FloatRect& other);
   virtual bool intersects(const SceneObject& other);

   // scene graph visitor interface
   virtual void accept(SceneGraphVisitor& visitor);

   // update interface
   virtual void update(Game& game);

protected:
   bool enable_debug_wireframe_;
   bool enable_debug_text_;

   PartList parts_;
   ComponentList components_;

   // scene graph interface hooks
   virtual void do_draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);
};

// ----------------------------------------------------------------------------
// template member specializations
// ----------------------------------------------------------------------------
template <typename ComponentType,
          typename = typename std::enable_if<std::is_base_of<PooledComponent<ComponentType>, ComponentType>::value::type> >
ComponentType* Entity::get() {
   try {
      return ComponentType::pool.get(this->components_.at(std::type_index(typeid(ComponentType))));
   } catch (const std::out_of_range& e) {
      return nullptr;
   }
}

template <typename ComponentType,
          typename = typename std::enable_if<std::is_base_of<PooledComponent<ComponentType>, ComponentType>::value::type> >
void Entity::set(Handle& handle) {
   this->components_[std::type_index(typeid(ComponentType))] = handle;
}

template <typename ComponentType,
          typename = typename std::enable_if<std::is_base_of<PooledComponent<ComponentType>, ComponentType>::value::type> >
Handle Entity::add() {
   Handle handle(ComponentType::pool.add());

   this->components_.insert(
      std::pair<std::type_index, Handle>(
         std::type_index(typeid(ComponentType)),
         handle
      )
   );

   return handle;
}

template <typename ComponentType,
          typename = typename std::enable_if<std::is_base_of<PooledComponent<ComponentType>, ComponentType>::value::type> >
void Entity::remove() {
   ComponentList::const_iterator it = this->components_.find(std::type_index(typeid(ComponentType)));
   if (it != this->components_.end()) {
      // remove component from it's object pool
      ComponentType::pool.remove(*it);

      // unattach component from this entity
      this->components_.erase(it);
   }
}

#endif
