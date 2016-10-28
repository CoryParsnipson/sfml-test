#include "DebugVisitor.h"
#include "DebugPart.h"
#include "Camera.h"
#include "Entity.h"
#include "Map.h"
#include "Grid.h"
#include "Widget.h"
#include "Gamepad.h"

DebugVisitor::DebugVisitor(bool show_debug_info /* = false */, sf::Font* font /* = nullptr */)
: show_debug_info_(show_debug_info)
, font_(font)
{
}

void DebugVisitor::show_debug(bool show) {
   this->show_debug_info_ = show;
}

bool DebugVisitor::show_debug() {
   return this->show_debug_info_;
}

void DebugVisitor::visit(Camera* camera) {}

void DebugVisitor::visit(Entity* entity) {
   if (!entity) {
      return;
   }

   if (this->show_debug_info_) {
      entity->add(new DebugPart("debug", this->font_));
   } else {
      entity->remove("debug");
   }
}

void DebugVisitor::visit(Map* map) {}
void DebugVisitor::visit(Grid* grid) {}
void DebugVisitor::visit(Widget* widget) {}
void DebugVisitor::visit(Gamepad* gamepad) {}
