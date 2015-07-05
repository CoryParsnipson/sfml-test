#ifndef MAPCOORDINATE_H
#define MAPCOORDINATE_H

#include "Coordinate.h"
#include "ViewportCoordinate.h"

class MapCoordinate : public Coordinate<int> {
public:
	MapCoordinate(int x, int y);
	MapCoordinate(const MapCoordinate& other);

	ViewportCoordinate to_screen_coordinates();
	std::string to_string() const;
};

#endif