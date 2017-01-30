#include "ToggleDebugInfoCommand.h"
#include "Game.h"
#include "SceneObject.h"
#include "Entity.h"

ToggleDebugInfoCommand::ToggleDebugInfoCommand(SceneObject* scene_graph, sf::Font* font)
: font_(font)
, scene_graph_(scene_graph)
, debug_visitor_(false, this->font_)
{
}

void ToggleDebugInfoCommand::execute() {
   if (!this->scene_graph_) {
      return;
   }

   this->debug_visitor_.show_debug(!this->debug_visitor_.show_debug());
   Game::logger().msg("ToggleDebugInfoCommand", Logger::INFO, "Debug info: " + std::string(this->debug_visitor_.show_debug() ? "SHOW" : "HIDE"));

   SceneObject::prefix_iterator it;
   for (it = this->scene_graph_->begin(); it != this->scene_graph_->end(); ++it) {
      (*it)->accept(this->debug_visitor_);
   }
}

void ToggleDebugInfoCommand::unexecute() {
   this->execute();
}
