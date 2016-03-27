#include "PanelWidget.h"
#include "TextureManager.h"

PanelWidget::PanelWidget(const sf::Vector2f& pos, const sf::Vector2f& size, Widget* parent)
: Widget(parent)
, box_(new Shape(new sf::RectangleShape()))
{
   this->border_["top_left"] = new Sprite(TextureManager::inst()->get_texture("ui_panel_test_tl"));
   this->border_["top_left"]->set_position(pos);

   this->border_["top_right"] = new Sprite(TextureManager::inst()->get_texture("ui_panel_test_tr"));
   this->border_["top_right"]->set_position(pos + sf::Vector2f(size.x - 40, 0));

   this->border_["top_middle"] = new Sprite(TextureManager::inst()->get_texture("ui_panel_test_tm"));
   this->border_["top_middle"]->set_texture_rect(sf::IntRect(0, 0, size.x - 80, 40));
   this->border_["top_middle"]->set_position(pos + sf::Vector2f(40, 0));

   this->border_["left_middle"] = new Sprite(TextureManager::inst()->get_texture("ui_panel_test_lm"));
   this->border_["left_middle"]->set_texture_rect(sf::IntRect(0, 0, 40, size.y - 80));
   this->border_["left_middle"]->set_position(pos + sf::Vector2f(0, 40));

   this->border_["right_middle"] = new Sprite(TextureManager::inst()->get_texture("ui_panel_test_rm"));
   this->border_["right_middle"]->set_texture_rect(sf::IntRect(0, 0, 40, size.y - 80));
   this->border_["right_middle"]->set_position(pos + sf::Vector2f(size.x - 40, 40));

   this->border_["bottom_left"] = new Sprite(TextureManager::inst()->get_texture("ui_panel_test_bl"));
   this->border_["bottom_left"]->set_position(pos + sf::Vector2f(0, size.y - 40));

   this->border_["bottom_right"] = new Sprite(TextureManager::inst()->get_texture("ui_panel_test_br"));
   this->border_["bottom_right"]->set_position(pos + sf::Vector2f(size.x - 40, size.y - 40));

   this->border_["bottom_middle"] = new Sprite(TextureManager::inst()->get_texture("ui_panel_test_bm"));
   this->border_["bottom_middle"]->set_texture_rect(sf::IntRect(40, size.y - 40, size.x - 80, 40));
   this->border_["bottom_middle"]->set_position(pos + sf::Vector2f(40, size.y - 40));

   this->box_->set_position(pos + sf::Vector2f(10, 10));
   this->box_->set_size(size - sf::Vector2f(20, 20));
   this->box_->set_fill_color(sf::Color(192, 192, 192, 255));
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

   this->border_["top_middle"]->draw(graphics);
   this->border_["top_left"]->draw(graphics);
   this->border_["top_right"]->draw(graphics);

   this->border_["left_middle"]->draw(graphics);
   this->border_["right_middle"]->draw(graphics);

   this->border_["bottom_middle"]->draw(graphics);
   this->border_["bottom_left"]->draw(graphics);
   this->border_["bottom_right"]->draw(graphics);

   // draw children on top of this widget
   Widget::draw(graphics);
}

void PanelWidget::update(Game& game, Scene* scene, Entity* entity) {

}

void PanelWidget::drag(MouseButtonCommand& c, sf::Vector2f delta) {
   sf::Vector2f mouse_pos(c.x, c.y);
   if (this->clicked_) {
      this->border_["top_middle"]->move(-1.0f * delta);
      this->border_["top_left"]->move(-1.0f * delta);
      this->border_["top_right"]->move(-1.0f * delta);

      this->border_["left_middle"]->move(-1.0f * delta);
      this->border_["right_middle"]->move(-1.0f * delta);

      this->border_["bottom_middle"]->move(-1.0f * delta);
      this->border_["bottom_left"]->move(-1.0f * delta);
      this->border_["bottom_right"]->move(-1.0f * delta);

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
