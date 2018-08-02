#ifndef ANIMATION_H
#define ANIMATION_H

#include <tuple>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Texture.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Animation;

// ----------------------------------------------------------------------------
// aliases
// ----------------------------------------------------------------------------
using AnimationPtr = std::shared_ptr<Animation>;

// ----------------------------------------------------------------------------
// Animation
// 
// Supply this class to a sprite to specify animation frames within the
// Sprite's texture.
// ----------------------------------------------------------------------------
class Animation {
public:
   using Frame = std::tuple<sf::IntRect, unsigned int>;
   using FrameList = std::vector<Frame>;

   Animation(const std::string& id = "Animation");
   Animation(const std::string& id, TexturePtr texture);
   virtual ~Animation();

   const std::string& id() const;

   unsigned int num_frames() const;
   const Frame& current_frame() const;

   const TexturePtr texture() const;
   void texture(TexturePtr texture);

   void add(const sf::IntRect& rect, unsigned int duration = 1);
   void insert(unsigned int idx, const sf::IntRect& rect, unsigned int duration = 1);
   void remove(unsigned int idx);

   const sf::IntRect& frame(unsigned int idx) const;

   unsigned int duration(unsigned int idx) const;
   void duration(unsigned int idx, unsigned int duration);

   // call this to update the current frame and remaining duration
   void update(unsigned int delta);

   // TODO: reset
   // TODO: reverse?

protected:
   void id(const std::string& id);

private:
   std::string id_;
   TexturePtr texture_;
   FrameList frames_;

   unsigned int current_frame_idx_;
   int current_duration_;
};

#endif
