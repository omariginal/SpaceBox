/**
 * File: World.hpp
 */

#include "World.hpp"

extern GLint showLight;

World::World() {

//	list[0] = new Cube();
//	list[1] = new Cube();

	myLight = new Light();
	myBezier = new Bezier();

    /* object list
	ObjectList.push_back(list[0]);
	ObjectList.push_back(list[1]);
    */

	/* object vector
	ObjectVector = new std::vector<Shape*>(2);
	std::vector<Shape*>& ObjectVecotrRef = *ObjectVector;
	ObjectVectorRef[0] = list[0];
	ObjectVectorRef[1] = list[1];
    */
}

World::~World(){
//    delete list[0];
//    delete list[1];

	delete myLight;
	delete myBezier;
}

void World::clean(){
//    delete list[0];
//    delete list[1]
	delete myLight;
	myLight = NULL;
	delete myBezier;
	myBezier = NULL;
}


void World::reSet(){
//    delete list[0];
//    delete list[1]
	myLight = new Light();
	myBezier = new Bezier();
}


void World::draw_world() {
//	list[0]->draw();
//	list[1]->draw();

	myBezier->draw();

	if (showLight) {
		myLight->draw();
	}

	/*
	for (std::list<Shape*>::const_iterator it = ObjectList.begin(); it!=ObjectList.end(); ++it) {
		(*it)->draw();
	} */

	/*
	std::vector<Shape*>& ObjectVectorRef = *ObjectVector;
	for (int i = 0; i < 2; i++ ) {
		 ObjectVectorRef[i]->draw();
	} */

}

