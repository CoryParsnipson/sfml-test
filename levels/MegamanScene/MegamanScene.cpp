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
   stand_r->add(sf::IntRect(0, 0, 34, 44), 120);
   stand_r->add(sf::IntRect(34, 0, 34, 44), 3);
   stand_r->add(sf::IntRect(68, 0, 34, 44), 120);
   stand_r->add(sf::IntRect(102, 0, 34, 44), 3);
   stand_r->add(sf::IntRect(136, 0, 34, 44), 3);
   stand_r->add(sf::IntRect(170, 0, 34, 44), 3);

   Entity* character = this->create_entity("CharacterEntity");
   character->add<Sprite>("CharacterSprite");
   character->get<Sprite>()->animation(stand_r);
   character->get<Sprite>()->scale(3, 3);
   character->get<Sprite>()->position(100, 100);
}

void MegamanScene::enter(Game& game) {
   Game::logger().msg(this->id(), Logger::INFO, "Entering MegamanScene.");
}

void MegamanScene::exit(Game& game) {
   Game::logger().msg(this->id(), Logger::INFO, "Exiting MegamanScene.");
}
