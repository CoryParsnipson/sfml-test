#include "CameraSceneGraphNode.h"
#include "Camera.h"

CameraSceneGraphNode::CameraSceneGraphNode(Camera& c, sf::RenderStates state /* = sf::RenderStates::Default */)
: SceneGraphNode(state)
, camera_(&c)
{
}

void CameraSceneGraphNode::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   this->camera_->draw(surface, render_states);

   // combine parent transform with current node's transform to pass onto children
   render_states.transform *= this->state_.transform;

   const_iterator<CompositeList> it;
   for (it = this->children_.begin(); it != this->children_.end(); ++it) {
      (*it)->draw(surface, render_states);
   }
}

void CameraSceneGraphNode::update(Game& game, Scene* scene /* = nullptr */, Entity* entity /* = nullptr */) {
   // empty
}
