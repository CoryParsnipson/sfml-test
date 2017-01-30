#include "DebugPart.h"
#include "Game.h"
#include "Draw.h"
#include "RenderSurface.h"
#include "TextFactory.h"
#include "Entity.h"
#include "GraphicsPart.h"
#include "PhysicsPart.h"

DebugPart::DebugPart(std::string id /* = "debug" */, sf::Font* debug_font /* = nullptr */)
: Part(id)
, show_text_(false)
, pos_text_(new TextGraphic("", debug_font))
{
   Game::logger().msg("DebugPart", Logger::INFO, "Creating DebugPart");
}

DebugPart::~DebugPart() {
   Game::logger().msg("DebugPart", Logger::INFO, "Destroying DebugPart");

   delete pos_text_;

   SpriteList::const_iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); ++sprite_it) {
      delete *sprite_it;
   }
   this->sprites_.clear();
}

void DebugPart::show_text(bool show_text) {
   this->show_text_ = show_text;
}

bool DebugPart::show_text() {
   return this->show_text_;
}

void DebugPart::draw(RenderSurface& surface, sf::RenderStates render_states /* = sf::RenderStates::Default */) {
   SpriteList::const_iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); ++sprite_it) {
      (*sprite_it)->draw(surface, render_states);
   }

   if (this->show_text()) {
      this->pos_text_->draw(surface, render_states);
   }
}

void DebugPart::update(Game& game, Scene* scene) {
   // TODO: redo this
   //unsigned int graphic_idx = 0;
   //Graphic* g = nullptr;
   //GraphicsPart* graphics = dynamic_cast<GraphicsPart*>(entity->get("graphics"));
   //PhysicsPart* physics = dynamic_cast<PhysicsPart*>(entity->get("physics"));

   //// get entity's debug enable
   //if (entity) {
   //   this->show_text(entity->enable_debug_text());
   //}

   //// update size and position of each debug part sprite based on entity's graphic part
   //if (graphics) {
   //   while ((g = graphics->get(graphic_idx))) {
   //      if (graphic_idx >= this->sprites_.size()) {
   //         Graphic* g_debug = new SpriteGraphic();

   //         g_debug->set_color(sf::Color::Transparent);
   //         g_debug->set_outline_color(sf::Color::Red);
   //         g_debug->set_outline_thickness(1.0);

   //         this->sprites_.push_back(g_debug);
   //      }

   //      this->sprites_[graphic_idx]->set_position(g->get_position());
   //      this->sprites_[graphic_idx]->set_size(g->get_size());

   //      ++graphic_idx;
   //   }
   //}

   //// write down position of entity based on entity's physics part
   //if (physics && this->show_text()) {
   //   sf::Vector2f pos = physics->get_position();
   //   this->pos_text_->set_string(std::to_string((int)pos.x) + ", " + std::to_string((int)pos.y));
   //   this->pos_text_->set_position(pos);
   //}
}

std::string DebugPart::serialize(Serializer& s) {
   // TODO: implement
   return "";
}

void DebugPart::deserialize(Serializer& s, Scene& scene, std::string& d) {
   // TODO: implement
}
