#include <cmath>
#include <cassert>

#include "Grid.h"

Grid::Grid(const std::string& id /* = "Grid Component" */, sf::Vector2f origin /* = sf::Vector2f(0, 0) */, float width /* = 1.f */, float height /* = 1.f */)
: Component(id)
, origin_(origin)
, tile_width_(width)
, tile_height_(height)
, zoom_factor_(1.f, 1.f)
{
}

Grid::Grid(const Grid& other)
: Component(other.id())
, origin_(other.origin_)
, tile_width_(other.tile_width_)
, tile_height_(other.tile_height_)
, zoom_factor_(other.zoom_factor_)
{
}

Grid::~Grid() {
}

Grid& Grid::operator=(const Grid& other) {
   Grid tmp(other);
   this->swap(tmp);
   return *this;
}

void Grid::swap(Grid& other) {
   std::swap(static_cast<Component&>(*this), static_cast<Component&>(other));
   std::swap(this->origin_, other.origin_);
   std::swap(this->tile_width_, other.tile_width_);
   std::swap(this->tile_height_, other.tile_height_);
   std::swap(this->zoom_factor_, other.zoom_factor_);
}

void Grid::origin(sf::Vector2f origin) {
   this->origin_ = origin;
}

void Grid::origin(float origin_x, float origin_y) {
   this->origin(sf::Vector2f(origin_x, origin_y));
}

const sf::Vector2f& Grid::origin() const {
   return this->origin_;
}

float Grid::tile_width() const {
   return this->tile_width_;
}

void Grid::tile_width(float width) {
   this->tile_width_ = width;
}

float Grid::tile_height() const {
   return this->tile_height_;
}

void Grid::tile_height(float height) {
   this->tile_height_ = height;
}

void Grid::zoom_factor(sf::Vector2f zoom_factor) {
   this->zoom_factor_ = zoom_factor;
}

void Grid::zoom_factor(float zoom_factor_x, float zoom_factor_y) {
   this->zoom_factor(sf::Vector2f(zoom_factor_x, zoom_factor_y));
}

const sf::Vector2f& Grid::zoom_factor() const {
   return this->zoom_factor_;
}

sf::Vector2f Grid::floor(const sf::Vector2f& pos) {
   float tile_width = this->tile_width() * this->zoom_factor().x;
   float tile_height = this->tile_height() * this->zoom_factor().y;

   sf::Vector2f origin_offset(std::fmod(this->origin_.x, tile_width), std::fmod(this->origin_.y, tile_height));

   return origin_offset + sf::Vector2f(
      std::floor(pos.x / tile_width) * tile_width,
      std::floor(pos.y / tile_height) * tile_height
   );
}

sf::Vector2f Grid::round(const sf::Vector2f& pos) {
   float tile_width = this->tile_width() * this->zoom_factor().x;
   float tile_height = this->tile_height() * this->zoom_factor().y;

   sf::Vector2f origin_offset(std::fmod(this->origin_.x, tile_width), std::fmod(this->origin_.y, tile_height));

   return origin_offset + sf::Vector2f(
      std::round(pos.x / tile_width) * tile_width,
      std::round(pos.y / tile_height) * tile_height
   );
}

sf::Vector2f Grid::ceil(const sf::Vector2f& pos) {
   float tile_width = this->tile_width() * this->zoom_factor().x;
   float tile_height = this->tile_height() * this->zoom_factor().y;

   sf::Vector2f origin_offset(std::fmod(this->origin_.x, tile_width), std::fmod(this->origin_.y, tile_height));

   return origin_offset + sf::Vector2f(
      std::ceil(pos.x / tile_width) * tile_width,
      std::ceil(pos.y / tile_height) * tile_height
   );
}

sf::Vector2i Grid::grid_index(const sf::Vector2f& pos) {
   sf::Vector2f rounded_pos = this->round(pos);
   float tile_width = this->tile_width() * this->zoom_factor().x;
   float tile_height = this->tile_height() * this->zoom_factor().y;

   return sf::Vector2i(std::round(rounded_pos.x / tile_width), std::round(rounded_pos.y / tile_height));
}

sf::Vector2i Grid::grid_index(float x, float y) {
   return this->grid_index(sf::Vector2f(x, y));
}

std::string Grid::serialize(Serializer& s) {
   Serializer::SerialData data;

   data["id"] = this->id();
   data["type"] = "Grid";
   data["origin_x"] = std::to_string(this->origin().x);
   data["origin_y"] = std::to_string(this->origin().y);
   data["tile_width"] = std::to_string(this->tile_width());
   data["tile_height"] = std::to_string(this->tile_height());

   return s.serialize(data);
}

void Grid::deserialize(Serializer& s, Scene& scene, std::string& d) {
   Serializer::SerialData data = s.deserialize(scene, d);

   assert(data["id"] == this->id());

   this->origin(std::stof(data["origin_x"]), std::stof(data["origin_y"]));
   this->tile_width(std::stof(data["tile_width"]));
   this->tile_height(std::stof(data["tile_height"]));
}
