#include "MouseMoveCommand.h"

MouseMoveCommand::MouseMoveCommand(sf::Event::MouseMoveEvent& e)
: x(e.x)
, y(e.y)
{
}
