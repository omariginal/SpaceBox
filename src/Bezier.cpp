#include "Bezier.hpp"

extern GLfloat red, green, blue;
extern GLint style;
extern GLint winWidth, winHeight;

extern GLint R, S, T;
extern GLint Ptgen, BCgen, BCRotation;

#define Pi 3.141592653589793238462643

Bezier::Bezier() {
	nCtrlPts = 0;
}

void Bezier::draw2DFrame() {
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f(-winWidth / 4, 0.0);
	glVertex2f(winWidth / 4, 0.0);

	glColor3f(0.0, 1.0, 0.0);

	glVertex2f(0.0, -winHeight / 4);
	glVertex2f(0.0, winHeight / 4);
	glEnd();
}

void Bezier::setPixel(GLfloat x, GLfloat y) {
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}



void Bezier::drawCPts() {
	glPointSize(3.0);
	glColor3f(1.0, 0.0, 0.0);
	for (int i = 0; i < nCtrlPts; i++)
		setPixel(ctrlPts[i].x, ctrlPts[i].y);
}

void Bezier::displayCPts() {
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-winWidth / 2, winWidth / 2, -winHeight / 2, winHeight / 2);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0); // Set display-window color to black
	draw2DFrame();
	drawCPts();
}


void Bezier::draw() {

	glPushMatrix();

	this->ctm_multiply();

	glScalef(s * 0.01, s * 0.01, s * 0.01);

	if (Ptgen) {
		displayCPts();
	}
	if (BCgen) {
		//displayBezCurve();
	}

	if (BCRotation) {
		//RotateBezier(ctrlPts, nCtrlPts, T);
		if (style == 1) {
			//drawWireRBC();
		}
		if (style == 2) {
			//drawSolidRBC();
		}
	}
	glPopMatrix();

}

