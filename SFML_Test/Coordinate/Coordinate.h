#ifndef COORDINATE_H
#define COORDINATE_H

#include "../dependencies.h"

template <class T>
class Coordinate {
public:
	Coordinate(T x, T y);
	virtual ~Coordinate() = 0; // make coordinate abstract class

	T getX() const;
	T getY() const;

	double magnitude() const;

	// TODO: overload operators +, -, *, /
protected:
	T x;
	T y;
};

template <class T>
Coordinate<T>::Coordinate(T x, T y)
: x(x)
, y(y)
{
}

template <class T>
Coordinate<T>::~Coordinate() {
}

template <class T>
T Coordinate<T>::getX() const {
	return x;
}

template <class T>
T Coordinate<T>::getY() const {
	return y;
}

template <class T>
double Coordinate<T>::magnitude() const {
	return std::sqrt(std::pow(this->getX(), 2) + std::pow(this->getY(), 2));
}

template <class T>
bool operator<(const Coordinate<T>& l, const Coordinate<T>& r) {
	if (l.magnitude() == r.magnitude()) {
		if (l.getX() == r.getX()) {
			return l.getY() < r.getY();
		}

		return l.getX() < r.getX();
	}
	return l.magnitude() < r.magnitude();
}

template <class T>
bool operator==(const Coordinate<T>& l, const Coordinate<T>& r) {
	return (l.getX() == r.getX() && l.getY() == r.getY());
}

#endif