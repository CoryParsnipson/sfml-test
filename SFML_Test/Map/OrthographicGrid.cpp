#include "OrthographicGrid.h"

#include "Graphic.h"
#include "UtilFactory.h"
#include "Entity.h"

OrthographicGrid::OrthographicGrid(const std::string& id)
: Grid(id)
, origin_dot_(nullptr)
{
   this->create_origin_dot();
}

OrthographicGrid::OrthographicGrid(const std::string& id, int tile_size)
: Grid(id, tile_size)
, origin_dot_(nullptr)
{
   this->create_origin_dot();
}

OrthographicGrid::OrthographicGrid(const std::string& id, const sf::Vector2f& tile_size)
: Grid(id, tile_size)
, origin_dot_(nullptr)
{
   this->create_origin_dot();
}

OrthographicGrid::~OrthographicGrid() {
   delete this->origin_dot_;
}

void OrthographicGrid::origin(const sf::Vector2f& origin) {
   this->origin_.x = origin.x;
   this->origin_.y = origin.y;
   
   // update origin dot position
   this->create_origin_dot();
}

sf::Vector2f OrthographicGrid::coord_to_screen(const sf::Vector2f& coord) {
   return sf::Vector2f(coord.x * this->tile_width(), coord.y * this->tile_height());
}

sf::Vector2f OrthographicGrid::floor(const sf::Vector2f& pos) {
   return sf::Vector2f(std::floor(pos.x / this->tile_width()) * this->tile_width(), std::floor(pos.y / this->tile_height()) * this->tile_height());
}

sf::Vector2f OrthographicGrid::round(const sf::Vector2f& pos) {
   return sf::Vector2f(std::round(pos.x / this->tile_width()) * this->tile_width(), std::round(pos.y / this->tile_height()) * this->tile_height());
}

sf::Vector2f OrthographicGrid::ceil(const sf::Vector2f& pos) {
   return sf::Vector2f(std::ceil(pos.x / this->tile_width()) * this->tile_width(), std::ceil(pos.y / this->tile_height()) * this->tile_height());
}

void OrthographicGrid::draw(Graphics& graphics) {
   this->origin_dot_->draw(graphics);
}

void OrthographicGrid::layer(Layer* layer) {
   Draw::layer(layer);

   this->origin_dot_->layer(layer);
}

Layer* OrthographicGrid::layer() {
   return Draw::layer();
}

void OrthographicGrid::create_origin_dot() {
   Shape* origin_dot = new Shape(new sf::RectangleShape(sf::Vector2f(3, 3)));
   origin_dot->set_fill_color(sf::Color::White);
   origin_dot->set_position(this->origin_);

   delete this->origin_dot_;
   this->origin_dot_ = UtilFactory::inst()->create_graphic(
      origin_dot,
      origin_dot->get_global_bounds(),
      this->show_debug_info_
   );
   this->origin_dot_->layer(this->layer());
}
