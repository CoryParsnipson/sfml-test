#ifndef MOVE_CAMERA_COMMAND_H
#define MOVE_CAMERA_COMMAND_H

#include "Command.h"
#include "dependencies.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Camera;

class MoveCameraCommand : public Command {
public:
   MoveCameraCommand(Camera& camera, sf::Vector2f delta = sf::Vector2f(0, 0), std::string id = "MoveCameraCommand");
   virtual ~MoveCameraCommand();

   virtual void execute();
   virtual void unexecute();

   void set_delta(sf::Vector2f delta);
   sf::Vector2f get_delta();

protected:
   sf::Vector2f delta_;
   Camera* camera_;
};

#endif
