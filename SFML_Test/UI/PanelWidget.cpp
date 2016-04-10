#include "PanelWidget.h"
#include "RenderSurface.h"
#include "TextureManager.h"

PanelWidget::PanelWidget(const sf::Vector2f& pos, const sf::Vector2f& size, Widget* parent, bool draggable, bool resizable)
: Widget()
, clicked_(false)
, resized_(false)
, draggable_(draggable)
, resizable_(resizable)
, panel_(new Shape(new sf::RectangleShape()))
, resize_handle_(nullptr)
{
   this->set_size(size);

   this->panel_->set_position(pos);
   this->panel_->set_fill_color(sf::Color(192, 192, 192, 255));

   if (this->resizable_) {
      this->resize_handle_ = new Sprite(TextureManager::inst()->get_texture("ui_resize_handle"));
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
   this->panel_->set_position(pos);
   if (this->resize_handle_) {
      this->resize_handle_->set_position(this->panel_->get_position() + this->panel_->get_size() - this->resize_handle_->get_size());
   }

   Widget::set_position(pos);
}

void PanelWidget::move(const sf::Vector2f& delta) {
   this->panel_->move(delta);
   if (this->resize_handle_) {
      this->resize_handle_->move(delta);
   }

   Widget::move(delta);
}

sf::Vector2f PanelWidget::get_size() {
   return this->panel_->get_size();
}

void PanelWidget::set_size(const sf::Vector2f& size) {
   this->panel_->set_size(size);

   WidgetList::const_iterator it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      (*it)->set_size(size);
   }
}

void PanelWidget::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   //this->panel_->draw(this->surface_, render_states);
   this->panel_->draw(surface, render_states);

   if (this->resize_handle_) {
      //this->resize_handle_->draw(this->surface_, render_states);
      this->resize_handle_->draw(surface, render_states);
   }

   // draw children on top of this widget
   //Widget::draw(this->surface_, render_states);
   Widget::draw(surface, render_states);

   // finalize intermediate surface
   //this->surface_.update();

   // take intermediate surface and draw it to the provided surface
   //sf::IntRect panel_area(this->panel_->get_global_bounds());
   //sf::Sprite panel_sprite(this->surface_.get_texture());
   //surface.draw(panel_sprite, render_states);
}

void PanelWidget::drag(MouseButtonCommand& c, sf::Vector2f delta) {
   sf::Vector2f mouse_pos(c.x, c.y);

   if (this->draggable_ && this->clicked_) {
      this->move(delta);
   } else if (this->resizable_ && this->resized_) {
      sf::Vector2f size = this->panel_->get_size();
      size.x = std::max(size.x + delta.x, this->MIN_PANEL_WIDTH);
      size.y = std::max(size.y + delta.y, this->MIN_PANEL_HEIGHT);

      this->set_size(size);
      this->resize_handle_->set_position(this->panel_->get_position() + size - this->resize_handle_->get_size());
   }
}

float PanelWidget::get_scale() { return 1.0; }
void PanelWidget::set_scale(float factor) {}

void PanelWidget::click(MouseButtonCommand& c) {
   sf::Vector2f mouse_pos(c.x, c.y);
   if (c.button == MouseButtonCommand::MOUSE_BUTTON::LEFT &&
       c.state == MouseButtonCommand::STATE::PRESSED) {
      this->resized_ = this->resize_handle_ && this->resize_handle_->get_global_bounds().contains(mouse_pos);
      this->clicked_ = !this->resized_ && this->panel_->get_global_bounds().contains(mouse_pos);
   } else {
      this->clicked_ = false;
   }
}