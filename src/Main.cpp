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

#include "glaux.h"
#include "Camera.hpp"
#include "World.hpp"

GLint winWidth = 800, winHeight = 800;

GLfloat dnear = 5.0f;
GLfloat dfar = 150.0f;
GLfloat vangle = 40.0;

Camera myEye;

enum {
	SKY_LEFT, SKY_BACK, SKY_RIGHT, SKY_FRONT, SKY_TOP, SKY_BOTTOM
};
unsigned int skybox[6];

void drawSkybox(float size) {
	glColor3f(.7, .7, .7);
	bool b1 = glIsEnabled(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_BACK]);
	glBegin(GL_QUADS);
	//back face
	glTexCoord2f(0, 0);
	glVertex3f(size / 2, size / 2, size / 2);
	glTexCoord2f(1, 0);
	glVertex3f(-size / 2, size / 2, size / 2);
	glTexCoord2f(1, 1);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(size / 2, -size / 2, size / 2);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_LEFT]);
	glBegin(GL_QUADS);
	//left face
	glTexCoord2f(0, 0);
	glVertex3f(-size / 2, size / 2, size / 2);
	glTexCoord2f(1, 0);
	glVertex3f(-size / 2, size / 2, -size / 2);
	glTexCoord2f(1, 1);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_FRONT]);
	glBegin(GL_QUADS);
	//front face
	glTexCoord2f(1, 0);
	glVertex3f(size / 2, size / 2, -size / 2);
	glTexCoord2f(0, 0);
	glVertex3f(-size / 2, size / 2, -size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glTexCoord2f(1, 1);
	glVertex3f(size / 2, -size / 2, -size / 2);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_RIGHT]);
	glBegin(GL_QUADS);
	//right face
	glTexCoord2f(0, 0);
	glVertex3f(size / 2, size / 2, -size / 2);
	glTexCoord2f(1, 0);
	glVertex3f(size / 2, size / 2, size / 2);
	glTexCoord2f(1, 1);
	glVertex3f(size / 2, -size / 2, size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(size / 2, -size / 2, -size / 2);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_TOP]);
	glBegin(GL_QUADS);
	//top face
	glTexCoord2f(1, 0);
	glVertex3f(size / 2, size / 2, size / 2);
	glTexCoord2f(0, 0);
	glVertex3f(-size / 2, size / 2, size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-size / 2, size / 2, -size / 2);
	glTexCoord2f(1, 1);
	glVertex3f(size / 2, size / 2, -size / 2);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_BOTTOM]);
	glBegin(GL_QUADS);
	//bottom face
	glTexCoord2f(1, 1);
	glVertex3f(size / 2, -size / 2, size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glTexCoord2f(0, 0);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glTexCoord2f(1, 0);
	glVertex3f(size / 2, -size / 2, -size / 2);
	glEnd();

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	if (!b1)
		glDisable(GL_TEXTURE_2D);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(myEye.vangle, 1.0, myEye.dnear, myEye.dfar);

	gluLookAt(myEye.getMC().mat[0][3], myEye.getMC().mat[1][3], myEye.getMC().mat[2][3], myEye.getMC().mat[0][1], myEye.yref, myEye.zref, myEye.getMC().mat[0][1], myEye.getMC().mat[1][1], myEye.getMC().mat[2][1]);

//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();

	drawSkybox(10);

	glFlush();
	glutSwapBuffers();
}

unsigned int loadTexture(const char* filename) //load the filename named texture
		{
	unsigned int num;       //the id for the texture
	glGenTextures(1, &num);  //we generate a unique one
	AUX_RGBImageRec *pBitMap = auxDIBImageLoad(filename);
	if (pBitMap == NULL)
		exit(0);

	glBindTexture(GL_TEXTURE_2D, num);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pBitMap->sizeX, pBitMap->sizeY, 0, GL_RGB,
	GL_UNSIGNED_BYTE, pBitMap->data);
	return num;     //and we return the id
}

void init(void) {

	glClearColor(0.0, 0.0, 0.0, 1.0);  // Set display-window color to black
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, winHeight, 0.0);
	glColor3f(1.0, 0.0, 0.0);

	skybox[SKY_LEFT] = loadTexture("src/Galaxy_LT.bmp");
	skybox[SKY_BACK] = loadTexture("src/Galaxy_BK.bmp");
	skybox[SKY_RIGHT] = loadTexture("src/Galaxy_RT.bmp");
	skybox[SKY_FRONT] = loadTexture("src/Galaxy_FT.bmp");
	skybox[SKY_TOP] = loadTexture("src/Galaxy_DN.bmp");
	skybox[SKY_BOTTOM] = loadTexture("src/Galaxy_UP.bmp");
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("SpaceBox");
	glewInit();

	init();
//	menu();
	glutDisplayFunc(display);
//	glutMotionFunc(mouseMotion);
//	glutMouseFunc(mouseAction);
//	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();

	return 0;
}


