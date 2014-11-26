/**
 *	CP411 Assignment 4. template, OPP version
 *	H. Fan, 07/11/13
 */

#include "Angel.h"

#include <GL/glew.h>
#include <GL/glut.h>

//#include <GL/glaux.h> // for reading bmp files
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "Shape.hpp"
#include "Camera.hpp"
#include "World.hpp"
#include "Solar.hpp"

GLint winWidth = 800, winHeight = 800;

// move the view setting to camera
//GLfloat xeye = 3.0, yeye = 3.0, zeye = 7.0;  //  Viewing-coordinate origin.
//GLfloat xref = 0.0, yref = 0.0, zref = 0.0;  //  Look-at point.
//GLfloat Vx = 0.0, Vy = 1.0, Vz = 0.0;        //  View up vector.
//GLfloat vangle = 40.0, dnear = 1.0, dfar = 10.0;

// temp share variale
GLfloat theta = 0.0;

GLfloat red = 1.0, green = 1.0, blue = 1.0;  //color

GLint moving = 0, xBegin = 0, yBegin = 0, coordinate = 1, type = 1,
		selected = 0;

GLfloat P = 1.0;

GLint style = 1, lightOn = 0, iSolar = 0, glslOption = 0,
 showLight = 1, lightAdjust = 1;

GLint R = 45, S = 5, T = 5;
GLint Ptgen = 1, BCgen = 0, BCRotation = 0;

//Lighting substitute lx, ly, lz
GLfloat position[] = { 1.8, 1.8, 1.5, 1.0 };
GLfloat positionSolar[] = { 0.0, 0.0, 0.0, 1.0 };

GLfloat ambient[] = { 0.1, 0.1, 0.3, 1.0 };
GLfloat diffuse[] = { 0.6, 0.6, 1.0, 1.0 };
GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };

//GLfloat lmodel_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
//GLfloat local_view[] = { 0.0 };

//Material
GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_ambient_color[] = { 1, 1, 1, 1 };
GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat high_shininess[] = { 100.0 };
GLfloat mat_emission[] = { 1, 1, 1, 1 };

Camera myEye;
World myWorld;
Solar mySolar;

GLuint programObject; // GLSL object

void animateSolar(int keepGoing) {
	mySolar.Sun->rotate(0, -1, 0, 5);

	mySolar.Earth->rotate_mc(mySolar.Earth->MC.mat[0][0],
			mySolar.Earth->MC.mat[1][0], mySolar.Earth->MC.mat[2][0], 5);
	mySolar.Earth->rotate_origin(0, -1, 0, 5);

	mySolar.Moon->rotate_mc(mySolar.Moon->MC.mat[0][0],
			mySolar.Moon->MC.mat[1][0], mySolar.Moon->MC.mat[2][0], 5);

	mySolar.Moon->rotate_origin(0, -1, 0, 5);
	mySolar.Moon->rotate_relative(mySolar.Earth->MC.mat[0][3],
			mySolar.Earth->MC.mat[1][3], mySolar.Earth->MC.mat[2][3], 0, -1, 0,
			2);

	if (keepGoing) {
		glutTimerFunc(40, animateSolar, 1);
	}
	glutPostRedisplay();
}

void display(void) {

	if (BCRotation) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(myEye.vangle, 1.0, myEye.dnear, myEye.dfar);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(myEye.getMC().mat[0][3], myEye.getMC().mat[1][3], myEye.getMC().mat[2][3], myEye.getMC().mat[0][1], myEye.yref, myEye.zref, myEye.getMC().mat[0][1], myEye.getMC().mat[1][1], myEye.getMC().mat[2][1]);

		if (lightOn) {
			glDisable(GL_LIGHTING);
		}

		glLineWidth(3);

		glBegin(GL_LINES);
		//x-axis
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(2, 0, 0);
		glVertex3f(0, 0, 0);
		//y-axis
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0, 2, 0);
		glVertex3f(0, 0, 0);
		//z-axis
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0, 0, 2);
		glVertex3f(0, 0, 0);
		glEnd();
		glLineWidth(1);

		if (lightOn) {
			glEnable(GL_LIGHTING);
		} else
			glDisable(GL_LIGHTING);

		if (style == 2 && lightOn) {
			glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
			glLightfv(GL_LIGHT0, GL_POSITION, position);

//			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
//			glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

			glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, mat_ambient);
			glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
			glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
		}
	}

	if (iSolar) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(myEye.vangle, 1.0, myEye.dnear, myEye.dfar);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		gluLookAt(myEye.getMC().mat[0][3], myEye.getMC().mat[1][3], myEye.getMC().mat[2][3], myEye.getMC().mat[0][1], myEye.yref, myEye.zref, myEye.getMC().mat[0][1], myEye.getMC().mat[1][1], myEye.getMC().mat[2][1]);

		glLightfv( GL_LIGHT0, GL_POSITION, positionSolar);
		glPushMatrix();

		mySolar.draw_world();

		glPopMatrix();

	} else {

		myWorld.draw_world();

	}

	glFlush();
	glutSwapBuffers();
}

