#include <cassert>

#include "OrthographicGrid.h"
#include "Game.h"
#include "Draw.h"
#include "RenderSurface.h"
#include "UtilFactory.h"
#include "TextFactory.h"

OrthographicGrid::OrthographicGrid(const std::string& id)
: Grid(id)
, origin_dot_(nullptr)
{
   this->create_origin_dot();
   this->create_gridlines();
   this->create_text_markers();
}

OrthographicGrid::OrthographicGrid(const std::string& id, int tile_size)
: Grid(id, tile_size)
, origin_dot_(nullptr)
{
   this->create_origin_dot();
   this->create_gridlines();
   this->create_text_markers();
}

OrthographicGrid::OrthographicGrid(const std::string& id, const sf::Vector2f& tile_size)
: Grid(id, tile_size)
, origin_dot_(nullptr)
{
   this->create_origin_dot();
   this->create_gridlines();
   this->create_text_markers();
}

OrthographicGrid::~OrthographicGrid() {
   delete this->origin_dot_;
   this->clear_gridlines();
   this->clear_text_markers();
}

void OrthographicGrid::origin(const sf::Vector2f& origin) {
   this->origin_.x = origin.x;
   this->origin_.y = origin.y;

   // update origin dot position
   this->origin_dot_->set_position(this->origin_);

   // update origin mod
   this->origin_mod_.x = (int)this->origin_.x % this->tile_width_;
   this->origin_mod_.y = (int)this->origin_.y % this->tile_height_;

   // update gridline positions
   this->clear_gridlines();
   this->create_gridlines();
}

const sf::Vector2f& OrthographicGrid::origin() {
   return Grid::origin();
}

void OrthographicGrid::tile_width(int width) {
   Grid::tile_width(width);

   this->clear_gridlines();
   this->create_gridlines();

   this->clear_text_markers();
   this->create_text_markers();
}

int OrthographicGrid::tile_width() {
   return this->tile_width_;
}

void OrthographicGrid::tile_height(int height) {
   Grid::tile_height(height);

   this->clear_gridlines();
   this->create_gridlines();

   this->clear_text_markers();
   this->create_text_markers();
}

int OrthographicGrid::tile_height() {
   return this->tile_height_;
}

sf::Vector2f OrthographicGrid::coord_to_screen(const sf::Vector2f& coord) {
   return sf::Vector2f(coord.x * this->tile_width(), coord.y * this->tile_height());
}

void OrthographicGrid::size(sf::Vector2f size) {
   Grid::size(size);
   
   this->clear_gridlines();
   this->create_gridlines();

   this->clear_text_markers();
   this->create_text_markers();
}

void OrthographicGrid::move(const sf::Vector2f& delta) {
   this->pan_delta_ += delta * this->scale_factor_;

   this->clear_gridlines();
   this->create_gridlines();

   this->clear_text_markers();
   this->create_text_markers();
}

void OrthographicGrid::set_scale(float factor) {
   this->scale_factor_ = factor;

   this->clear_gridlines();
   this->create_gridlines();

   this->clear_text_markers();
   this->create_text_markers();
}

void OrthographicGrid::set_position(const sf::Vector2f& pos) {
   this->pan_delta_ = pos;

   this->clear_gridlines();
   this->create_gridlines();

   this->clear_text_markers();
   this->create_text_markers();
}

sf::Vector2f OrthographicGrid::floor(const sf::Vector2f& pos) {
   return this->origin_mod_ + sf::Vector2f(
      std::floor(pos.x / this->tile_width()) * this->tile_width(),
      std::floor(pos.y / this->tile_height()) * this->tile_height()
   );
}

sf::Vector2f OrthographicGrid::round(const sf::Vector2f& pos) {
   return this->origin_mod_ + sf::Vector2f(
      std::round(pos.x / this->tile_width()) * this->tile_width(),
      std::round(pos.y / this->tile_height()) * this->tile_height()
   );
}

sf::Vector2f OrthographicGrid::ceil(const sf::Vector2f& pos) {
   return this->origin_mod_ + sf::Vector2f(
      std::ceil(pos.x / this->tile_width()) * this->tile_width(),
      std::ceil(pos.y / this->tile_height()) * this->tile_height()
   );
}

std::string OrthographicGrid::serialize(Serializer& s) {
   Serializer::SerialData data;
   
   data["type"] = "grid";
   data["class"] = "OrthographicGrid";
   data["id"] = this->id();
   data["tile_width"] = std::to_string(this->tile_width());
   data["tile_height"] = std::to_string(this->tile_height());
   data["origin_x"] = std::to_string((int)this->origin().x);
   data["origin_y"] = std::to_string((int)this->origin().y);

   return s.serialize(data);
}

