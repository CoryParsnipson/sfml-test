#include "RenderTexture.h"
#include "Camera.h"

RenderTexture::RenderTexture() {
}

RenderTexture::~RenderTexture() {
}

const sf::Texture& RenderTexture::get_texture() const {
   return this->surface_.getTexture();
}

void RenderTexture::set_camera(Camera& camera) {
   this->surface_.setView(camera.get_view());
}

sf::Vector2f RenderTexture::get_world_coord(const sf::Vector2i& point, Camera* camera) {
   return (camera ? this->surface_.mapPixelToCoords(point, camera->get_view()) : this->surface_.mapPixelToCoords(point));
}

sf::Vector2i RenderTexture::get_screen_coord(const sf::Vector2f& point, Camera* camera) {
   return (camera ? this->surface_.mapCoordsToPixel(point, camera->get_view()) : this->surface_.mapCoordsToPixel(point));
}

void RenderTexture::draw(sf::Drawable& d, int layer) {
   // TODO: handle layers
   this->surface_.draw(d);
}

void RenderTexture::update() {
   this->surface_.display();
}

void RenderTexture::clear() {
   this->surface_.clear();
}

void RenderTexture::clear(const sf::Color& color) {
   this->surface_.clear(color);
}
