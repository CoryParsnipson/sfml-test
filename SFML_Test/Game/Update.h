#ifndef UPDATE_H
#define UPDATE_H

class Game;
class Scene;
class Entity;

class Update {
public:
   virtual void update(Game& game, Scene* scene = nullptr, Entity* entity = nullptr) = 0;
};

#endif