void winReshapeFcn(GLint newWidth, GLint newHeight) {
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	winWidth = newWidth;
	winHeight = newHeight;
}

void mouseAction(int button, int action, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		moving = 1;
		xBegin = x;
		yBegin = y;
	} else if (button == GLUT_LEFT_BUTTON && action == GLUT_UP) {
		moving = 0;
	}

	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN && Ptgen == 1) {
		if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {

			myWorld.myBezier->ctrlPts[myWorld.myBezier->nCtrlPts].x = x
					- winWidth / 2;
			myWorld.myBezier->ctrlPts[myWorld.myBezier->nCtrlPts].y = winHeight
					/ 2 - y;
			myWorld.myBezier->ctrlPts[myWorld.myBezier->nCtrlPts].z = 0;

			printf("Point %d set: %f %f \n", myWorld.myBezier->nCtrlPts,
					myWorld.myBezier->ctrlPts[myWorld.myBezier->nCtrlPts].x,
					myWorld.myBezier->ctrlPts[myWorld.myBezier->nCtrlPts].y);

			if (myWorld.myBezier->nCtrlPts < 10) {
				myWorld.myBezier->nCtrlPts++;
			}
		}
		glutPostRedisplay();
	}
}

void mouseMotion(GLint x, GLint y) {
	GLfloat rx, ry, rz, theta;
	if (moving) {
		if (coordinate == 1 && type == 1) {
			theta = (xBegin - x > 0) ? 1 : -1;
			rx = myWorld.myBezier->getMC().mat[0][0];
			ry = myWorld.myBezier->getMC().mat[1][0];
			rz = myWorld.myBezier->getMC().mat[2][0];

			if (selected == 0) {
				myWorld.myBezier->rotate_mc(rx, ry, rz, theta * 0.5);
			}
		}

		else if (coordinate == 1 && type == 2) {
			theta = (xBegin - x > 0) ? 1 : -1;
			rx = myWorld.myBezier->getMC().mat[0][1];
			ry = myWorld.myBezier->getMC().mat[1][1];
			rz = myWorld.myBezier->getMC().mat[2][1];
			if (selected == 0) {
				myWorld.myBezier->rotate_mc(rx, ry, rz, theta * 0.5);
			}

		}

		else if (coordinate == 2 && type == 1) {
			theta = (xBegin - x > 0) ? 1 : -1;
			if (selected == 0) {
				myWorld.myBezier->rotate_origin(1, 0, 0, theta * 0.5);
			}
			if (selected == 1) {
				myWorld.myLight->rotate_origin(1, 0, 0, theta * 0.5);

				position[0] = myWorld.myLight->getMC().mat[0][3];
				position[1] = myWorld.myLight->getMC().mat[1][3];
				position[1] = myWorld.myLight->getMC().mat[2][3];
			}

		}

		else if (coordinate == 2 && type == 2) {
			theta = (xBegin - x > 0) ? 1 : -1;
			if (selected == 0) {
				myWorld.myBezier->rotate_origin(0, 1, 0, theta * 0.5);
			}
			if (selected == 1) {
				myWorld.myLight->rotate_origin(0, 1, 0, theta * 0.5);
				position[0] = myWorld.myLight->getMC().mat[0][3];
				position[1] = myWorld.myLight->getMC().mat[1][3];
				position[1] = myWorld.myLight->getMC().mat[2][3];


			}
		}

		else if (coordinate == 2 && type == 4) {
			theta = (xBegin - x < 0) ? 1 : -1;

			if (selected == 0) {
				myWorld.myBezier->translate(theta * 0.02, 0, 0);
			}
			if (selected == 1) {
				myWorld.myLight->translate(theta * 0.02, 0, 0);
				position[0] = myWorld.myLight->getMC().mat[0][3];
				position[1] = myWorld.myLight->getMC().mat[1][3];
				position[1] = myWorld.myLight->getMC().mat[2][3];
			}
		}

		else if (coordinate == 3 && type == 1) {
			theta = (xBegin - x > 0) ? 1 : -1;
			myEye.rotate_origin(1, 0, 0, theta * 0.5);
		}

		// VC rotate y-axis
		else if (coordinate == 3 && type == 2) {
			theta = (xBegin - x < 0) ? 1 : -1;
			myEye.rotate_origin(0, 1, 0, theta * 0.5);

		}

		// Adjust Ka
		else if (lightAdjust == 1) {
			theta = (xBegin - x < 0) ? 0.05 : -0.05;
			ambient[0] += theta;
			ambient[1] += theta;
			ambient[2] += theta;
		}

		// Adjust B
		else if (lightAdjust == 2) {
			theta = (xBegin - x < 0) ? 0.02 : -0.02;
			ambient[0] += theta;
			ambient[1] += theta;
			ambient[2] += theta;
		}

		// Adjust Kd
		else if (lightAdjust == 3) {
			theta = (xBegin - x < 0) ? 0.05 : -0.05;
			diffuse[0] += theta;
			diffuse[1] += theta;
			diffuse[2] += theta;
		}

		// Adjust P
		else if (lightAdjust == 4) {
			theta = (xBegin - x < 0) ? 0.05 : -0.05;
			if (myWorld.myLight->P + theta < 1 && myWorld.myLight->P + theta > 0) {
				myWorld.myLight->P += theta;
				diffuse[0] += theta;
				diffuse[1] += theta;
				diffuse[2] += theta;

			}
		}

		glutPostRedisplay();
	}
}

