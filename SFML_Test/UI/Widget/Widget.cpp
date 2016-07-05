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

void Widget::on_mouse_in() {}
void Widget::on_mouse_out() {}
void Widget::on_click() {}
void Widget::on_release() {}

void Widget::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   sf::RenderStates widget_state = render_states;
   widget_state.transform.translate(this->get_position());

   Widget::iterator it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      (*it)->draw(surface, (static_cast<Widget*>(*it)->get_positioning() == UI::Positioning::Default ? widget_state : render_states));
   }
}

void Widget::update(Game& game, Scene* scene, Entity* entity) {
   Widget::iterator it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      (*it)->update(game, scene, entity);
   }
}

Widget::operator std::string() const {
   return this->to_string();
}

std::string Widget::to_string() const {
   return "[Widget: " + this->id_ + "]";
}
