#include <stdexcept>
#include "Animation.h"

Animation::Animation(const std::string& id /* = "Animation" */)
: id_(id)
, current_frame_idx_(0)
, current_duration_(0)
{
}

Animation::Animation(const std::string& id, TexturePtr texture)
: id_(id)
, texture_(texture)
, current_frame_idx_(0)
, current_duration_(0)
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

const Animation::Frame& Animation::current_frame() const {
   return this->frames_[this->current_frame_idx_];
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

void Animation::update(unsigned int delta) {
   if (this->frames_.size() <= 1) {
      return;
   }

   this->current_duration_ -= delta;

   while (this->current_duration_ <= 0) {
      this->current_frame_idx_ = (this->current_frame_idx_ + 1) % (int)this->frames_.size();
      this->current_duration_ += this->duration(this->current_frame_idx_);
   }
}

void Animation::id(const std::string& id) {
   this->id_ = id;
}
