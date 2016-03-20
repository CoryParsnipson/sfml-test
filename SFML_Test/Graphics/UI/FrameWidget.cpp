#include "FrameWidget.h"

FrameWidget::FrameWidget(const sf::Vector2f& size, Widget* parent)
: Widget(parent)
{
   // load/create border
   //this->border_["top_right"] = ;
}

FrameWidget::~FrameWidget() {
   FrameWidget::BorderList::iterator it;
   for (it = this->border_.begin(); it != this->border_.end(); ++it) {
      delete it->second;
      it->second = nullptr;
   }

   delete this->box_;
   this->box_ = nullptr;
}

void FrameWidget::draw(Graphics& graphics) {

   // draw children on top of this widget
   Widget::draw(graphics);
}

void FrameWidget::update(Game& game, Scene* scene, Entity* entity) {

}
