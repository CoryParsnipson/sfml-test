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
