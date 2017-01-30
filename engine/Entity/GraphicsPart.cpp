#include "GraphicsPart.h"
#include "RenderSurface.h"
#include "Entity.h"
#include "PhysicsPart.h"
#include "Texture.h"
#include "Game.h"
#include "Scene.h"

GraphicsPart::GraphicsPart(std::string id)
: Part(id)
{
   Game::logger().msg("GraphicsPart", Logger::INFO, "Creating GraphicsPart");
}

GraphicsPart::~GraphicsPart() {
   Game::logger().msg("GraphicsPart", Logger::INFO, "Destroying GraphicsPart");

   SpriteList::const_iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); ++sprite_it) {
      delete *sprite_it;
   }
   this->sprites_.clear();
}

void GraphicsPart::add(Graphic* sprite) {
   this->sprites_.push_back(sprite);
}

Graphic* GraphicsPart::get(int idx) {
   Graphic* sprite = nullptr;

   if (idx < 0 || idx >= (signed)this->sprites_.size()) {
      return sprite;
   }

   SpriteList::const_iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); ++sprite_it) {
      if (idx == 0) {
         sprite = *sprite_it;
         break;
      }
      --idx;
   }

   return sprite;
}

void GraphicsPart::set_position(float x, float y) {
   SpriteList::const_iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); ++sprite_it) {
      (*sprite_it)->set_position(x, y);
   }
}

void GraphicsPart::set_position(const sf::Vector2f& pos) {
   SpriteList::const_iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); ++sprite_it) {
      (*sprite_it)->set_position(pos);
   }
}

void GraphicsPart::set_size(float width, float height) {
   SpriteList::const_iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); ++sprite_it) {
      (*sprite_it)->set_size(width, height);
   }
}

void GraphicsPart::set_size(const sf::Vector2f& size) {
   SpriteList::const_iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); ++sprite_it) {
      (*sprite_it)->set_size(size);
   }
}

void GraphicsPart::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   SpriteList::const_iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); ++sprite_it) {
      (*sprite_it)->draw(surface, render_states);
   }
}

void GraphicsPart::update(Game& game, Scene* scene) {
}

std::string GraphicsPart::serialize(Serializer& s) {
   Serializer::SerialData data;
   
   Graphic* sprite = this->get(0);
   Texture* texture;
   sf::Vector2f pos;

   if (sprite) {
      pos = sprite->get_position();
      texture = sprite->get_texture();
   }

   data["texture"] = (texture ? texture->id() : "");
   data["x"] = std::to_string((int)pos.x);
   data["y"] = std::to_string((int)pos.y);

   return s.serialize(data);
}

void GraphicsPart::deserialize(Serializer& s, Scene& scene, std::string& d) {
   Serializer::SerialData data = s.deserialize(scene, d);

   sf::Vector2f pos;
   pos.x = std::stoi(data["x"]);
   pos.y = std::stoi(data["y"]);

   if (scene.textures().get(data["texture"])) {
      Graphic* sprite = new SpriteGraphic(*scene.textures().get(data["texture"]));
      sprite->set_position(pos);

      this->add(sprite);
   }
}
