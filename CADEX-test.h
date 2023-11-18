#pragma once

#define _USE_MATH_DEFINES
#include <iostream>
#include <ostream>
#include <sstream>
#include <vector>
#include <cmath>
#include <iomanip>

/*
круг - имеет радиус
элипс - два радиуса, относительно Х и У
спираль - радиус, шаг (расстояние между витками)

сама фигура плоская, но находится в 3д

методы (наследуются от абстрактного класса и переопределяются для каждого типа фигуры):
точка в 3д - задаем угол, вычисляем координаты точки относительно типа фигура
первая производная - задаем угол, ищем через метод двух точек, либо иным способом для каждого типа фигуры
*/

template<class T>
class vector3d {
public:
	T x;
	T y;
	T z;

	vector3d(T _x, T _y, T _z) :
		x(_x),
		y(_y),
		z(_z)
	{}

	std::string str() const {
		std::ostringstream ss;
		ss << std::fixed;
		ss << std::setprecision(2);
		ss << "{" << x << "," << y << "," << z << "}";
		return ss.str();
	}

	friend std::ostream& operator<<(std::ostream& os, const vector3d& coords) {
		os << "{" << coords.x << "," << coords.y << "," << coords.z << "}";
		return os;
	}
};

template <typename T>
class Figure
{
public:
	Figure(T _x, T _y, T _z) :
		position(_x, _y, _z)
	{}

	virtual ~Figure() {}

	virtual vector3d<T> point(double angle) const = 0;

	virtual vector3d<T> derirative(double angle) const = 0;

	vector3d<T> position;
};

template <class T>
class Ellipse : public Figure<T>
{
public:
	Ellipse(T _x, T _y, T _r1, T _r2) : 
		Ellipse(_x, _y, static_cast<T>(0), _r1, _r2) 
	{}

	Ellipse(T _x, T _y, T _z, T _r1, T _r2) :
		Figure(_x, _y, _z), r1(_r1), r2(_r2)
	{}

	// формула точки на эллипсе по углу
	// https://ip76.ru/theory-and-practice/ellipse-point-coord/
	vector3d <T> point(double angle) override
	{
		return vector3d<T>(static_cast<T>(this->position.x + r1 * std::cos(angle)),
			static_cast<T>(this->position.y + r2 * std::sin(angle)),
			0);
	}

	vector3d<T> derirative(double angle) override
	{
		return vector3d<T>(static_cast<T>(r1 * -std::sin(angle)),
			static_cast<T>(r2 * std::cos(angle)),
			0);
	}

	T r1; // больший радиус
	T r2; // меньший радиус
};

template <typename T>
class Circle : public Figure<T>
{
public:
	explicit Circle(T _x, T _y, T _r) :
		Circle(_x, _y, 0, _r)
	{}

	explicit Circle(T _x, T _y, T _z, T _r) :
		Ellipse<T>(_x, _y, _z, _r, _r)
	{}
};

template <class T>
class Helix : public Figure<T>
{
public:
	explicit Helix(T _x, T _y, T _z, T _r, T _step) :
		Helix<T>(_x, _y, _z, _r, _r, _step, 0)

	{}

	explicit Helix(T _x, T _y, T _z, T _r1, T _r2, T _step, T _angleStart) :
		Figure<T>(_x, _y, _z), r1(_r1), r2(_r2), step(_step), angleStart(_angleStart)
	{}

	vector3d<T> point(double angle)
	{
		return vector3d<T>(static_cast<T>(this->position.x + r1 * cos(angle)),
			static_cast<T>(this->position.y + r2 * sin(angle)),
			// note changed here in difference with ellipse3::calculate()
			this->position.z + (angleStart + angle) / (M_PI * 2) * step);
	}

	T x;
	T y;
	T z;
	T r1;
	T r2;
	T step;
	T angleStart;
};
