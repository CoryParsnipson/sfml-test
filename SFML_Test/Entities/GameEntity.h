#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

// forward declarations
class CloseCommand;

// abstract class that indicates that it is part of the game world
// used mainly for implementing a command interface. (Should rename this
// to be Controllable??)
class GameEntity {
public:
   // command interface
   virtual void process(CloseCommand& c) = 0;
};

#endif
