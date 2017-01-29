#include "ButtonWidget.h"
#include "RenderSurface.h"

ButtonWidget::ButtonWidget(std::string id, const sf::Vector2f& pos, const sf::Vector2f& size, Command* action /* = nullptr */, Widget* parent /* = nullptr */)
: Widget(id)
, background_(new SpriteGraphic())
{
   this->clickable(true);

   this->background_->set_position(pos);
   this->background_->set_size(size);
   this->background_->set_color(sf::Color::Red);

   this->on_left_click(action);
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

bool ButtonWidget::intersects(const sf::Vector2i& other) {
   return this->intersects(static_cast<sf::Vector2f>(other));
}

bool ButtonWidget::intersects(const sf::Vector2f& other) {
   sf::Transform transform = this->global_transform();
   return transform.transformRect(this->background_->get_global_bounds()).contains(other);
}

bool ButtonWidget::intersects(const sf::FloatRect& other) {
   sf::Transform transform = this->global_transform();
   return transform.transformRect(this->background_->get_global_bounds()).intersects(other);
}

bool ButtonWidget::intersects(const SceneObject& other) {
   // TODO: implement me
   return false;
}

void ButtonWidget::on_mouse_in() {
   this->background_->set_color(sf::Color(255, 255, 255, 127));
}

void ButtonWidget::on_mouse_out() {
   this->background_->set_color(sf::Color(255, 255, 255, 255));
}

void ButtonWidget::do_draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   this->background_->draw(surface, render_states);
}

std::string ButtonWidget::serialize(Serializer& s) {
   // TODO: implement me
   return "";
}

void ButtonWidget::deserialize(Serializer& s, Scene& scene, std::string& d) {
   // TODO: implement me
}
