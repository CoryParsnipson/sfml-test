#include "StartMenuScene.h"
#include "BuilderScene.h"
#include "TestUIScene.h"

#include "UtilFactory.h"
#include "TextFactory.h"

#include "Game.h"

#include "Draw.h"
#include "GraphicsPart.h"
#include "DebugPart.h"

StartMenuScene::StartMenuScene()
: Scene("StartMenuScene")
, show_debug_info_(false)
{
   sf::RenderStates s;
   s.transform = sf::Transform::Identity;
   s.transform.translate(this->camera_->get_center());
   this->scene_graph_[0]->set_render_state(s);

   // populate entities
   SceneGraphNode* node = new SceneGraphNode(
      TextFactory::inst()->create_text_entity(
         "SFML TEST",
         "retro",
         sf::Vector2f(0, 0),
         36,
         TextFactory::ALIGN::CENTER,
         sf::Color::White,
         this->show_debug_info_
   ));
   this->scene_graph_[0]->add(node);

   node = new SceneGraphNode(
      TextFactory::inst()->create_text_entity(
         "main menu",
         "retro",
         sf::Vector2f(0, 45),
         12,
         TextFactory::ALIGN::CENTER,
         sf::Color::White,
         this->show_debug_info_
   ));
   this->scene_graph_[0]->add(node);

   node = new SceneGraphNode(
      TextFactory::inst()->create_text_entity(
         "(Press SPACE or ENTER)",
         "retro",
         sf::Vector2f(0, 60),
         12,
         TextFactory::ALIGN::CENTER,
         sf::Color::White,
         0,
         this->show_debug_info_
   ));
   this->scene_graph_[0]->add(node);
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

void StartMenuScene::process(Game& game, CloseCommand& c) {
   Scene::process(game, c);
}

void StartMenuScene::process(Game& game, KeyPressCommand& c) {
   if (c.event.code == sf::Keyboard::Key::Space || c.event.code == sf::Keyboard::Key::Return) {
      game.switch_scene(new BuilderScene());
   } else if (c.event.code == sf::Keyboard::Key::Escape) {
      // go to super special awesome test scene
      game.switch_scene(new TestUIScene());
   } else if (c.event.code == sf::Keyboard::Key::O) {
      this->toggle_debug_info();
   }
}

void StartMenuScene::process(Game& game, WindowResizeCommand& c) {
   sf::Vector2f original_offset = this->camera_->get_center();

   Scene::process(game, c);

   sf::Vector2f new_size(c.width, c.height);
   sf::Vector2f new_center(c.width / 2.f, c.height / 2.f);

   // adjust scene graph
   sf::RenderStates s(this->scene_graph_[0]->get_render_state());
   s.transform.translate(new_center - original_offset);
   this->scene_graph_[0]->set_render_state(s);
}

void StartMenuScene::process(Game& game, MouseMoveCommand& c) {}
void StartMenuScene::process(Game& game, MouseButtonCommand& c) {}
void StartMenuScene::process(Game& game, MouseWheelCommand& c) {}

void StartMenuScene::toggle_debug_info() {
   this->show_debug_info_ = !this->show_debug_info_;
   Service::get_logger().msg(this->id(), Logger::INFO, "Debug info: " + std::string(this->show_debug_info_ ? "SHOW" : "HIDE"));

   SceneGraph::iterator it;
   for (it = this->scene_graph_.begin(); it != this->scene_graph_.end(); ++it) {
      SceneGraphNode::prefix_iterator node_it;
      for (node_it = it->second->begin(); node_it != it->second->end(); ++node_it) {
         // TODO: clunky
         Entity* e = dynamic_cast<Entity*>(node_it->get_drawable());
         if (!e) {
            continue;
         }

         if (this->show_debug_info_) {
            e->add(new DebugPart());
         } else {
            e->remove("debug");
         }
      }
   }
}
