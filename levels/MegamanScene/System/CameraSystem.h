#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include "System.h"
#include "Camera.h"

// ----------------------------------------------------------------------------
// CameraSystem
// 
// This controls the camera positioning.
// ----------------------------------------------------------------------------
class CameraSystem : public System {
public:
   CameraSystem(const std::string& id, CameraPtr camera);
   virtual ~CameraSystem();

   CameraPtr camera() const;

private:
   CameraPtr camera_;

   virtual void on_init(Game& game);
   virtual void on_update(Game& game, Entity& e);
};

#endif
