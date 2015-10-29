#include "MouseWheelCommand.h"

MouseWheelCommand::MouseWheelCommand(sf::Event::MouseWheelEvent& e)
: delta(e.delta)
, x(e.x)
, y(e.y)
{
}
