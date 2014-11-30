#ifndef CWORLD_HPP
#define CWORLD_HPP

#include <iostream>
#include <list>
#include <vector>
#include "Shape.hpp"
#include "Sphere.hpp"

class World {

public:

	Shape* list[2];
	World();

	~World();

	void draw_world(); // draw all objects in the world

};

#endif
