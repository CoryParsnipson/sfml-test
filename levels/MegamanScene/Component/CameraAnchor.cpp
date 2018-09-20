#include "CameraAnchor.h"

CameraAnchor::CameraAnchor(const std::string& id /* = "CameraAnchor Component" */)
: Component(id)
{
}

CameraAnchor::CameraAnchor(const CameraAnchor& other)
: Component(other.id())
{
}

CameraAnchor::~CameraAnchor() {
}

CameraAnchor& CameraAnchor::operator=(const CameraAnchor& other) {
   CameraAnchor tmp(other);
   this->swap(tmp);
   return *this;
}

void CameraAnchor::swap(CameraAnchor& other) {
   std::swap(static_cast<Component&>(*this), static_cast<Component&>(other));
}
