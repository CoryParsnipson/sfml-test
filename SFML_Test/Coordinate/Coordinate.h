#ifndef COORDINATE_H
#define COORDINATE_H

#include "../dependencies.h"

template <class T>
class Coordinate {
public:
	Coordinate();
	Coordinate(T x, T y);
	Coordinate(Coordinate& c);
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
Coordinate<T>::Coordinate(T x, T y) :
x(x),
y(y)
{
}

template <class T>
Coordinate<T>::~Coordinate() {
}

template <class T>
T Coordinate<T>::getX() const {
	return this->x;
}

template <class T>
T Coordinate<T>::getY() const {
	return this->y;
}

template <class T>
double Coordinate<T>::magnitude() const {
	return std::sqrt(std::pow(this->x, 2) + std::pow(this->y, 2));
}

template <class T>
bool operator<(const Coordinate<T>& l, const Coordinate<T>& r) {
	return (l.magnitude() < r.magnitude());
}

template <class T>
bool operator==(const Coordinate<T>& l, const Coordinate<T>& r) {
	return (l.getX() == r.getX() && l.getY() == r.getY());
}

#endif