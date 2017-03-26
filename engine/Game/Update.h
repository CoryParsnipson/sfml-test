#ifndef UPDATE_H
#define UPDATE_H

class Game;

class Update {
public:
   virtual void update(Game& game) = 0;
};

#endif
