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
	glColor4f(.23, .78, .32, .41);
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();
}

