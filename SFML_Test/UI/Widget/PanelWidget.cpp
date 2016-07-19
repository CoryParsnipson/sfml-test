#include "PanelWidget.h"
#include "RenderSurface.h"
#include "TextureManager.h"

PanelWidget::PanelWidget(std::string id, const sf::Vector2f& pos, const sf::Vector2f& size)
: Widget(id)
, panel_(new SpriteGraphic())
, surface_(pos + size)
{
   this->set_size(size);

   this->transform_.translate(pos);

   this->panel_->set_position(pos);
   this->panel_->set_color(sf::Color(192, 192, 192, 255));

   // change render texture offset
   this->surface_.offset(pos);
}

PanelWidget::~PanelWidget() {
   delete this->panel_;
}

const sf::Vector2f& PanelWidget::get_position() {
   return this->panel_->get_position();
}

void PanelWidget::set_position(const sf::Vector2f& pos) {
   // update scene graph node transform info
   this->transform_.translate(-1.f * this->panel_->get_position());
   this->transform_.translate(pos);

   this->panel_->set_position(pos);
   
   // change render texture offset
   this->surface_.offset(pos);
}

sf::Vector2f PanelWidget::get_size() {
   return this->panel_->get_size();
}

void PanelWidget::set_size(const sf::Vector2f& size) {
   sf::Vector2f effective_size(std::max(size.x, this->MIN_PANEL_WIDTH), std::max(size.y, this->MIN_PANEL_HEIGHT));

   this->panel_->set_size(effective_size);

   // make sure the render texture is big enough
   this->surface_.bounds(this->get_position(), effective_size);
}

bool PanelWidget::intersects(const sf::Vector2i& other) {
   return this->intersects(static_cast<sf::Vector2i>(other));
}

bool PanelWidget::intersects(const sf::Vector2f& other) {
   sf::Transform transform = this->transform();
   return transform.transformRect(this->panel_->get_local_bounds()).contains(other);
}

bool PanelWidget::intersects(const sf::FloatRect& other) {
   sf::Transform transform = this->transform();
   return transform.transformRect(this->panel_->get_local_bounds()).intersects(other);
}

bool PanelWidget::intersects(const SceneObject& other) {
   // TODO: implement me
   return false;
}

void PanelWidget::on_mouse_in() {
   this->panel_->set_outline_thickness(-2.0f);
   this->panel_->set_outline_color(sf::Color::Red);
}

void PanelWidget::on_mouse_out() {
   this->panel_->set_outline_thickness(0);
   this->panel_->set_outline_color(sf::Color::Transparent);
}

void PanelWidget::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   this->surface_.clear();
   this->panel_->draw(this->surface_);

   // draw children on top of this widget
   Widget::draw(this->surface_);

   // draw to intermediate surface
   this->surface_.update();

   // take intermediate surface and draw it to the provided surface
   sf::Sprite widget_sprite(this->surface_.get_texture(), static_cast<sf::IntRect>(this->panel_->get_local_bounds()));
   widget_sprite.setPosition(this->panel_->get_position());

   surface.draw(widget_sprite, render_states);
}
