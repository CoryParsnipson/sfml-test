#include "MoveCommand.h"

MoveCommand::MoveCommand(Moveable* moveable, sf::Vector2f delta /* = sf::Vector2f(0, 0) */, std::string id /* = "MoveCommand" */)
: Command(id)
, delta_(delta)
, moveable_(moveable)
{
}

MoveCommand::~MoveCommand() {
}

void MoveCommand::execute() {
   if (this->moveable_) {
      this->moveable_->move(this->delta_);
   }
}

void MoveCommand::unexecute() {
   if (this->moveable_) {
      this->moveable_->move(-1.f * this->delta_);
   }
}

void MoveCommand::delta(sf::Vector2f delta) {
   this->delta_ = delta;
}

const sf::Vector2f& MoveCommand::delta() {
   return this->delta_;
}

void MoveCommand::moveable(Moveable* moveable) {
   this->moveable_ = moveable;
   this->delta_ = sf::Vector2f(0, 0);
}

Moveable* MoveCommand::moveable() {
   return this->moveable_;
}
