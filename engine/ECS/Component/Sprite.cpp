#include <cassert>
#include <algorithm>

#include "Sprite.h"
#include "Scene.h"
#include "RenderSurface.h"
#include "Texture.h"

// ----------------------------------------------------------------------------
// Animation implementation
// ----------------------------------------------------------------------------
Animation::Animation(const std::string& id /* = "Animation" */)
: id_(id)
{
}

Animation::~Animation() {
   this->frames_.clear();
}

const std::string& Animation::id() const {
   return this->id_;
}

unsigned int Animation::num_frames() const {
   return this->frames_.size();
}

void Animation::add(const sf::IntRect& rect, unsigned int duration) {
   this->frames_.push_back(std::make_tuple(rect, duration));
}

void Animation::insert(int idx, const sf::IntRect& rect, unsigned int duration) {
   std::vector<std::tuple<sf::IntRect, unsigned int> >::iterator insert_pos = this->frames_.begin();
   if (idx > 0 && (unsigned int)idx < this->frames_.size()) {
      insert_pos += idx;
   } else if ((unsigned int)idx > this->frames_.size()) {
      insert_pos = this->frames_.end();
   }

   this->frames_.insert(insert_pos, std::make_tuple(rect, duration));
}

void Animation::remove(int idx) {
   if (idx < 0 || (unsigned int)idx > this->frames_.size()) {
      return;
   }

   std::vector<std::tuple<sf::IntRect, unsigned int> >::iterator erase_pos = this->frames_.begin();
   this->frames_.erase(erase_pos + idx);
}

const sf::IntRect& Animation::get_rect(int idx) const {
   return std::get<0>(this->frames_.at(idx));
}

unsigned int Animation::get_duration(int idx) const {
   return std::get<1>(this->frames_.at(idx));
}

// ----------------------------------------------------------------------------
// Sprite implementation
// ----------------------------------------------------------------------------
Sprite::Sprite(const std::string& id /* = "Sprite" */)
: Component(id)
, texture_(nullptr)
, drawable_()
, default_animation_(id + " Default Animation")
, cur_idx(0)
, remaining_frame_duration_(0)
, animation_(&this->default_animation_)
{
   this->default_animation_.add(this->drawable_.getTextureRect(), 0);
}

Sprite::Sprite(const std::string& id, std::shared_ptr<Texture> texture)
: Component(id)
, texture_(texture)
, drawable_(texture->get_texture())
, default_animation_(id + " Default Animation")
, cur_idx(0)
, remaining_frame_duration_(0)
, animation_(&this->default_animation_)
{
   this->default_animation_.add(this->drawable_.getTextureRect(), 0);
}

Sprite::Sprite(const std::string& id, std::shared_ptr<Texture> texture, const sf::IntRect& texture_rect)
: Component(id)
, texture_(texture)
, drawable_(texture->get_texture(), texture_rect)
, default_animation_(id + " Default Animation")
, cur_idx(0)
, remaining_frame_duration_(0)
, animation_(&this->default_animation_)
{
   this->default_animation_.add(this->drawable_.getTextureRect(), 0);
}

Sprite::Sprite(const Sprite& other)
: Component(other.id())
, texture_(other.texture_)
, drawable_(other.drawable_)
, default_animation_(other.id() + " Default Animation")
, cur_idx(other.cur_idx)
, remaining_frame_duration_(other.remaining_frame_duration_)
, animation_(&this->default_animation_)
{
   this->default_animation_.add(this->drawable_.getTextureRect(), 0);
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
   this->default_animation_ = other.default_animation_;
   std::swap(this->cur_idx, other.cur_idx);
   std::swap(this->remaining_frame_duration_, other.remaining_frame_duration_);
   this->animation_ = &this->default_animation_;
}

void Sprite::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   surface.draw(this->drawable_, render_states);
}

void Sprite::update(Game& game) {
   assert(this->animation_);

   // TODO: update animation here
}

sf::FloatRect Sprite::local_bounds() const {
   return this->drawable_.getLocalBounds();
}

sf::FloatRect Sprite::global_bounds() const {
   return this->drawable_.getGlobalBounds();
}

void Sprite::position(float x, float y) {
   this->drawable_.setPosition(x, y);
}

const sf::Vector2f& Sprite::position() const {
   return this->drawable_.getPosition();
}

void Sprite::move(float x, float y) {
   this->drawable_.move(x, y);
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

void Sprite::animation(Animation* animation) {
   if (animation == nullptr) {
      this->animation_ = &this->default_animation_;
   } else {
      this->animation_ = animation;
   }
}

const Animation& Sprite::animation() const {
   return *this->animation_;
}

std::string Sprite::serialize(Serializer& s) {
   Serializer::SerialData data;

   Color color(this->color());

   data["type"] = "Sprite";

   data["id"] = this->id();
   data["x"] = std::to_string(this->position().x);
   data["y"] = std::to_string(this->position().y);
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
   this->position(std::stof(data["x"]), std::stof(data["y"]));
   this->rotation(std::stof(data["rotation"]));
   this->origin(std::stof(data["origin_x"]), std::stof(data["origin_y"]));
   this->color(color);

   std::shared_ptr<Texture> texture = scene.textures().get(data["texture"]);
   if (texture) {
      this->texture(texture);
   }
}
