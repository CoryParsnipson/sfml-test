#include "WindowResizeCommand.h"

WindowResizeCommand::WindowResizeCommand(sf::Event::SizeEvent& e)
: width(e.width)
, height(e.height)
{
}
