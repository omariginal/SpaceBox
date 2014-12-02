/*
 * Paddle.cpp
 *
 *  Created on: Dec 1, 2014
 *      Author: Jackson
 */

#include "Paddle.hpp"

Paddle::Paddle(/*float X, float Y, float Width, float Height*/) {
	posX = 0;
	posY = 0;
	width = 0;
	height = 0;
	speed = 10;
}

void Paddle::draw(float x, float y, float width, float height) {
	glBegin(GL_QUADS);
	glColor4f(0,1,1,.4);
	glVertex2f(x, y);
	glColor4f(0,0,1,.4);
	glVertex2f(x + width, y);
	glColor4f(1,0,1,.6);
	glVertex2f(x + width, y + height);
	glColor4f(1,1,1,.6);
	glVertex2f(x, y + height);
	glEnd();
}

