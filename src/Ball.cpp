/*
 * Ball.cpp
 *
 *  Created on: Dec 1, 2014
 *      Author: Jackson
 */

#include "Ball.hpp"
#define DEG2RAD (M_PI/180.0)
Ball::Ball() {
	posX = 500;
	posY = 340;
	dirX = rand() % 40;
	dirY = rand() % 10;
	size = 20;
	speed = 5;
}

void Ball::draw(float x, float y, float radius) {
	glBegin (GL_LINE_LOOP);
	for (int i = 0; i < 360; i++) {
		float degInRad = i * DEG2RAD;
		glVertex2f(cos(degInRad) * radius + x, sin(degInRad) * radius + y);
	}
	glEnd();
}