void reset() {
  // add your code
}

void init(void) {

	myWorld.myLight->SetLight(1.8, 1.8, 1.5, 1.0);

	myEye.SetCamera(3.0, 3.0, 7.0, 0, 1, 0, 0, 0, 0, 40.0, 1.0, 10.0);

	glClearColor(0.0, 0.0, 0.0, 1.0);  // Set display-window color to black
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, winHeight, 0.0);
	glColor3f(1.0, 0.0, 0.0);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);


	// read and load shader here


	// load texture hear to mySolar

}

void lightTransSubMenu(GLint transOption) {
	selected = 1;
	coordinate = 2;
	switch (transOption) {
	case 1:
		type = 1;
		break;
	case 2:
		type = 2;
		break;
	case 3:
		type = 3;
		break;
	case 4:
		type = 4;
		break;
	case 5:
		type = 5;
		break;
	case 6:
		type = 6;
		break;
	case 7:
		lightAdjust = 1;
		coordinate = 0;
		type = 0;
		break;
	case 8:
		lightAdjust = 2;
		coordinate = 0;
		type = 0;
		break;
	case 9:
		lightAdjust = 3;
		coordinate = 0;
		type = 0;
		break;
	case 10:
		lightAdjust = 4;
		coordinate = 0;
		type = 0;
		break;
	}
	glutPostRedisplay();
}

