#ifndef BEZIER_H
#define BEZIER_H

#include <GL/glut.h>

#include "Shape.hpp"
#include "Point3D.hpp"

class Bezier: public Shape {

protected:

public:
	GLint nCtrlPts;
	wcPt3D ctrlPts[10]; // to store control points,
	GLint R, S, T;
	wcPt3D RBM[73][26];
	wcPt3D RBC_face_norm[73][26]; // to store the mesh points and norm at each points

	Bezier();

   ~Bezier() {};

	void draw2DFrame();

	void setPixel(GLfloat x, GLfloat y);

	void binomialCoeffs(GLint n, GLint * C);

	void computeBezPt(GLfloat u, wcPt3D * bezPt, wcPt3D * BezTan, GLint nCtrlPts, wcPt3D * ctrlPts, GLint * C);

	void bezier(wcPt3D * ctrlPts, GLint nCtrlPts, GLint nBezCurvePts);

	void RotateBezier(wcPt3D * ctrlPts, GLint nCtrlPts, GLint nBezCurvePts);

	void drawWireRBC();

	void drawSolidRBC();

	void drawCPts();

	void displayCPts();

	void computeBezPt(GLfloat u, wcPt3D * bezPt, wcPt3D * BezTan, GLint nCtrlPts, wcPt3D * ctrlPts);

	void drawBezCurve(wcPt3D * ctrlPts, GLint nCtrlPts);

	void displayBezCurve();

	void draw();

};

#endif
