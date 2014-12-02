/**
 * File: World.hpp
 */

#include "World.hpp"

World::World() {
	//list[0] = new Platform();
	//mySkyBox = new SkyBox();
}

World::~World() {
	delete list[0];
}

void World::reset() {
	delete list[0];
	//list[0] = new Platform();
}


void World::draw_world() {
	list[0]->draw();
}

