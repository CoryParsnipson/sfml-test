#include <cassert>
#include <algorithm>
#include <tuple>

#include "Sprite.h"
#include "Scene.h"
#include "RenderSurface.h"
#include "Texture.h"

Sprite::Sprite(const std::string& id /* = "Sprite" */)
: Component(id)
, texture_(nullptr)
, drawable_()
, current_frame_idx_(0)
, current_frame_duration_(0)
, animation_(nullptr)
{
}

Sprite::Sprite(const std::string& id, std::shared_ptr<Texture> texture)
: Component(id)
, texture_(texture)
, drawable_(texture->get_texture())
, current_frame_idx_(0)
, current_frame_duration_(0)
, animation_(nullptr)
{
}

Sprite::Sprite(const std::string& id, std::shared_ptr<Texture> texture, const sf::IntRect& texture_rect)
: Component(id)
, texture_(texture)
, drawable_(texture->get_texture(), texture_rect)
, current_frame_idx_(0)
, current_frame_duration_(0)
, animation_(nullptr)
{
}

Sprite::Sprite(const Sprite& other)
: Component(other.id())
, texture_(other.texture_)
, drawable_(other.drawable_)
, current_frame_idx_(other.current_frame_idx_)
, current_frame_duration_(other.current_frame_duration_)
, animation_(nullptr)
{
}

Sprite::~Sprite() {
}

Sprite& Sprite::operator=(const Sprite& other) {
   Sprite tmp(other);
   this->swap(tmp);
   return *this;
}

void Sprite::swap(Sprite& other) {
   std::swap(static_cast<Component&>(*this), static_cast<Component&>(other));

   this->texture_ = other.texture_;
   std::swap(this->drawable_, other.drawable_);
   std::swap(this->current_frame_idx_, other.current_frame_idx_);
   std::swap(this->current_frame_duration_, other.current_frame_duration_);
}

void Sprite::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   surface.draw(this->drawable_, render_states);
}

void Sprite::update(Game& game) {
   bool need_to_change_frame = false;
   unsigned int delta = 1; // TODO: fix your timestep

   // if the animation is set, it will overwrite any manually set textures
   if (this->animation_ && this->animation_->num_frames() > 1) {
      this->current_frame_duration_ -= delta; // should this be delta % num_frames?

      while (this->current_frame_duration_ <= 0) {
         this->current_frame_idx_ = (this->current_frame_idx_ + 1) % this->animation_->num_frames();
         this->current_frame_duration_ += this->animation_->duration(this->current_frame_idx_);
         need_to_change_frame = true;
      }

      if (need_to_change_frame) {
         this->set_texture_rect(this->animation_->frame(this->current_frame_idx_));
      }
   }
}

sf::FloatRect Sprite::local_bounds() const {
   return this->drawable_.getLocalBounds();
}

sf::FloatRect Sprite::global_bounds() const {
   return this->drawable_.getGlobalBounds();
}

void Sprite::rotation(float angle) {
   this->drawable_.setRotation(angle);
}

float Sprite::rotation() const {
   return this->drawable_.getRotation();
}

void Sprite::scale(float x, float y) {
   this->drawable_.setScale(x, y);
}

const sf::Vector2f& Sprite::scale() const {
   return this->drawable_.getScale();
}

void Sprite::origin(float x, float y) {
   this->drawable_.setOrigin(x, y);
}

const sf::Vector2f& Sprite::origin() const {
   return this->drawable_.getOrigin();
}

void Sprite::offset(float x, float y) {
   this->drawable_.setPosition(x, y);
}

const sf::Vector2f& Sprite::offset() const {
   return this->drawable_.getPosition();
}

void Sprite::move_offset(float x, float y) {
   this->drawable_.move(x, y);
}

void Sprite::color(const sf::Color& color) {
   this->drawable_.setColor(color);
}

const sf::Color& Sprite::color() const {
   return this->drawable_.getColor();
}

const sf::Transform& Sprite::transform() const {
   return this->drawable_.getTransform();
}

void Sprite::texture(std::shared_ptr<Texture> texture) {
   this->texture_ = texture;
   this->drawable_.setTexture(texture->get_texture(), true);
}

std::shared_ptr<Texture> Sprite::texture() const {
   return this->texture_;
}

void Sprite::set_texture_rect(const sf::IntRect& rectangle) {
   this->drawable_.setTextureRect(rectangle);
}

const sf::IntRect& Sprite::get_texture_rect() const {
   return this->drawable_.getTextureRect();
}

void Sprite::animation(AnimationPtr animation) {
   this->animation_ = animation;

   if (this->animation_) {
      // update to the animation texture
      this->texture(this->animation_->texture());

      // reset playback data
      this->current_frame_idx_ = 0;
      this->current_frame_duration_ = 0;

      // update the sprite to show the first frame, if this animation has any frames
      if (this->animation_->num_frames() >= 1) {
         this->set_texture_rect(this->animation_->frame(0));
         this->current_frame_duration_ = this->animation_->duration(0);
      }
   }
}

AnimationPtr Sprite::animation() const {
   return this->animation_;
}

std::string Sprite::serialize(Serializer& s) {
   Serializer::SerialData data;

   Color color(this->color());

   data["type"] = "Sprite";

   data["id"] = this->id();
   data["x"] = std::to_string(this->offset().x);
   data["y"] = std::to_string(this->offset().y);
   data["rotation"] = std::to_string(this->rotation());
   data["origin_x"] = std::to_string(this->origin().x);
   data["origin_y"] = std::to_string(this->origin().y);
   data["color"] = color.serialize(s);
   data["texture"] = (this->texture_ ? this->texture_->id() : "");

   return s.serialize(data);
}

void Sprite::deserialize(Serializer& s, Scene& scene, std::string& d) {
   Serializer::SerialData data = s.deserialize(scene, d);

   Color color(sf::Color::Black);
   color.deserialize(s, scene, data["color"]);

   this->id(data["id"]);
   this->offset(std::stof(data["x"]), std::stof(data["y"]));
   this->rotation(std::stof(data["rotation"]));
   this->origin(std::stof(data["origin_x"]), std::stof(data["origin_y"]));
   this->color(color);

   std::shared_ptr<Texture> texture = scene.textures().get(data["texture"]);
   if (texture) {
      this->texture(texture);
   }
}
