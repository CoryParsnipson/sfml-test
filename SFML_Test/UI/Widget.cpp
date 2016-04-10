#include "Widget.h"

Widget::Widget()
: position_(Widget::Position::RELATIVE)
{}

Widget::~Widget() {
   WidgetList::const_iterator it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      delete *it;
   }
   this->children_.clear();
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
   WidgetList::const_iterator it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      if ((*it)->get_positioning() == Widget::Position::FIXED) {
         continue;
      }
      (*it)->move(delta);
   }
}

void Widget::add(Widget* widget) {
   if (!widget) {
      return;
   }

   if (widget->get_positioning() == Widget::Position::RELATIVE) {
      widget->move(this->get_position());
   }

   this->children_.push_back(widget);
}

Widget* Widget::get(unsigned int idx) {
   if (idx >= this->children_.size()) {
      Service::get_logger().msg("Widget", Logger::WARNING, "Index (" + std::to_string(idx) + ") out of bounds when trying to get child widget.");
      return nullptr;
   }

   return this->children_[idx];
}

void Widget::remove(Widget* child) {
   WidgetList::const_iterator it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      if (*it == child) {
         this->children_.erase(it);
         return;
      }
   }
}

void Widget::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   WidgetList::const_iterator it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      if (!*it) {
         this->children_.erase(it);
         continue;
      }
      (*it)->draw(surface, render_states);
   }
}

void Widget::update(Game& game, Scene* scene, Entity* entity) {
   WidgetList::const_iterator it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      if (!*it) {
         this->children_.erase(it);
         continue;
      }
      (*it)->update(game, scene, entity);
   }
}
