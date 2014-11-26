/*
 * Main.cpp
 *
 *  Created on: Nov 26, 2014
 *      Author: Jackson
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>

#include "Camera.hpp"
#include "World.hpp"

GLint winWidth = 800, winHeight = 800;

void display(void) {


	glFlush();
	glutSwapBuffers();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("SpaceBox");
	glewInit();

//	init();
//	menu();
	glutDisplayFunc(display);
//	glutMotionFunc(mouseMotion);
//	glutMouseFunc(mouseAction);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();

	return 0;
}


