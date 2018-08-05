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
   stand_r->add(sf::IntRect(34, 0, 34, 41), 9);
   stand_r->add(sf::IntRect(68, 0, 34, 41), 30);
   stand_r->add(sf::IntRect(102, 0, 34, 41), 9);
   stand_r->add(sf::IntRect(136, 0, 34, 41), 9);
   stand_r->add(sf::IntRect(170, 0, 34, 41), 9);

   AnimationPtr run_r = std::make_shared<Animation>("megaman_zero_run_r", this->textures().get("megaman_zero_spritesheet"));
   //run_r->add(sf::IntRect(  0, 78, 40, 39), 5); // this first frame is transition between standing and running
   run_r->add(sf::IntRect( 40, 78, 40, 39), 5);
   run_r->add(sf::IntRect( 80, 78, 40, 39), 5);
   run_r->add(sf::IntRect(120, 78, 40, 39), 5);
   run_r->add(sf::IntRect(160, 78, 40, 39), 5);
   run_r->add(sf::IntRect(200, 78, 40, 39), 5);
   run_r->add(sf::IntRect(240, 78, 40, 39), 5);
   run_r->add(sf::IntRect(280, 78, 40, 39), 5);
   run_r->add(sf::IntRect(320, 78, 40, 39), 5);
   run_r->add(sf::IntRect(360, 78, 40, 39), 5);
   run_r->add(sf::IntRect(400, 78, 40, 39), 5);

   AnimationPtr run_l = std::make_shared<Animation>("megaman_zero_run_l", this->textures().get("megaman_zero_spritesheet"));
   //run_l->add(sf::IntRect(  40, 78, -40, 39), 5); // this first frame is transition between standing and running
   run_l->add(sf::IntRect( 80, 78, -40, 39), 5);
   run_l->add(sf::IntRect(120, 78, -40, 39), 5);
   run_l->add(sf::IntRect(160, 78, -40, 39), 5);
   run_l->add(sf::IntRect(200, 78, -40, 39), 5);
   run_l->add(sf::IntRect(240, 78, -40, 39), 5);
   run_l->add(sf::IntRect(280, 78, -40, 39), 5);
   run_l->add(sf::IntRect(320, 78, -40, 39), 5);
   run_l->add(sf::IntRect(360, 78, -40, 39), 5);
   run_l->add(sf::IntRect(400, 78, -40, 39), 5);
   run_l->add(sf::IntRect(440, 78, -40, 39), 5);

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

   Entity* character_run2 = this->create_entity("RunEntity2");
   character_run2->add<Sprite>("RunSprite2");
   character_run2->get<Sprite>()->animation(run_l);
   character_run2->get<Sprite>()->scale(3, 3);
   character_run2->get<Sprite>()->position(300, 100);
}

void MegamanScene::enter(Game& game) {
   Game::logger().msg(this->id(), Logger::INFO, "Entering MegamanScene.");
}

void MegamanScene::exit(Game& game) {
   Game::logger().msg(this->id(), Logger::INFO, "Exiting MegamanScene.");
}
