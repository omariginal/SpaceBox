#ifndef CWORLD_HPP
#define CWORLD_HPP

#include <iostream>
#include <list>
#include <vector>


class World {

public:

	Shape* list[2];
	World();

	~World();

	void draw_world(); // draw all objects in the world
	void draw_bezier(); // draw 2d world
};

#endif
