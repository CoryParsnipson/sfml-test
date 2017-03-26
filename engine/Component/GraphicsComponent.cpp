#include "GraphicsComponent.h"

GraphicsComponent::GraphicsComponent(std::string id /* = "GraphicsComponent" */, Graphic* graphic /* = nullptr */)
: Component(id)
, graphic_(graphic)
{
}

GraphicsComponent::~GraphicsComponent() {
}

void GraphicsComponent::init() {
}

void GraphicsComponent::reset() {
   this->id("GraphicsComponent");
   this->graphic(nullptr);
}

void GraphicsComponent::graphic(Graphic* graphic) {
   this->graphic_ = graphic;
}

const Graphic* GraphicsComponent::graphic() {
   return this->graphic_;
}
