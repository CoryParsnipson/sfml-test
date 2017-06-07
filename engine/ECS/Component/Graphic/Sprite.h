#ifndef SPRITE_H
#define SPRITE_H

#include <tuple>
#include <string>
#include <vector>

#include "sfml.h"

#include "Update.h"
#include "PooledComponent.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Texture;
class RenderSurface;

// ----------------------------------------------------------------------------
// Animation
// 
// Supply this class to a sprite to specify animation frames within the
// Sprite's texture.
// ----------------------------------------------------------------------------
class Animation {
public:
   Animation(const std::string& id = "Animation");
   virtual ~Animation();

   const std::string& id() const;
   unsigned int num_frames() const;

   void add(const sf::IntRect& rect, unsigned int duration);
   void insert(int idx, const sf::IntRect& rect, unsigned int duration);
   void remove(int idx);

   const sf::IntRect& get_rect(int idx) const;
   unsigned int get_duration(int idx) const;

private:
   std::string id_;
   std::vector<std::tuple<sf::IntRect, unsigned int> > frames_;
};

// ----------------------------------------------------------------------------
// Sprite
//
// This is a primitive Graphic class that has a reference to a texture.
// ----------------------------------------------------------------------------
class Sprite
: public PooledComponent<Sprite>
{
public:
   explicit Sprite(const std::string& id = "Sprite");
   Sprite(const std::string& id, Texture& texture);
   Sprite(const std::string& id, Texture& texture, const sf::IntRect& texture_rect);
   explicit Sprite(const Sprite& other);
   virtual ~Sprite();

   Sprite& operator=(const Sprite& other);
   void swap(Sprite& other);

   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   virtual void update(Game& game);

   virtual sf::FloatRect local_bounds() const;
   virtual sf::FloatRect global_bounds() const;

   virtual void position(float x, float y);
   virtual void position(const sf::Vector2f& pos) { this->position(pos.x, pos.y); }
   virtual const sf::Vector2f& position() const;

   virtual void move(float x, float y);
   virtual void move(const sf::Vector2f& offset) { this->move(offset.x, offset.y); }

   virtual void rotation(float angle);
   virtual float rotation() const;

   virtual void scale(float x, float y);
   virtual void scale(const sf::Vector2f& scale) { this->scale(scale.x, scale.y); }
   virtual const sf::Vector2f& scale() const;

   virtual void origin(float x, float y);
   virtual void origin(const sf::Vector2f& factor) { this->origin(factor.x, factor.y); }
   virtual const sf::Vector2f& origin() const;

   virtual void color(const sf::Color& color);
   virtual const sf::Color& color() const;

   virtual const sf::Transform& transform() const;
   
   // sprite manipulation interface
   void texture(Texture& texture);
   const Texture* texture() const;

   void animation(Animation* animation);
   const Animation& animation() const;

private:
   Texture* texture_;
   sf::Sprite drawable_;
   Animation default_animation_;

   int cur_idx;
   unsigned int remaining_frame_duration_;
   Animation* animation_;
};

#endif
