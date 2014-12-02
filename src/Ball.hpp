/*
 * Ball.hpp
 *
 *  Created on: Dec 1, 2014
 *      Author: Jackson
 */

#ifndef BALL_HPP_
#define BALL_HPP_

#include <math.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <string>
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <sstream>
#include <stdio.h>
#include <math.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <time.h>
#include "GL/freeglut.h"
#include <mmsystem.h>

#include "ball.hpp"

class Ball {
public:
	float posX;
	float posY;
	float dirX;
	float dirY;
	int size;
	int speed;

	Ball();
	void draw(float x,float y, float radius);
};

#endif /* BALL_HPP_ */
