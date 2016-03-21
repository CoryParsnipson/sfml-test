#include "PanelWidget.h"

PanelWidget::PanelWidget(const sf::Vector2f& pos, const sf::Vector2f& size, Widget* parent)
: Widget(parent)
, box_(new Shape(new sf::RectangleShape()))
{
   // load/create border
   //Graphic* tr_one = new Shape(new sf::RectangleShape());
   //tr_one->set_size()

   //this->border_["top_right"] = ;
   this->box_->set_position(pos);
   this->box_->set_size(size);
   this->box_->set_fill_color(sf::Color(70, 69, 88, 255));
   this->box_->set_outline_thickness(1.0);
   this->box_->set_outline_color(sf::Color(120, 121, 137, 255));
}

PanelWidget::~PanelWidget() {
   PanelWidget::BorderList::iterator it;
   for (it = this->border_.begin(); it != this->border_.end(); ++it) {
      delete it->second;
      it->second = nullptr;
   }

   delete this->box_;
   this->box_ = nullptr;
}

void PanelWidget::draw(Graphics& graphics) {
   this->box_->draw(graphics);

   // draw children on top of this widget
   Widget::draw(graphics);
}

void PanelWidget::update(Game& game, Scene* scene, Entity* entity) {

}

void PanelWidget::drag(MouseButtonCommand& c, sf::Vector2f delta) {
   sf::Vector2f mouse_pos(c.x, c.y);
   if (this->clicked_) {
      this->box_->move(-1.0f * delta);
   }
}

float PanelWidget::get_scale() { return 1.0; }
void PanelWidget::set_scale(float factor) {}

void PanelWidget::click(MouseButtonCommand& c) {
   sf::Vector2f mouse_pos(c.x, c.y);
   if (this->box_->get_global_bounds().contains(mouse_pos) &&
       c.button == MouseButtonCommand::MOUSE_BUTTON::LEFT &&
       c.state == MouseButtonCommand::STATE::PRESSED) {
          this->clicked_ = true;
   } else {
      this->clicked_ = false;
   }
}
