#ifndef TRANSFORM_SCENE_GRAPH_NODE_H
#define TRANSFORM_SCENE_GRAPH_NODE_H

#include "SceneGraphNode.h"

class TransformSceneGraphNode : public SceneGraphNode {
public:
   TransformSceneGraphNode(sf::RenderStates state = sf::RenderStates::Default, bool visible = true);
};
#endif
