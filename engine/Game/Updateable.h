#ifndef UPDATEABLE_H
#define UPDATEABLE_H

class Game;

class Updateable {
public:
   virtual void update(Game& game) = 0;
};

#endif
