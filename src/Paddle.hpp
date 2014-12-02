/*
 * Paddle.hpp
 *
 *  Created on: Dec 1, 2014
 *      Author: Jackson
 */

#ifndef PADDLE_HPP_
#define PADDLE_HPP_

#include <gl\gl.h>
#include <gl\glu.h>

class Paddle {
public:
	float width;
	float height;
	float posX;
	float posY;

	Paddle();
	void draw(float x, float y, float width, float height);
};

#endif /* PADDLE_HPP_ */
