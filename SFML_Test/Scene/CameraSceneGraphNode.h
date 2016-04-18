#ifndef CAMERA_SCENE_GRAPH_NODE_H
#define CAMERA_SCENE_GRAPH_NODE_H

#include "SceneGraphNode.h"

class Camera;

class CameraSceneGraphNode : public SceneGraphNode {
public:
   CameraSceneGraphNode(Camera& c, sf::RenderStates state = sf::RenderStates::Default);

protected:
   Camera* camera_;

   virtual void pre_draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);
};

#endif
