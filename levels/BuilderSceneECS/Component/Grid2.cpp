#include "Grid2.h"

Grid2::Grid2(const std::string& id /* = "Grid2 Component" */, sf::Vector2f origin /* = sf::Vector2f(0, 0) */, unsigned int width /* = 1 */, unsigned int height /* = 1 */)
: Component(id)
, origin_(origin)
, delta_(0, 0)
, tile_width_(width)
, tile_height_(height)
{
}

Grid2::Grid2(const Grid2& other)
: Component(other.id())
, origin_(other.origin_)
, delta_(other.delta_)
, tile_width_(other.tile_width_)
, tile_height_(other.tile_height_)
{
}

Grid2::~Grid2() {
}

Grid2& Grid2::operator=(const Grid2& other) {
   Grid2 tmp(other);
   this->swap(tmp);
   return *this;
}

void Grid2::swap(Grid2& other) {
   std::swap(static_cast<Component&>(*this), static_cast<Component&>(other));
   std::swap(this->origin_, other.origin_);
   std::swap(this->delta_, other.delta_);
   std::swap(this->tile_width_, other.tile_width_);
   std::swap(this->tile_height_, other.tile_height_);
}

unsigned int Grid2::tile_width() const {
   return this->tile_width_;
}

void Grid2::tile_width(unsigned int width) {
   this->tile_width_ = width;
}

unsigned int Grid2::tile_height() const {
   return this->tile_height_;
}

void Grid2::tile_height(unsigned int height) {
   this->tile_height_ = height;
}
