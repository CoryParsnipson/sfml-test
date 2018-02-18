#include <cmath>

#include "Grid.h"

Grid::Grid(const std::string& id /* = "Grid Component" */, sf::Vector2f origin /* = sf::Vector2f(0, 0) */, float width /* = 1.f */, float height /* = 1.f */)
: Component(id)
, zoom_factor(1.f, 1.f)
, origin_(origin)
, tile_width_(width)
, tile_height_(height)
{
}

Grid::Grid(const Grid& other)
: Component(other.id())
, zoom_factor(other.zoom_factor)
, origin_(other.origin_)
, tile_width_(other.tile_width_)
, tile_height_(other.tile_height_)
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
   std::swap(this->zoom_factor, other.zoom_factor);
   std::swap(this->origin_, other.origin_);
   std::swap(this->tile_width_, other.tile_width_);
   std::swap(this->tile_height_, other.tile_height_);
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

sf::Vector2f Grid::floor(const sf::Vector2f& pos) {
   float tile_width = this->tile_width() * this->zoom_factor.x;
   float tile_height = this->tile_height() * this->zoom_factor.y;

   sf::Vector2f origin_offset(std::fmod(this->origin_.x, tile_width), std::fmod(this->origin_.y, tile_height));

   return origin_offset + sf::Vector2f(
      std::floor(pos.x / tile_width) * tile_width,
      std::floor(pos.y / tile_height) * tile_height
   );
}

sf::Vector2f Grid::round(const sf::Vector2f& pos) {
   float tile_width = this->tile_width() * this->zoom_factor.x;
   float tile_height = this->tile_height() * this->zoom_factor.y;

   sf::Vector2f origin_offset(std::fmod(this->origin_.x, tile_width), std::fmod(this->origin_.y, tile_height));

   return origin_offset + sf::Vector2f(
      std::round(pos.x / tile_width) * tile_width,
      std::round(pos.y / tile_height) * tile_height
   );
}

sf::Vector2f Grid::ceil(const sf::Vector2f& pos) {
   float tile_width = this->tile_width() * this->zoom_factor.x;
   float tile_height = this->tile_height() * this->zoom_factor.y;

   sf::Vector2f origin_offset(std::fmod(this->origin_.x, tile_width), std::fmod(this->origin_.y, tile_height));

   return origin_offset + sf::Vector2f(
      std::ceil(pos.x / tile_width) * tile_width,
      std::ceil(pos.y / tile_height) * tile_height
   );
}
