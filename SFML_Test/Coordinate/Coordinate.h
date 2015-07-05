#ifndef COORDINATE_H
#define COORDINATE_H

#include "../dependencies.h"

template <class T>
class Coordinate {
public:
	Coordinate(T x, T y);
	Coordinate(Coordinate& c);
	virtual ~Coordinate() = 0; // make coordinate abstract class

	T getX() const;
	T getY() const;

	double magnitude() const;

	// TODO: overload operators +, -, *, /

protected:
	T first;
	T second;
};

template <class T>
Coordinate<T>::Coordinate(T x, T y) :
first(x),
second(y)
{
}

template <class T>
Coordinate<T>::~Coordinate() {
}

template <class T>
T Coordinate<T>::getX() const {
	return this->first;
}

template <class T>
T Coordinate<T>::getY() const {
	return this->second;
}

template <class T>
double Coordinate<T>::magnitude() const {
	return std::sqrt(std::pow(this->first, 2) + std::pow(this->second, 2));
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