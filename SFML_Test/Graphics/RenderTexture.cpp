#include "RenderTexture.h"
#include "Layer.h"

RenderTexture::RenderTexture() {
}

RenderTexture::~RenderTexture() {
}

const sf::Texture& RenderTexture::get_texture() const {
   return this->surface_.getTexture();
}

void RenderTexture::set_active_layer(Layer& layer) {
   this->surface_.setView(layer.get_view());
}

sf::Vector2f RenderTexture::get_world_coord(const sf::Vector2i& point, Layer* layer) {
   return (layer ? this->surface_.mapPixelToCoords(point, layer->get_view()) : this->surface_.mapPixelToCoords(point));
}

sf::Vector2i RenderTexture::get_screen_coord(const sf::Vector2f& point, Layer* layer) {
   return (layer ? this->surface_.mapCoordsToPixel(point, layer->get_view()) : this->surface_.mapCoordsToPixel(point));
}

void RenderTexture::draw(sf::Drawable& d) {
   this->surface_.draw(d);
}

void RenderTexture::draw(sf::Drawable& d, Layer& layer) {
   sf::View original_view = this->surface_.getView();

   this->set_active_layer(layer);
   this->draw(d);

   this->surface_.setView(original_view);
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
