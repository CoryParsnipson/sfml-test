#include "DrawableSceneGraphNode.h"

DrawableSceneGraphNode::DrawableSceneGraphNode(Draw& d, sf::RenderStates state /* = sf::RenderStates::Default */, bool visible /* = true */)
: SceneGraphNode(state, visible)
, drawable_(&d)
{
}

void DrawableSceneGraphNode::pre_draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   this->drawable_->draw(surface, render_states);
}
