#include "GraphicsPart.h"
#include "Service.h"
#include "RenderSurface.h"
#include "Entity.h"
#include "PhysicsPart.h"

GraphicsPart::GraphicsPart(std::string id)
: Part(id)
{
   Service::get_logger().msg("GraphicsPart", Logger::INFO, "Creating GraphicsPart");
}

GraphicsPart::~GraphicsPart() {
   Service::get_logger().msg("GraphicsPart", Logger::INFO, "Destroying GraphicsPart");

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

Serialize::SerialObj GraphicsPart::serialize() {
   Serialize::SerialObj obj;

   Graphic* sprite = this->get(0);
   Texture* texture(nullptr);
   sf::Vector2f pos;
   if (sprite) {
      pos = sprite->get_position();
      texture = sprite->get_texture();
   }

   // don't need to specify type for Part serialization
   obj["pos_x"] = std::to_string(pos.x);
   obj["pos_y"] = std::to_string(pos.y);

   if (texture) {
      obj["texture"] = texture->id();
   }

   return obj;
}

void GraphicsPart::deserialize(Serialize::SerialObj& obj, const TextureAtlas* textures /* = nullptr*/) {
   sf::Vector2f pos(0, 0);
   pos.x = std::stod(obj["pos_x"]);
   pos.y = std::stod(obj["pos_y"]);

   Graphic* sprite = nullptr;

   if (textures && textures->get(obj["texture"])) {
      sprite = new SpriteGraphic(*textures->get(obj["texture"]));
   } else {
      sprite = new SpriteGraphic();
   }
   sprite->set_position(pos);

   this->add(sprite);
}