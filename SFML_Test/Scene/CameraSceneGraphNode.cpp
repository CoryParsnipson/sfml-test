#include "CameraSceneGraphNode.h"
#include "Camera.h"

CameraSceneGraphNode::CameraSceneGraphNode(Camera& c, sf::RenderStates state /* = sf::RenderStates::Default */, bool visible /* = true */)
: SceneGraphNode(state, visible)
, camera_(&c)
{
}

void CameraSceneGraphNode::pre_draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   this->camera_->draw(surface, render_states);
}
