#ifndef UPDATE_H
#define UPDATE_H

class Game;
class Scene;

class Update {
public:
   virtual void update(Game& game, Scene* scene = nullptr) = 0;
};

#endif
