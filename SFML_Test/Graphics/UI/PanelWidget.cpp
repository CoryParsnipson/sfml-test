#include "PanelWidget.h"
#include "TextureManager.h"

PanelWidget::PanelWidget(const sf::Vector2f& pos, const sf::Vector2f& size, Widget* parent, bool draggable, bool resizable)
: Widget(parent)
, clicked_(false)
, resized_(false)
, draggable_(draggable)
, resizable_(resizable)
, panel_(new Shape(new sf::RectangleShape()))
, resize_handle_(nullptr)
{
   this->panel_->set_position(pos);
   this->panel_->set_size(size);
   this->panel_->set_fill_color(sf::Color(192, 192, 192, 255));

   if (this->resizable_) {
      Graphic::GraphicList* rh_gl = new Graphic::GraphicList();
      rh_gl->push_back(new Sprite(TextureManager::inst()->get_texture("ui_resize_handle")));
      (*rh_gl)[0]->scale(2.0f, 2.0f);
      rh_gl->push_back(new Shape(new sf::RectangleShape()));
      (*rh_gl)[1]->set_size(2.0f * (*rh_gl)[0]->get_size());
      (*rh_gl)[1]->set_fill_color(sf::Color::Transparent);
      (*rh_gl)[1]->set_outline_color(sf::Color::Red);
      (*rh_gl)[1]->set_outline_thickness(1.0f);

      this->resize_handle_ = new CompositeGraphic(rh_gl);
      this->resize_handle_->set_position(pos + size - this->resize_handle_->get_size());
   }
}

PanelWidget::~PanelWidget() {
   delete this->panel_;
   delete this->resize_handle_;
}

void PanelWidget::draw(Graphics& graphics) {
   this->panel_->draw(graphics);

   if (this->resize_handle_) {
      this->resize_handle_->draw(graphics);
   }

   // draw children on top of this widget
   Widget::draw(graphics);
}

void PanelWidget::update(Game& game, Scene* scene, Entity* entity) {
}

void PanelWidget::drag(MouseButtonCommand& c, sf::Vector2f delta) {
   sf::Vector2f mouse_pos(c.x, c.y);

   if (this->draggable_ && this->clicked_) {
      if (this->resize_handle_) {
         this->resize_handle_->move(delta);
      }

      this->panel_->move(delta);
   } else if (this->resizable_ && this->resized_) {
      sf::Vector2f size = this->panel_->get_size();
      size.x = std::max(size.x + delta.x, this->MIN_PANEL_WIDTH);
      size.y = std::max(size.y + delta.y, this->MIN_PANEL_HEIGHT);

      this->panel_->set_size(size);
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
