#ifndef SPRITE_H
#define SPRITE_H

#include <tuple>
#include <string>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

#include "Color.h"
#include "Component.h"
#include "Updateable.h"
#include "Animation.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Texture;
class RenderSurface;

// ----------------------------------------------------------------------------
// Sprite
//
// This is a primitive Graphic class that has a reference to a texture.
// ----------------------------------------------------------------------------
class Sprite
: public Component
, public Updateable
{
public:
   explicit Sprite(const std::string& id = "Sprite");
   Sprite(const std::string& id, std::shared_ptr<Texture> texture);
   Sprite(const std::string& id, std::shared_ptr<Texture> texture, const sf::IntRect& texture_rect);
   Sprite(const Sprite& other);
   virtual ~Sprite();

   Sprite& operator=(const Sprite& other);
   void swap(Sprite& other);

   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   virtual void update(Game& game);

   virtual sf::FloatRect local_bounds() const;
   virtual sf::FloatRect global_bounds() const;

   virtual void rotation(float angle);
   virtual float rotation() const;

   virtual void scale(float x, float y);
   virtual void scale(const sf::Vector2f& scale) { this->scale(scale.x, scale.y); }
   virtual const sf::Vector2f& scale() const;

   virtual void origin(float x, float y);
   virtual void origin(const sf::Vector2f& factor) { this->origin(factor.x, factor.y); }
   virtual const sf::Vector2f& origin() const;

   virtual void offset(float x, float y);
   virtual void offset(const sf::Vector2f& pos) { this->offset(pos.x, pos.y); }
   virtual const sf::Vector2f& offset() const;

   virtual void move_offset(float x, float y);
   virtual void move_offset(const sf::Vector2f& offset) { this->move_offset(offset.x, offset.y); }

   virtual void color(const sf::Color& color);
   virtual const sf::Color& color() const;

   virtual const sf::Transform& transform() const;
   
   // sprite manipulation interface
   void texture(std::shared_ptr<Texture> texture);
   std::shared_ptr<Texture> texture() const;

   void texture_rect(const sf::IntRect& rectangle);
   const sf::IntRect& texture_rect() const;

   void animation(AnimationPtr animation);
   AnimationPtr animation() const;

   // serialize interface
   virtual std::string serialize(Serializer& s);
   virtual void deserialize(Serializer& s, Scene& scene, std::string& d);

private:
   std::shared_ptr<Texture> texture_;
   sf::Sprite drawable_;

   unsigned int current_frame_idx_;
   int current_frame_duration_;
   AnimationPtr animation_;
};

#endif
