#include "MapCoordinate.h"

MapCoordinate::MapCoordinate(int x, int y)
	: Coordinate<int>(x, y)
{
}

MapCoordinate::MapCoordinate(const MapCoordinate& other) :
Coordinate(other.getX(), other.getY())
{
}

ViewportCoordinate MapCoordinate::to_screen_coordinates() {
	// calculate x screen coordinate
	float screen_x = this->getX() * Settings::Instance()->TILE_WIDTH + (this->getY() & 1) * Settings::Instance()->TILE_WIDTH / 2.f;
	float screen_y = this->getY() * (Settings::Instance()->TILE_HEIGHT_OVERLAP) - (Settings::Instance()->TILE_HEIGHT - Settings::Instance()->TILE_HEIGHT_RHOMBUS);

	// TODO: double check the above. Separate tiles might be overlapping... Make sure to settle on a "base height" and draw tiles to stay within bounds.

	return ViewportCoordinate((int)screen_x, (int)screen_y);
}

std::string MapCoordinate::to_string() const {
	std::stringstream output;
	output << "Map Coord. (" << this->getX() << ", " << this->getY() << ")";
	return output.str();
}

bool operator<(const MapCoordinate& l, const MapCoordinate& r) {
	// a natural way for comparison would be to compare magnitudes, but it is common for two completely different 2d vectors
	// to have equal magnitude, causing < to return false. This is problematic to use as a map key...

	//return (l.magnitude() < r.magnitude());
	return (l.getX() < r.getX() || (l.getX() == r.getX() && l.getY() < r.getY()));
}

bool operator==(const MapCoordinate& l, const MapCoordinate& r) {
	return (l.getX() == r.getX() && l.getY() == r.getY());
}