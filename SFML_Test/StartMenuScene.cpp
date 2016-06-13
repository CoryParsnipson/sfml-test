#include "StartMenuScene.h"
#include "BuilderScene.h"
#include "TestUIScene.h"

#include "UtilFactory.h"
#include "TextFactory.h"

#include "Game.h"

#include "Draw.h"
#include "GraphicsPart.h"
#include "DebugPart.h"

#include "EntitySceneGraphNode.h"

StartMenuScene::StartMenuScene()
: Scene("StartMenuScene")
, show_debug_info_(false)
{
   // populate entities
   SceneGraphNode* node = new EntitySceneGraphNode(
      *TextFactory::inst()->create_text_entity(
         "SFML TEST",
         "retro",
         sf::Vector2f(0, 0),
         36,
         TextFactory::ALIGN::CENTER,
         sf::Color::White,
         this->show_debug_info_
   ));
   this->scene_graph_[0]->add(node);

   node = new EntitySceneGraphNode(
      *TextFactory::inst()->create_text_entity(
         "main menu",
         "retro",
         sf::Vector2f(0, 45),
         12,
         TextFactory::ALIGN::CENTER,
         sf::Color::White,
         this->show_debug_info_
   ));
   this->scene_graph_[0]->add(node);

   node = new EntitySceneGraphNode(
      *TextFactory::inst()->create_text_entity(
         "(Press SPACE or ENTER)",
         "retro",
         sf::Vector2f(0, 60),
         12,
         TextFactory::ALIGN::CENTER,
         sf::Color::White,
         this->show_debug_info_
   ));
   this->scene_graph_[0]->add(node);

   this->camera_->move(sf::Vector2f(Settings::Instance()->cur_width() / 2.f, Settings::Instance()->cur_height() / 2.f));
}

StartMenuScene::~StartMenuScene() {
}

void StartMenuScene::enter(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Entering game start menu state.");
}

void StartMenuScene::exit(Game& game) {
   Service::get_logger().msg(this->id_, Logger::INFO, "Exiting game start menu state.");
}

void StartMenuScene::update(Game& game, Scene* scene, Entity* entity) {
   Scene::update(game, scene, entity);
}

void StartMenuScene::process(Game& game, CloseInputEvent& e) {
   Scene::process(game, e);
}

void StartMenuScene::process(Game& game, ResizeInputEvent& e) {
   sf::Vector2f original_offset(this->camera_->get_size() / 2.f);

   Scene::process(game, e);

   sf::Vector2f new_center(e.width / 2.f, e.height / 2.f);

   this->camera_->set_size(sf::Vector2f(e.width, e.height));
   this->camera_->move(new_center - original_offset);
}

void StartMenuScene::process(Game& game, KeyPressInputEvent& e) {
   if (e.key == Key::Space || e.key == Key::Return) {
      game.switch_scene(new BuilderScene());
   } else if (e.key == Key::Escape) {
      // go to super special awesome test scene
      game.switch_scene(new TestUIScene());
   } else if (e.key == Key::O) {
      this->toggle_debug_info();
   }
}

void StartMenuScene::process(Game& game, MouseMoveInputEvent& e) {}
void StartMenuScene::process(Game& game, MouseWheelInputEvent& e) {}
void StartMenuScene::process(Game& game, MouseButtonInputEvent& e) {}

void StartMenuScene::toggle_debug_info() {
   this->show_debug_info_ = !this->show_debug_info_;
   Service::get_logger().msg(this->id(), Logger::INFO, "Debug info: " + std::string(this->show_debug_info_ ? "SHOW" : "HIDE"));

   SceneGraph::iterator it;
   for (it = this->scene_graph_.begin(); it != this->scene_graph_.end(); ++it) {
      SceneGraphNode::prefix_iterator node_it;
      for (node_it = it->second->begin(); node_it != it->second->end(); ++node_it) {
         auto e = dynamic_cast<EntitySceneGraphNode*>(*node_it);
         if (!e) {
            continue;
         }

         if (this->show_debug_info_) {
            e->get_entity()->add(new DebugPart());
         } else {
            e->get_entity()->remove("debug");
         }
      }
   }
}
