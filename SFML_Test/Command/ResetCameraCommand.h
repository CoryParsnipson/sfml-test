#ifndef RESET_CAMERA_COMMAND_H
#define RESET_CAMERA_COMMAND_H

#include "dependencies.h"
#include "Command.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Camera;
class Grid;

// ----------------------------------------------------------------------------
// ResetCameraCommand
//
// Move the provided camera back to the original position
// ----------------------------------------------------------------------------
class ResetCameraCommand : public Command {
public:
   ResetCameraCommand(Camera* camera, Grid* grid = nullptr);
   virtual ~ResetCameraCommand();

   virtual void execute();
   virtual void unexecute();

private:
   Camera* camera_;
   Grid* grid_;
   float scale_factor_;
   sf::Vector2f pan_delta_;
};

#endif
