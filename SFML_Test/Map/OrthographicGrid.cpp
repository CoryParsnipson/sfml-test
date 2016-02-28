#include "OrthographicGrid.h"

#include "Graphic.h"
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

sf::Vector2f OrthographicGrid::coord_to_screen(const sf::Vector2f& coord) {
   return sf::Vector2f(coord.x * this->tile_width(), coord.y * this->tile_height());
}

void OrthographicGrid::move(const sf::Vector2f& delta) {
   this->pan_delta_ += delta;   

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

void OrthographicGrid::draw(Graphics& graphics) {
   GridlineList::const_iterator it;
   for (it = this->grid_cols_.begin(); it != this->grid_cols_.end(); ++it) {
      (*it)->draw(graphics);
   }

   for (it = this->grid_rows_.begin(); it != this->grid_rows_.end(); ++it) {
      (*it)->draw(graphics);
   }

   this->origin_dot_->draw(graphics);

   TextMarkerList::const_iterator tm_it;
   for (tm_it = this->text_markers_.begin(); tm_it != this->text_markers_.end(); ++tm_it) {
      (*tm_it)->draw(graphics);
   }
}

void OrthographicGrid::layer(Layer* layer) {
   Draw::layer(layer);

   this->origin_dot_->layer(layer);

   GridlineList::const_iterator it;
   for (it = this->grid_cols_.begin(); it != this->grid_cols_.end(); ++it) {
      (*it)->layer(layer);
   }

   for (it = this->grid_rows_.begin(); it != this->grid_rows_.end(); ++it) {
      (*it)->layer(layer);
   }

   TextMarkerList::const_iterator tm_it;
   for (tm_it = this->text_markers_.begin(); tm_it != this->text_markers_.end(); ++tm_it) {
      (*tm_it)->layer(layer); 
   }
}

Layer* OrthographicGrid::layer() {
   return Draw::layer();
}

void OrthographicGrid::create_origin_dot() {
   this->origin_dot_ = new Shape(new sf::RectangleShape(sf::Vector2f(3, 3)));
   this->origin_dot_->set_fill_color(sf::Color(255, 157, 75, 255));
   this->origin_dot_->set_position(this->origin_);
   this->origin_dot_->layer(this->layer());
}

void OrthographicGrid::create_gridlines() {
   int cur_width = Settings::Instance()->cur_width() * this->scale_factor_;
   int cur_height = Settings::Instance()->cur_height() * this->scale_factor_;

   sf::Vector2f center(Settings::Instance()->cur_width() / 2.f, Settings::Instance()->cur_height() / 2.f);
   sf::Vector2f screen_start = center - sf::Vector2f(cur_width / 2.f, cur_height / 2.f) + this->pan_delta_;
   sf::Vector2f start_pos = this->round(screen_start);

   for (int col_pos = 0; col_pos <= cur_width; col_pos += this->tile_width()) {
      Graphic* col = new Shape(new sf::RectangleShape(sf::Vector2f(1, cur_height)));
      col->set_position(col_pos + start_pos.x, screen_start.y);
      col->set_fill_color(sf::Color(230, 230, 230, 90));
      col->layer(this->layer());

      this->grid_cols_.push_back(col);
   }

   for (int row_pos = 0; row_pos <= cur_height; row_pos += this->tile_height()) {
      Graphic* row = new Shape(new sf::RectangleShape(sf::Vector2f(cur_width, 1)));
      row->set_position(screen_start.x, row_pos + start_pos.y);
      row->set_fill_color(sf::Color(230, 230, 230, 90));
      row->layer(this->layer());

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
   int cur_width = Settings::Instance()->cur_width() * this->scale_factor_;
   int cur_height = Settings::Instance()->cur_height() * this->scale_factor_;
   int text_interval = this->tile_width() * 8;
   
   sf::Vector2f pos;
   sf::Vector2f center(Settings::Instance()->cur_width() / 2.f, Settings::Instance()->cur_height() / 2.f);
   sf::Vector2f screen_start = center - sf::Vector2f(cur_width / 2.f, cur_height / 2.f) + this->pan_delta_;
   sf::Vector2f start_pos(
      std::round(screen_start.x / (8 * this->tile_width())) * 8 * this->tile_width(),
      std::round(screen_start.y / (8 * this->tile_height())) * 8 * this->tile_height()
   );
   for (int col_pos = 0; col_pos <= cur_width; col_pos += text_interval) {
      for (int row_pos = 0; row_pos <= cur_height; row_pos += text_interval) {
         pos.x = col_pos + start_pos.x;
         pos.y = row_pos + start_pos.y;

         text_markers_.push_back(TextFactory::inst()->create_text(
            std::to_string((int)pos.x) + ", " + std::to_string((int)pos.y),
            "retro",
            this->layer(),
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
