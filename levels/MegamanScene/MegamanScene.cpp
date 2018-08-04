#include "MegamanScene.h"

#include "Game.h"
#include "Entity.h"
#include "Animation.h"

#include "Sprite.h"

MegamanScene::MegamanScene()
: Scene("MegamanScene")
{
}

MegamanScene::~MegamanScene() {
}

void MegamanScene::init(Game& game) {
   // load fonts
   this->fonts().load("retro", "retro.ttf");

   // load textures
   this->textures().load("megaman_zero_spritesheet", "megaman_zero.png");

   // setup player's input mapping
   game.add_player(1);

   AnimationPtr stand_r = std::make_shared<Animation>("megaman_zero_stand_r", this->textures().get("megaman_zero_spritesheet"));
   stand_r->add(sf::IntRect(0, 0, 34, 41), 120);
   stand_r->add(sf::IntRect(34, 0, 34, 41), 3);
   stand_r->add(sf::IntRect(68, 0, 34, 41), 120);
   stand_r->add(sf::IntRect(102, 0, 34, 41), 3);
   stand_r->add(sf::IntRect(136, 0, 34, 41), 3);
   stand_r->add(sf::IntRect(170, 0, 34, 41), 3);

   AnimationPtr run_r = std::make_shared<Animation>("megaman_zero_stand_r", this->textures().get("megaman_zero_spritesheet"));
   //run_r->add(sf::IntRect(0, 41, 39, 39), 7); // this first frame is transition between standing and running
   run_r->add(sf::IntRect(39, 41, 39, 39), 7);
   run_r->add(sf::IntRect(78, 41, 39, 39), 7);
   run_r->add(sf::IntRect(117, 41, 39, 39), 7);
   run_r->add(sf::IntRect(156, 41, 39, 39), 7);
   run_r->add(sf::IntRect(195, 41, 39, 39), 7);
   run_r->add(sf::IntRect(234, 41, 39, 39), 7);
   run_r->add(sf::IntRect(273, 41, 39, 39), 7);
   run_r->add(sf::IntRect(312, 41, 39, 39), 7);
   run_r->add(sf::IntRect(351, 41, 39, 39), 7);
   run_r->add(sf::IntRect(390, 41, 39, 39), 7);

   Entity* character = this->create_entity("CharacterEntity");
   character->add<Sprite>("CharacterSprite");
   character->get<Sprite>()->animation(stand_r);
   character->get<Sprite>()->scale(3, 3);
   character->get<Sprite>()->position(100, 100);

   Entity* character_run = this->create_entity("RunEntity");
   character_run->add<Sprite>("RunSprite");
   character_run->get<Sprite>()->animation(run_r);
   character_run->get<Sprite>()->scale(3, 3);
   character_run->get<Sprite>()->position(200, 100);
}

void MegamanScene::enter(Game& game) {
   Game::logger().msg(this->id(), Logger::INFO, "Entering MegamanScene.");
}

void MegamanScene::exit(Game& game) {
   Game::logger().msg(this->id(), Logger::INFO, "Exiting MegamanScene.");
}
