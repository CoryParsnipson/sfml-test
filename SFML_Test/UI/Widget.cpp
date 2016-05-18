#include "Widget.h"

Widget::Widget()
: position_(Widget::Position::RELATIVE)
{}

Widget::~Widget() {
}

void Widget::set_positioning(Widget::Position position) {
   this->position_ = position;
}

Widget::Position Widget::get_positioning() {
   return this->position_;
}

void Widget::set_position(const sf::Vector2f& pos) {
   sf::Vector2f delta = pos - this->get_position();
   this->move(delta);
}

void Widget::move(const sf::Vector2f& delta) {
   const_iterator<CompositeList> it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      if ((*it)->get_positioning() == Widget::RELATIVE) {
         continue;
      }
      (*it)->move(-delta);
   }
}

void Widget::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   sf::RenderStates widget_state = sf::RenderStates::Default;
   widget_state.transform.translate(this->get_position());

   const_iterator<CompositeList> it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      if ((*it)->get_positioning() == Widget::RELATIVE) {
         render_states.transform *= widget_state.transform;
      }
      (*it)->draw(surface, render_states);
   }
}

void Widget::update(Game& game, Scene* scene, Entity* entity) {
   const_iterator<CompositeList> it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      (*it)->update(game, scene, entity);
   }
}
