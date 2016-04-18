#ifndef CAMERA_SCENE_GRAPH_NODE_H
#define CAMERA_SCENE_GRAPH_NODE_H

#include "SceneGraphNode.h"

class Camera;

class CameraSceneGraphNode : public SceneGraphNode {
public:
   CameraSceneGraphNode(Camera& c, sf::RenderStates state = sf::RenderStates::Default);

   // draw interface
   virtual void draw(RenderSurface& surface, sf::RenderStates render_states = sf::RenderStates::Default);

   // update interface
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr);

protected:
   Camera* camera_;
};

#endif