void mainMenu(GLint option) {
	switch (option) {
	case 1:
		reset();
		break;
	case 2:
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void VCTransMenu(GLint transOption) {
	coordinate = 3;
	switch (transOption) {
	case 1:
		type = 1;
		break;
	case 2:
		type = 2;
		break;
	case 3:
		type = 3;
		break;
	case 4:
		type = 4;
		break;
	case 5:
		type = 5;
		break;
	case 6:
		type = 6;
		break;
	case 7:
		type = 7;
		break;
	case 8:
		type = 8;
		break;
	case 9:
		type = 9;
		break;

	default:
		break;
	}

	glutPostRedisplay();
}

void MCTransMenu(GLint transOption) {
	coordinate = 1;
	switch (transOption) {
	case 1:
		type = 1;
		break;
	case 2:
		type = 2;
		break;
	case 3:
		type = 3;
		break;
	case 4:
		type = 4;
		break;
	}
	glutPostRedisplay();
}

void WCTransMenu(GLint transOption) {
	coordinate = 2;
	switch (transOption) {
	case 1:
		type = 1;
		break;
	case 2:
		type = 2;
		break;
	case 4:
		type = 4;
		break;
	}
	glutPostRedisplay();
}

void ObjSubMenu(GLint objectOption) {
	switch (objectOption) {
	case 1:
		selected = 0;
		break;
	case 2:
		selected = 1;
		break;
	}
	glutPostRedisplay();
}

void viewOptionsSubMenu(GLint viewOption) {
	switch (viewOption) {
	case 1:
		lightOn = 0;
		break;
	case 2:
		lightOn = 1;
		break;
	case 3:
		showLight = 1;
		break;
	case 4:
		showLight = 0;
		break;

	default:
		break;
	}

	glutPostRedisplay();
}

void a4SubMenu(GLint A4Option) {
	switch (A4Option) {
	case 1: {
		iSolar = 0;

		Ptgen = 1;
	}
		break;
	case 2: {
		iSolar = 0;

		Ptgen = 0;
		BCgen = 1;
	}
		break;
	case 3: {
		iSolar = 0;

		Ptgen = 0;
		BCgen = 0;
		BCRotation = 1;


		// myWorld.myBezier->RotateBezier(myWorld.myBezier->ctrlPts, myWorld.myBezier->nCtrlPts, T);
	}
		break;
	case 4: {
		iSolar = 0;
		style = 1;
		lightOn = 0;
		Ptgen = 0;
		BCgen = 0;
		BCRotation = 1;

//		myWorld.myBezier->RotateBezier(myWorld.myBezier->ctrlPts,
//				myWorld.myBezier->nCtrlPts, T);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);
		glDisable( GL_NORMALIZE);
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_LIGHT0);
	}
		break;
	case 5: {
		iSolar = 0;
		lightOn = 1;
		style = 2;
		Ptgen = 0;
		BCgen = 0;
		BCRotation = 1;
		glEnable(GL_DEPTH_TEST); // enable OpenGL depth buffer algorithm for hidden surface removal
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable( GL_NORMALIZE);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_LIGHT0);
		glCullFace(GL_BACK);

	}
		break;
	case 6: {

		glClearColor(0.0, 0.0, 0.0, 1.0); // Set display-window color to black
		glMatrixMode(GL_PROJECTION);
		gluPerspective(myEye.vangle, 1.0, myEye.dnear, myEye.dfar);

		glMatrixMode(GL_MODELVIEW);

		gluLookAt(myEye.getMC().mat[0][3], myEye.getMC().mat[1][3], myEye.getMC().mat[2][3], myEye.getMC().mat[0][1], myEye.yref, myEye.zref, myEye.getMC().mat[0][1], myEye.getMC().mat[1][1], myEye.getMC().mat[2][1]);

		Ptgen = 0;
		BCgen = 0;

		iSolar = 1;

		glEnable(GL_DEPTH_TEST);

		glEnable( GL_NORMALIZE);

		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_ambient);
		glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

		// Set up simple lighting model
		glEnable( GL_LIGHTING);
//		glLightModelfv( GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
//		glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_POSITION, positionSolar);
		glEnable( GL_LIGHT0);

		// Enable material properties for lighting
		glEnable( GL_COLOR_MATERIAL);
		glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

		glPixelStorei( GL_UNPACK_ALIGNMENT, 1);

		glEnable( GL_CULL_FACE);
		glEnable( GL_TEXTURE_2D);

		animateSolar(1);

	}
	}

	glutPostRedisplay();

}

