#ifndef CAMERA_ANCHOR_H
#define CAMERA_ANCHOR_H

#include "Component.h"

class CameraAnchor : public Component {
public:
   explicit CameraAnchor(const std::string& id = "CameraAnchor Component");
   CameraAnchor(const CameraAnchor& other);
   virtual ~CameraAnchor();

   CameraAnchor& operator=(const CameraAnchor& other);
   void swap(CameraAnchor& other);
};

#endif
