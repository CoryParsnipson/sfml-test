#include "ViewportCoordinate.h"

ViewportCoordinate::ViewportCoordinate(int x, int y) :
Coordinate(x, y)
{
}

std::string ViewportCoordinate::to_string() const {
	std::stringstream output;
	output << "Screen Coord. (" << this->first << ", " << this->second << ")";
	return output.str();
}