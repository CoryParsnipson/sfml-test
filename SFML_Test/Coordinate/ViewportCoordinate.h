#ifndef VIEWPORT_COORDINATE_H
#define VIEWPORT_COORDINATE_H

#include "Coordinate.h"

class ViewportCoordinate : public Coordinate<int> {
public:
	ViewportCoordinate(int x, int y);

	std::string to_string() const;
};

#endif