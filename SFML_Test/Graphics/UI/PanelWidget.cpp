#include "PanelWidget.h"
#include "TextureManager.h"

PanelWidget::PanelWidget(const sf::Vector2f& pos, const sf::Vector2f& size, Widget* parent, bool draggable, bool resizable)
: Widget(parent)
, clicked_(false)
, resized_(false)
, draggable_(draggable)
, resizable_(resizable)
, panel_(new Shape(new sf::RectangleShape()))
{
   this->panel_->set_position(pos);
   this->panel_->set_size(size);
   this->panel_->set_fill_color(sf::Color(192, 192, 192, 255));

   sf::Vector2f resize_handle_size(40, 40);
   sf::Vector2f resize_handle_pos = pos + size - resize_handle_size;

   this->resize_handle_ = new Shape(new sf::RectangleShape());
   this->resize_handle_->set_position(resize_handle_pos);
   this->resize_handle_->set_size(resize_handle_size);
   this->resize_handle_->set_fill_color(sf::Color::Transparent);
   this->resize_handle_->set_outline_color(sf::Color::Red);
   this->resize_handle_->set_outline_thickness(1.0f);
}

PanelWidget::~PanelWidget() {
   delete this->panel_;
   this->panel_ = nullptr;
}

void PanelWidget::draw(Graphics& graphics) {
   this->panel_->draw(graphics);
   this->resize_handle_->draw(graphics);

   // draw children on top of this widget
   Widget::draw(graphics);
}

void PanelWidget::update(Game& game, Scene* scene, Entity* entity) {

}

void PanelWidget::drag(MouseButtonCommand& c, sf::Vector2f delta) {
   sf::Vector2f mouse_pos(c.x, c.y);

   if (this->draggable_ && this->clicked_) {
      this->resize_handle_->move(delta);
      this->panel_->move(delta);
   } else if (this->resizable_ && this->resized_) {
      sf::Vector2f size = this->panel_->get_size();
      size.x = std::max(size.x + delta.x, 90.0f);
      size.y = std::max(size.y + delta.y, 90.0f);

      this->panel_->set_size(size);
      this->resize_handle_->move(delta);
   }
}

float PanelWidget::get_scale() { return 1.0; }
void PanelWidget::set_scale(float factor) {}

void PanelWidget::click(MouseButtonCommand& c) {
   sf::Vector2f mouse_pos(c.x, c.y);
   if (c.button == MouseButtonCommand::MOUSE_BUTTON::LEFT &&
       c.state == MouseButtonCommand::STATE::PRESSED) {
      this->resized_ = this->resize_handle_->get_global_bounds().contains(mouse_pos);
      this->clicked_ = !this->resized_ && this->panel_->get_global_bounds().contains(mouse_pos);
   } else {
      this->clicked_ = false;
   }
}
