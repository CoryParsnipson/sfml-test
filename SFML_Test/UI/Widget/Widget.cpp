#include "Widget.h"

// ----------------------------------------------------------------------------
// free function helper methods
// ----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& stream, const Widget& widget) {
   stream << widget.to_string();
   return stream;
}

// ----------------------------------------------------------------------------
// Widget constructor implementations
// ----------------------------------------------------------------------------
Widget::Widget(std::string id /* = "Widget" */)
: id_(std::move(id))
, positioning_(UI::Positioning::Default)
{}

Widget::~Widget() {
}

// ----------------------------------------------------------------------------
// Widget method implementations
// ----------------------------------------------------------------------------
void Widget::accept(SceneGraphVisitor& visitor) {
   visitor.visit(this);
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

Widget::operator std::string() const {
   return this->to_string();
}

std::string Widget::to_string() const {
   return "[Widget: " + this->id_ + "]";
}
