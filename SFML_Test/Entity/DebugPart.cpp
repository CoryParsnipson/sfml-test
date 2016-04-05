#include "DebugPart.h"

#include "RenderTarget.h"
#include "Graphic.h"

#include "TextFactory.h"

#include "Entity.h"
#include "GraphicsPart.h"
#include "PhysicsPart.h"

DebugPart::DebugPart(std::string id)
: Part(id)
, show_text_(false)
, pos_text_(new Text("", TextFactory::inst()->get_font("retro")))
{
   Service::get_logger().msg("DebugPart", Logger::INFO, "Creating DebugPart");
}

DebugPart::~DebugPart() {
   Service::get_logger().msg("DebugPart", Logger::INFO, "Destroying DebugPart");

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

void DebugPart::draw(RenderTarget& surface) {
   SpriteList::const_iterator sprite_it;
   for (sprite_it = this->sprites_.begin(); sprite_it != this->sprites_.end(); ++sprite_it) {
      (*sprite_it)->draw(surface);
   }

   if (this->show_text()) {
      this->pos_text_->draw(surface);
   }
}

void DebugPart::update(Game& game, Scene* scene, Entity* entity) {
   unsigned int graphic_idx = 0;
   Graphic* g = nullptr;
   GraphicsPart* graphics = dynamic_cast<GraphicsPart*>(entity->get("graphics"));
   PhysicsPart* physics = dynamic_cast<PhysicsPart*>(entity->get("physics"));

   // get entity's debug enable
   if (entity) {
      this->show_text(entity->enable_debug_text());
   }

   // update size and position of each debug part sprite based on entity's graphic part
   if (graphics) {
      while ((g = graphics->get(graphic_idx))) {
         if (graphic_idx >= this->sprites_.size()) {
            Graphic* g_debug = new Shape(new sf::RectangleShape());

            g_debug->set_fill_color(sf::Color::Transparent);
            g_debug->set_outline_color(sf::Color::Red);
            g_debug->set_outline_thickness(1.0);

            this->sprites_.push_back(g_debug);
         }

         this->sprites_[graphic_idx]->set_position(g->get_position());
         this->sprites_[graphic_idx]->set_size(g->get_size());

         ++graphic_idx;
      }
   }

   // write down position of entity based on entity's physics part
   if (physics && this->show_text()) {
      sf::Vector2f pos = physics->get_position();
      this->pos_text_->set_string(std::to_string((int)pos.x) + ", " + std::to_string((int)pos.y));
      this->pos_text_->set_position(pos);
   }
}

Serialize::SerialObj DebugPart::serialize() {
   Serialize::SerialObj obj;
   // TODO: implement
   return obj;
}

void DebugPart::deserialize(Serialize::SerialObj& obj) {
   // TODO: implement
}
