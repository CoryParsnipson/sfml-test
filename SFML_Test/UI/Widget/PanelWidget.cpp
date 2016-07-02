#include "PanelWidget.h"
#include "RenderSurface.h"
#include "TextureManager.h"

PanelWidget::PanelWidget(std::string id, const sf::Vector2f& pos, const sf::Vector2f& size, bool draggable, bool resizable)
: Widget(id)
, clicked_(false)
, resized_(false)
, draggable_(draggable)
, resizable_(resizable)
, panel_(new SpriteGraphic())
, resize_handle_(nullptr)
, surface_(pos + size)
{
   this->set_size(size);

   this->transform_.translate(pos);

   this->panel_->set_position(pos);
   this->panel_->set_color(sf::Color(192, 192, 192, 255));

   // change render texture offset
   this->surface_.offset(pos);

   if (this->resizable_) {
      this->resize_handle_ = new SpriteGraphic(TextureManager::inst()->get_texture("ui_resize_handle"));
      this->resize_handle_->scale(2.0f, 2.0f);
      this->resize_handle_->set_position(pos + size - this->resize_handle_->get_size());
   }
}

PanelWidget::~PanelWidget() {
   delete this->panel_;
   delete this->resize_handle_;
}

const sf::Vector2f& PanelWidget::get_position() {
   return this->panel_->get_position();
}

void PanelWidget::set_position(const sf::Vector2f& pos) {
   // update scene graph node transform info
   this->transform_.translate(-1.f * this->panel_->get_position());
   this->transform_.translate(pos);

   this->panel_->set_position(pos);
   if (this->resize_handle_) {
      this->resize_handle_->set_position(this->panel_->get_position() + this->panel_->get_size() - this->resize_handle_->get_size());
   }
   
   // change render texture offset
   this->surface_.offset(pos);
}

sf::Vector2f PanelWidget::get_size() {
   return this->panel_->get_size();
}

void PanelWidget::set_size(const sf::Vector2f& size) {
   sf::Vector2f effective_size(std::max(size.x, this->MIN_PANEL_WIDTH), std::max(size.y, this->MIN_PANEL_HEIGHT));

   this->panel_->set_size(effective_size);
   if (this->resize_handle_) {
      this->resize_handle_->set_position(this->panel_->get_position() + effective_size - this->resize_handle_->get_size());
   }

   // make sure the render texture is big enough
   this->surface_.bounds(this->get_position(), effective_size);
}

bool PanelWidget::intersects(const sf::Vector2f& other) {
   sf::Transform transform = this->transform();
   return transform.transformRect(this->panel_->get_local_bounds()).contains(other);
}

void PanelWidget::on_click() {
   // TODO: implement me
}

void PanelWidget::on_release() {
   this->resized_ = false;
   this->clicked_ = false;
}

void PanelWidget::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   this->surface_.clear();
   this->panel_->draw(this->surface_, render_states);

   if (this->resize_handle_) {
      this->resize_handle_->draw(this->surface_, render_states);
   }

   // draw children on top of this widget
   Widget::draw(this->surface_, render_states);

   // draw to intermediate surface
   this->surface_.update();

   // take intermediate surface and draw it to the provided surface
   sf::Sprite widget_sprite(this->surface_.get_texture(), static_cast<sf::IntRect>(this->panel_->get_local_bounds()));
   widget_sprite.setPosition(this->panel_->get_position());

   surface.draw(widget_sprite, render_states);
}

void PanelWidget::drag(MouseButton button, sf::Vector2f pos, sf::Vector2f delta) {
   if (this->draggable_ && this->clicked_) {
      this->move(delta);
   }

   if (this->resizable_ && this->resized_) {
      this->set_size(this->panel_->get_size() + delta);
   }
}

float PanelWidget::get_scale() { return 1.0; }
void PanelWidget::set_scale(float factor) {}

void PanelWidget::click(MouseButton button, ButtonState state, sf::Vector2f pos) {
   if (button == MouseButton::Left && state == ButtonState::Pressed) {
      sf::FloatRect rh_bounds(this->resize_handle_->get_local_bounds());
      rh_bounds.left += this->get_size().x - rh_bounds.width;
      rh_bounds.top += this->get_size().y - rh_bounds.height;

      this->resized_ = this->resize_handle_ && this->transform().transformRect(rh_bounds).contains(pos);
      this->clicked_ = !this->resized_ && this->intersects(pos);
   } else {
      this->resized_ = false;
      this->clicked_ = false;
   }
}
