#ifndef CWORLD_HPP
#define CWORLD_HPP

#include <iostream>
#include <list>
#include <vector>

#include "Shape.hpp"

#include "Cube.hpp"
#include "Light.hpp"
#include "Bezier.hpp"


class World {

public:
	//Shape* list[2];
	// std::list<Shape*> ObjectList;
	// std::vector<Shape*>* ObjectVector;

	Light* myLight;

	Bezier* myBezier;

	World();

	~World();

	void clean();

	void reSet();

	void draw_world();
};

#endif
