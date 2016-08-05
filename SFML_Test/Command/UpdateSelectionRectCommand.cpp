#include "UpdateSelectionRectCommand.h"
#include "Entity.h"
#include "UtilFactory.h"
#include "PhysicsPart.h"

UpdateSelectionRectCommand::UpdateSelectionRectCommand(Entity* selection_rect)
: set_visible_(selection_rect ? selection_rect->visible() : true)
, selection_rect_(selection_rect)
, old_selection_rect_(selection_rect)
, click_pos_(nullptr)
, release_pos_(nullptr)
{
}

UpdateSelectionRectCommand::~UpdateSelectionRectCommand() {
   delete this->click_pos_;
   delete this->release_pos_;
}

void UpdateSelectionRectCommand::execute() {
   if (!this->selection_rect_) {
      return;
   }

   PhysicsPart* srp = dynamic_cast<PhysicsPart*>(this->selection_rect_->get("physics"));
   if (!srp) {
      Service::get_logger().msg("UpdateSelectionRectCommand", Logger::WARNING, "Selection Rectangle entity does not have physics part!");
      return;
   }

   sf::Vector2f final_p1(0, 0);
   sf::Vector2f final_p2(0, 0);

   if (this->click_pos_) {
      final_p1 = *this->click_pos_;
   }

   if (this->release_pos_) {
      final_p2 = *this->release_pos_;
   }

   // TODO: need to write copy constructor for Entity, Part, and Graphic classes
   //this->old_selection_rect_ = new Entity(this->selection_rect_);
   sf::FloatRect* r = UtilFactory::inst()->create_float_rect(final_p1, final_p2);
   
   this->selection_rect_->set_position(r->left, r->top);
   this->selection_rect_->set_size(r->width, r->height);

   this->selection_rect_->visible(this->set_visible_);

   delete r;
}

void UpdateSelectionRectCommand::unexecute() {
   // implement me
}

void UpdateSelectionRectCommand::click_pos(sf::Vector2f pos) {
   if (!this->click_pos_) {
      this->click_pos_ = new sf::Vector2f(pos);
      return;
   }
   *this->click_pos_ = pos;
}

void UpdateSelectionRectCommand::click_pos(float x, float y) {
   if (!this->click_pos_) {
      this->click_pos_ = new sf::Vector2f(x, y);
      return;
   }
   this->click_pos_->x = x;
   this->click_pos_->y = y;
}

sf::Vector2f* UpdateSelectionRectCommand::click_pos() {
   return this->click_pos_;
}

sf::Vector2f* UpdateSelectionRectCommand::release_pos() {
   return this->release_pos_;
}

void UpdateSelectionRectCommand::release_pos(sf::Vector2f pos) {
   if (!this->release_pos_) {
      this->release_pos_ = new sf::Vector2f(pos);
      return;
   }
   *this->release_pos_ = pos;
}

void UpdateSelectionRectCommand::release_pos(float x, float y) {
   if (!this->release_pos_) {
      this->release_pos_ = new sf::Vector2f(x, y);
      return;
   }
   this->release_pos_->x = x;
   this->release_pos_->y = y;
}

void UpdateSelectionRectCommand::visible(bool visible) {
   this->set_visible_ = visible;
}
