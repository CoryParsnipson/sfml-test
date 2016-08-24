#ifndef ZOOM_COMMAND_H
#define ZOOM_COMMAND_H

#include "Command.h"

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------
class Gamepad;
class Camera;
class Grid;

// ----------------------------------------------------------------------------
// ZoomCommand
//
// Zoom a viewport to the specified scaling
// ----------------------------------------------------------------------------
class ZoomCommand : public Command {
public:
   const float ZOOM_COEFFICIENT = 10.f;

   ZoomCommand(Camera* camera, Grid* grid, Gamepad* gamepad);
   virtual ~ZoomCommand();

   virtual void execute();
   virtual void unexecute();

private:
   float old_scale_factor_;
   Camera* camera_;
   Grid* grid_;
   Gamepad* gamepad_;
};

#endif
