/**
 * File: World.hpp
 */

#include "World.hpp"

World::World() {
	list[0] = new Sphere();

}

World::~World(){
    delete list[0];
}

void World::draw_world() {
	list[0]->draw();
}