void rotSubMenu(GLint rotOption) {
	switch (rotOption) {
	case 1:
		R = 45;
		break;
	case 2:
		R = 90;
		break;
	case 3:
		R = 135;
		break;
	case 4:
		R = 180;
		break;
	case 5:
		R = 225;
		break;
	case 6:
		R = 270;
		break;
	case 7:
		R = 315;
		break;
	case 8:
		R = 360;
		break;
	case 9:
		S = 5;
		break;
	case 10:
		S = 10;
		break;
	case 11:
		S = 15;
		break;
	case 12:
		T = 5;
		break;
	case 13:
		T = 10;
		break;
	case 14:
		T = 15;
		break;
	case 15:
		T = 20;
		break;
	case 16:
		T = 25;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void glslSubMenu(GLint glslOption) {
	switch (glslOption) {
	case 1:
		// use GLSL shader
		break;
	case 2:
		// disable GLSL shader
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void menu() {
	GLint WCTrans_Menu, VCTrans_Menu, MCTrans_Menu, Object_Menu, Style_Menu,
			Display_Menu, ViewOptions_Menu, LTrans_Menu, A4_Menu, Rot_Menu,
			glsl_shad;

	MCTrans_Menu = glutCreateMenu(MCTransMenu);
	glutAddMenuEntry(" Rotate x ", 1);
	glutAddMenuEntry(" Rotate y ", 2);
	glutAddMenuEntry(" Rotate z", 3);
	glutAddMenuEntry(" Scale", 4);

	Object_Menu = glutCreateMenu(ObjSubMenu);
	glutAddMenuEntry(" Bezier ", 1);
	glutAddMenuEntry(" Light ", 2);

	WCTrans_Menu = glutCreateMenu(WCTransMenu);
	glutAddMenuEntry(" Rotate x ", 1);
	glutAddMenuEntry(" Rotate y ", 2);
	glutAddMenuEntry(" Rotate z", 3);
	glutAddMenuEntry(" Translate x ", 4);
	glutAddMenuEntry(" Translate y ", 5);
	glutAddMenuEntry(" Translate z", 6);

	VCTrans_Menu = glutCreateMenu(VCTransMenu);
	glutAddMenuEntry(" Rotate x ", 1);
	glutAddMenuEntry(" Rotate y ", 2);
	glutAddMenuEntry(" Rotate z", 3);
	glutAddMenuEntry(" Translate x ", 4);
	glutAddMenuEntry(" Translate y ", 5);
	glutAddMenuEntry(" Translate z", 6);
	glutAddMenuEntry(" Clipping Near ", 7);
	glutAddMenuEntry(" Clipping Far ", 8);
	glutAddMenuEntry(" Angle ", 9);

	ViewOptions_Menu = glutCreateMenu(viewOptionsSubMenu);
	glutAddMenuEntry(" Point Light Off", 1);
	glutAddMenuEntry(" Point Light On", 2);
	glutAddMenuEntry(" Show Point Light", 3);
	glutAddMenuEntry(" Hide Light On", 4);

	LTrans_Menu = glutCreateMenu(lightTransSubMenu);
	glutAddMenuEntry(" Rotate x ", 1);
	glutAddMenuEntry(" Rotate y ", 2);
	glutAddMenuEntry(" Rotate z", 3);
	glutAddMenuEntry(" Translate x ", 4);
	glutAddMenuEntry(" Translate y ", 5);
	glutAddMenuEntry(" Translate z", 6);
	glutAddMenuEntry(" Ambient Ka ", 7);
	glutAddMenuEntry(" Ambient B ", 8);
	glutAddMenuEntry(" Point Light Kd ", 9);
	glutAddMenuEntry(" Point Intensity P ", 10);

	Rot_Menu = glutCreateMenu(rotSubMenu);
	glutAddMenuEntry(" R->45 ", 1);
	glutAddMenuEntry(" R->90 ", 2);
	glutAddMenuEntry(" R->135 ", 3);
	glutAddMenuEntry(" R->180 ", 4);
	glutAddMenuEntry(" R->225 ", 5);
	glutAddMenuEntry(" R->270 ", 6);
	glutAddMenuEntry(" R->315 ", 7);
	glutAddMenuEntry(" R->360 ", 8);
	glutAddMenuEntry(" S->5 ", 9);
	glutAddMenuEntry(" S->10 ", 10);
	glutAddMenuEntry(" S->15 ", 11);
	glutAddMenuEntry(" T->5 ", 12);
	glutAddMenuEntry(" T->10 ", 13);
	glutAddMenuEntry(" T->15 ", 14);
	glutAddMenuEntry(" T->20 ", 15);
	glutAddMenuEntry(" T->25 ", 16);

	glsl_shad = glutCreateMenu(glslSubMenu);
	glutAddMenuEntry(" On ", 1);
	glutAddMenuEntry(" Off ", 2);

	A4_Menu = glutCreateMenu(a4SubMenu);
	glutAddMenuEntry(" Control Point Selection ", 1);
	glutAddMenuEntry(" Bezier Curve Generation ", 2);
	glutAddMenuEntry(" X-axis rotate ", 3);
	glutAddSubMenu(" Rotation Options ", Rot_Menu);
	glutAddMenuEntry(" Mesh Without Lighting ", 4);
	glutAddMenuEntry(" Solid With Lighting ", 5);
	glutAddSubMenu(" GLSL Shading ", glsl_shad);
	glutAddMenuEntry(" A4P2: Solar System ", 6);

	glutCreateMenu(mainMenu);      // Create main pop-up menu.
	glutAddSubMenu(" A4 ", A4_Menu);
	glutAddSubMenu(" Object Selection ", Object_Menu);
	glutAddSubMenu(" Model Transformations ", MCTrans_Menu);
	glutAddSubMenu(" WC Transformations ", WCTrans_Menu);
	glutAddSubMenu(" View Transformations ", VCTrans_Menu);
	glutAddSubMenu(" View Options ", ViewOptions_Menu);
	glutAddSubMenu(" Light Transformations ", LTrans_Menu);
	glutAddMenuEntry(" Reset ", 1);
	glutAddMenuEntry(" Quit", 2);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("A4 Template");

	glewInit();
	init();
	menu();

	glutDisplayFunc(display);
	glutMotionFunc(mouseMotion);
	glutMouseFunc(mouseAction);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();

	return 0;
}

