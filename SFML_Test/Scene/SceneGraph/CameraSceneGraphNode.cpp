#include "CameraSceneGraphNode.h"
#include "SceneGraphVisitor.h"
#include "Camera.h"

CameraSceneGraphNode::CameraSceneGraphNode(Camera& c, bool visible /* = true */)
: SceneGraphNode(visible)
, camera_(&c)
{
}


void CameraSceneGraphNode::accept(SceneGraphVisitor& visitor) {
   visitor.visit(this->camera_);
}

void CameraSceneGraphNode::pre_draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   this->camera_->draw(surface, render_states);
}
