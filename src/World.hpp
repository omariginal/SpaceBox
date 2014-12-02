#ifndef CWORLD_HPP
#define CWORLD_HPP
//#include "Paddle.hpp"
//#include "Ball.hpp"
#include "Shape.hpp"
#include <iostream>
#include <list>
#include <vector>
//#include "SkyBox.hpp"

class World {

public:

	Shape* list[1];
	//SkyBox* mySkyBox;
	// std::list<Shape*> ObjectList;
	// std::vector<Shape*>* ObjectVector;

	World();
	~World();
	void draw_world(); // draw all objects in the world
	void reset(); // reset shapes
};

#endif
