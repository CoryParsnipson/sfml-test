#include <memory>

#include "UpdateTileCursorCommand.h"
#include "Service.h"
#include "Grid.h"
#include "Entity.h"
#include "PhysicsPart.h"
#include "UtilFactory.h"

UpdateTileCursorCommand::UpdateTileCursorCommand(Grid& grid, Entity* tile_cursor)
: grid_(&grid)
, tile_cursor_(tile_cursor)
, click_pos_(0, 0)
, release_pos_(0, 0)
{
}

UpdateTileCursorCommand::~UpdateTileCursorCommand() {
}

void UpdateTileCursorCommand::execute() {
   if (!this->tile_cursor_) {
      return;
   }

   PhysicsPart* tcp = dynamic_cast<PhysicsPart*>(this->tile_cursor_->get("physics"));
   if (!tcp) {
      Service::get_logger().msg("UpdateTileCursorCommand", Logger::WARNING, "Tile Cursor entity does not have physics part!");
      return;
   }

   std::shared_ptr<sf::FloatRect> r(UtilFactory::inst()->create_float_rect(this->click_pos_, this->release_pos_));
   
   bool is_drag_gesture = (r->width >= this->grid_->tile_width() / 3.f ||
                           r->height >= this->grid_->tile_height() / 3.f);

   if (!is_drag_gesture && this->tile_cursor_->visible() && this->tile_cursor_->intersects(this->click_pos_)) {
      this->tile_cursor_->visible(false);
      return;
   }

   // round to nearest map grid point
   sf::Vector2f start_point(r->left, r->top);
   sf::Vector2f end_point(r->left + r->width, r->top + r->height);

   start_point = this->grid_->floor(start_point);
   end_point = this->grid_->ceil(end_point);

   // update new tile cursor rectangle
   r->left = start_point.x;
   r->top = start_point.y;

   r->width = std::max(end_point.x - r->left, (float)this->grid_->tile_width());
   r->height = std::max(end_point.y - r->top, (float)this->grid_->tile_height());

   this->tile_cursor_->set_position(r->left, r->top);
   this->tile_cursor_->set_size(r->width, r->height);
   this->tile_cursor_->visible(true);
}

void UpdateTileCursorCommand::unexecute() {
   // implement me
}

void UpdateTileCursorCommand::click_pos(sf::Vector2f pos) {
   this->click_pos(pos.x, pos.y);
}

void UpdateTileCursorCommand::click_pos(float x, float y) {
   this->click_pos_.x = x;
   this->click_pos_.y = y;
}

const sf::Vector2f& UpdateTileCursorCommand::click_pos() {
   return this->click_pos_;
}

void UpdateTileCursorCommand::release_pos(sf::Vector2f pos) {
   this->release_pos(pos.x, pos.y);
}

void UpdateTileCursorCommand::release_pos(float x, float y) {
   this->release_pos_.x = x;
   this->release_pos_.y = y;
}

const sf::Vector2f& UpdateTileCursorCommand::release_pos() {
   return this->release_pos_;
}