void OrthographicGrid::deserialize(Serializer& s, Scene& scene, std::string& d) {
   Serializer::SerialData data = s.deserialize(scene, d);

   assert(data["type"] == "grid");
   assert(data["class"] == "OrthographicGrid");

   this->id_ = data["id"];
   this->tile_width(std::stoi(data["tile_width"]));
   this->tile_height(std::stoi(data["tile_height"]));
   this->origin(sf::Vector2f(std::stod(data["origin_x"]), std::stod(data["origin_y"])));

   Game::logger().msg(this->id_, Logger::INFO, "tile width: " + std::to_string(this->tile_width()) + ", tile_height: " + std::to_string(this->tile_height()));
}

void OrthographicGrid::create_origin_dot() {
   this->origin_dot_ = new SpriteGraphic();
   this->origin_dot_->set_size(3, 3);
   this->origin_dot_->set_color(sf::Color(255, 157, 75, 255));
   this->origin_dot_->set_position(this->origin_);
}

void OrthographicGrid::create_gridlines() {
   sf::Vector2f screen_start = (this->size_ / 2.f) - (this->size() / 2.f) + this->pan_delta_;
   sf::Vector2f start_pos = this->round(screen_start);

   for (int col_pos = 0; col_pos <= this->size().x; col_pos += this->tile_width()) {
      Graphic* col = new SpriteGraphic();
      col->set_size(1, this->size().y);
      col->set_position(col_pos + start_pos.x, screen_start.y);
      col->set_color(sf::Color(230, 230, 230, 90));

      this->grid_cols_.push_back(col);
   }

   for (int row_pos = 0; row_pos <= this->size().y; row_pos += this->tile_height()) {
      Graphic* row = new SpriteGraphic();
      row->set_size(this->size().x, 1);
      row->set_position(screen_start.x, row_pos + start_pos.y);
      row->set_color(sf::Color(230, 230, 230, 90));

      this->grid_rows_.push_back(row);
   }
}

void OrthographicGrid::clear_gridlines() {
   GridlineList::iterator it;
   for (it = this->grid_cols_.begin(); it != this->grid_cols_.end(); ++it) {
      delete *it;
   }
   this->grid_cols_.clear();

   for (it = this->grid_rows_.begin(); it != this->grid_rows_.end(); ++it) {
      delete *it;
   }
   this->grid_rows_.clear();
}

void OrthographicGrid::create_text_markers() {
   int text_interval = this->tile_width() * 8;

   sf::Vector2f pos;
   sf::Vector2f screen_start = (this->size_ / 2.f) - (this->size() / 2.f) + this->pan_delta_;
   sf::Vector2f start_pos(
      std::ceil(screen_start.x / (8 * this->tile_width())) * 8 * this->tile_width(),
      std::ceil(screen_start.y / (8 * this->tile_height())) * 8 * this->tile_height()
   );
   for (int col_pos = -text_interval; col_pos <= this->size().x; col_pos += text_interval) {
      for (int row_pos = -text_interval; row_pos <= this->size().y; row_pos += text_interval) {
         pos.x = col_pos + start_pos.x;
         pos.y = row_pos + start_pos.y;

         text_markers_.push_back(TextFactory::inst()->create_text(
            std::to_string((int)pos.x) + ", " + std::to_string((int)pos.y),
            this->font_,
            pos + sf::Vector2f(4, 4),
            8,
            TextFactory::ALIGN::LEFT,
            sf::Color(193, 193, 193, 255)
         ));
      }
   }
}

void OrthographicGrid::clear_text_markers() {
   TextMarkerList::const_iterator it;
   for (it = this->text_markers_.begin(); it != this->text_markers_.end(); ++it) {
      delete *it;
   }
   this->text_markers_.clear();
}

bool OrthographicGrid::intersects(const sf::Vector2i& other) { return true; }
bool OrthographicGrid::intersects(const sf::Vector2f& other) { return true; }
bool OrthographicGrid::intersects(const sf::FloatRect& other) { return true; }
bool OrthographicGrid::intersects(const SceneObject& other) { return true; }

void OrthographicGrid::accept(SceneGraphVisitor& visitor) {
   visitor.visit(this);
}

void OrthographicGrid::do_draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   GridlineList::const_iterator it;
   for (it = this->grid_cols_.begin(); it != this->grid_cols_.end(); ++it) {
      (*it)->draw(surface, render_states);
   }

   for (it = this->grid_rows_.begin(); it != this->grid_rows_.end(); ++it) {
      (*it)->draw(surface, render_states);
   }

   this->origin_dot_->draw(surface, render_states);

   TextMarkerList::const_iterator tm_it;
   for (tm_it = this->text_markers_.begin(); tm_it != this->text_markers_.end(); ++tm_it) {
      (*tm_it)->draw(surface, render_states);
   }
}
