#include "WidgetSceneGraphNode.h"
#include "Widget.h"

WidgetSceneGraphNode::WidgetSceneGraphNode(Widget& w, bool visible /* = true */)
: SceneGraphNode(visible)
, widget_(&w)
{
}

Widget* WidgetSceneGraphNode::get_widget() {
   return this->widget_;
}

void WidgetSceneGraphNode::accept(SceneGraphVisitor& visitor) {
   visitor.visit(this->widget_);
}

void WidgetSceneGraphNode::pre_draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default) {
   this->widget_->draw(surface, render_states);
}

void WidgetSceneGraphNode::post_update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr) {
   this->widget_->update(game, scene, entity);
}
