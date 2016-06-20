#include "ButtonWidget.h"

ButtonWidget::ButtonWidget(const sf::Vector2f& pos, const sf::Vector2f& size, Widget* parent /* = nullptr */)
: Widget()
, background_(new SpriteGraphic())
{
   this->background_->set_position(pos);
   this->background_->set_size(size);
   this->background_->set_color(sf::Color::Red);
}

ButtonWidget::~ButtonWidget() {
   delete this->background_;
}

void ButtonWidget::set_background(Graphic* background) {
   Graphic* old_background = this->background_;

   this->background_ = background;
   this->background_->set_position(old_background->get_position());

   delete old_background;
}

const sf::Vector2f& ButtonWidget::get_position() {
   return this->background_->get_position();
}

void ButtonWidget::set_position(const sf::Vector2f& pos) {
   this->background_->set_position(pos);
}

sf::Vector2f ButtonWidget::get_size() {
   return this->background_->get_size();
}

void ButtonWidget::set_size(const sf::Vector2f& size) {
   this->background_->set_size(size);
}

bool ButtonWidget::intersects(const sf::Vector2f& other) {
   return this->background_->get_global_bounds().contains(other);
}

void ButtonWidget::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   this->background_->draw(surface, render_states);

   // draw children on top of this widget
   Widget::draw(surface, render_states);
}

void ButtonWidget::drag(MouseButton button, sf::Vector2f pos, sf::Vector2f delta) {
}

float ButtonWidget::get_scale() {
   return 1.0f;
}

void ButtonWidget::set_scale(float factor) {
}

void ButtonWidget::click(MouseButton button, MouseButtonState state, sf::Vector2f pos) {
}
