#include "Widget.h"

Widget::Widget()
: parent_(nullptr)
, positioning_(UI::Positioning::Default)
{}

Widget::~Widget() {
   // remove itself from parent
   if (this->parent_) {
      this->parent_->remove(this);
   }

   // delete all children (Widget can only have 1 parent)
   iterator<CompositeList> it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      delete *it;
   }
   this->children_.clear();
}

void Widget::add_post(Widget* child) {
   if (child && child->get_parent()) {
      child->get_parent()->remove(child);
   }
   child->set_parent(this);
}

void Widget::remove_post(Widget* child) {
   assert(child);
   child->set_parent(nullptr);
}

void Widget::set_positioning(UI::Positioning positioning) {
   this->positioning_ = positioning;
}

UI::Positioning Widget::get_positioning() {
   return this->positioning_;
}

void Widget::move(const sf::Vector2f& delta) {
   this->set_position(this->get_position() + delta);
}

void Widget::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   sf::RenderStates widget_state = render_states;
   widget_state.transform.translate(this->get_position());

   const_iterator<CompositeList> it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      (*it)->draw(surface, ((*it)->get_positioning() == UI::Positioning::Default ? widget_state : render_states));
   }
}

void Widget::update(Game& game, Scene* scene, Entity* entity) {
   const_iterator<CompositeList> it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      (*it)->update(game, scene, entity);
   }
}

void Widget::set_parent(Widget* parent) {
   this->parent_ = parent;
}

Widget* Widget::get_parent() {
   return this->parent_;
}
