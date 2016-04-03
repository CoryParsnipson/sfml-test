#ifndef WIDGET_H
#define WIDGET_H

#include "dependencies.h"
#include "Draw.h"
#include "Update.h"

class Widget;

class Widget
: public Draw
, public Update
{
public:
   typedef std::vector<Widget*> WidgetList;
   enum Position {
      ABSOLUTE,
      RELATIVE,
      FIXED
   };

   Widget();
   virtual ~Widget();

   // widget interface
   void set_positioning(Widget::Position position) { this->position_ = position; }
   Widget::Position get_positioning() { return this->position_; }

   virtual const sf::Vector2f& get_position() = 0;
   virtual void set_position(const sf::Vector2f& pos) {
      sf::Vector2f delta = pos - this->get_position();
      this->move(delta);
   }
   virtual void move(const sf::Vector2f& delta) {
      WidgetList::const_iterator it;
      for (it = this->children_.begin(); it != this->children_.end(); ++it) {
         if ((*it)->get_positioning() == Widget::Position::FIXED) {
            continue;
         }
         (*it)->move(delta);
      }
   }

   void add(Widget* widget) {
      if (!widget) {
         return;
      }

      if (widget->get_positioning() == Widget::Position::RELATIVE) {
         widget->move(this->get_position());
      }

      this->children_.push_back(widget);
   }

   Widget* get(unsigned int idx) {
      if (idx >= this->children_.size()) {
         Service::get_logger().msg("Widget", Logger::WARNING, "Index (" + std::to_string(idx) + ") out of bounds when trying to get child widget.");
         return nullptr;
      }

      return this->children_[idx];
   }

   void remove(Widget* child) {
      WidgetList::const_iterator it;
      for (it = this->children_.begin(); it != this->children_.end(); ++it) {
         if (*it == child) {
            this->children_.erase(it);
            return;
         }
      }
   }

   // draw interface
   virtual void draw(Graphics& graphics) {
      WidgetList::const_iterator it;
      for (it = this->children_.begin(); it != this->children_.end(); ++it) {
         if (!*it) {
            this->children_.erase(it);
            continue;
         }
         (*it)->draw(graphics);
      }
   }

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr) {
      WidgetList::const_iterator it;
      for (it = this->children_.begin(); it != this->children_.end(); ++it) {
         if (!*it) {
            this->children_.erase(it);
            continue;
         }
         (*it)->update(game, scene, entity);
      }
   }

protected:
   Position position_;
   WidgetList children_;
};

#endif
