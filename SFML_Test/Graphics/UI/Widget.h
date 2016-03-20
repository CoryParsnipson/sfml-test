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

   Widget(Widget* parent = nullptr)
   : parent_(parent)
   {
   }

   virtual ~Widget() {
      WidgetList::const_iterator it;
      for (it = this->children_.begin(); it != this->children_.end(); ++it) {
         delete *it;
      }
      this->children_.clear();
   }

   // draw interface
   virtual void draw(Graphics& graphics) {
      WidgetList::const_iterator it;
      for (it = this->children_.begin(); it != this->children_.end(); ++it) {
         (*it)->draw(graphics);
      }
   }

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr) = 0;

protected:
   Widget* parent_;
   WidgetList children_;
};

#endif
