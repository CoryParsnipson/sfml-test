#include <stdexcept>
#include "Animation.h"

Animation::Animation(const std::string& id /* = "Animation" */)
: id_(id)
{
}

Animation::Animation(const std::string& id, TexturePtr texture)
: id_(id)
, texture_(texture)
{
}

Animation::~Animation() {
}

const std::string& Animation::id() const {
   return this->id_;
}

unsigned int Animation::num_frames() const {
   return this->frames_.size();
}

const TexturePtr Animation::texture() const {
   return this->texture_;
}

void Animation::texture(TexturePtr texture) {
   this->texture_ = texture;
}

void Animation::add(const sf::IntRect& rect, unsigned int duration /* = 1 */) {
   this->frames_.push_back(std::make_tuple(rect, duration));
}

void Animation::insert(unsigned int idx, const sf::IntRect& rect, unsigned int duration /* = 1 */) {
   if (idx >= this->num_frames()) {
      throw std::out_of_range("Animation '" + this->id() + "' has no idx '" + std::to_string(idx) + "'");
   }

   this->frames_[idx] = std::make_tuple(rect, duration);
}

void Animation::remove(unsigned int idx) {
   if (idx >= this->num_frames()) {
      return;
   }

   this->frames_.erase(this->frames_.begin() + idx);
}

const sf::IntRect& Animation::frame(unsigned int idx) const {
   return std::get<0>(this->frames_[idx]);
}

unsigned int Animation::duration(unsigned int idx) const {
   return std::get<1>(this->frames_[idx]);
}

void Animation::duration(unsigned int idx, unsigned int duration) {
   this->insert(idx, this->frame(idx), duration);
}

void Animation::id(const std::string& id) {
   this->id_ = id;
}
