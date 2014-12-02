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
#include "World.hpp"

int width = 1000;
int height = 680;
int interval = 1000 / 60;

int racket_width = 20;
int racket_height = 180;
int racket_widtht = 180;
int racket_heightt = 20;
int racket_speed = 8;

float ball_pos_x = width / 2;
float ball_pos_y = height / 2;
float ball_dir_x = -1.0f;
float ball_dir_y = 0.0f;
int ball_size = 80;
int ball_speed = 5;

float racket_left_x = 10.0f;
float racket_left_y = 50.0f;

float racket_right_x = width - racket_width - 10;
float racket_right_y = 50;
GLint play = 1;

float racket_bot_x = 0.0f;
float racket_bot_y = 10.0f;
float racket_top_x = 0;
float racket_top_y = height - racket_heightt - 10;

#undef PI
#define PI 3.141592657

enum {
	NORMAL = 0, WEIRD = 1
};

enum {
	STREAK = 0, CIRCLE = 1
};

#define MAXSTARS 400
#define MAXPOS 10000
#define MAXWARP 10
#define MAXANGLES 6000

typedef struct _starRec {
	GLint type;
	float x[2], y[2], z[2];
	float offsetX, offsetY, offsetR, rotation;
} starRec;

GLenum doubleBuffer;
GLint windW = 1000, windH = 600;

GLenum flag = NORMAL;
GLint starCount = MAXSTARS / 2;
float speed = 0.2;
GLint nitro = 0;
starRec stars[MAXSTARS];
float sinTable[MAXANGLES];

float Sin(float angle) {
	return (sinTable[(GLint) angle]);
}

float Cos(float angle) {
	return (sinTable[((GLint) angle + (MAXANGLES / 4)) % MAXANGLES]);
}

std::string int2str(int x) {
	// converts int to string
	std::stringstream ss;
	ss << x;
	return ss.str();
}

void drawRect(float x, float y, float width, float height) {
	//glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glVertex2f(x, y);

	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glColor3f(0, 0, 0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnd();
}
void keyboard() {
	// left racket
	if (GetAsyncKeyState(VK_UP) && (racket_left_y < 500)) {
		racket_left_y += racket_speed;
		racket_right_y += racket_speed;
	}
	if (GetAsyncKeyState(VK_DOWN) && (racket_left_y > 0)) {
		racket_left_y -= racket_speed;
		racket_right_y -= racket_speed;
	}
	if (GetAsyncKeyState(VK_LEFT) && (racket_top_x > 0)) {
		racket_top_x -= racket_speed;
		racket_bot_x -= racket_speed;
	}
	if (GetAsyncKeyState(VK_RIGHT) && (racket_top_x < 1000)) {
		racket_top_x += racket_speed;
		racket_bot_x += racket_speed;
	}
	// printf("Pos: %f \n", racket_right_y);

	//  std::cout << "Pos:" << " " << racket_left_y ;

}

void MoveStars(void) {
	float offset;
	GLint n;

	offset = speed * 60.0;

	for (n = 0; n < starCount; n++) {
		stars[n].x[1] = stars[n].x[0];
		stars[n].y[1] = stars[n].y[0];
		stars[n].z[1] = stars[n].z[0];
		stars[n].x[0] += stars[n].offsetX;
		stars[n].y[0] += stars[n].offsetY;
		stars[n].z[0] -= offset;
		stars[n].rotation += stars[n].offsetR;
		if (stars[n].rotation > MAXANGLES) {
			stars[n].rotation = 0.0;
		}
	}
}

void RotatePoint(float *x, float *y, float rotation) {
	float tmpX, tmpY;

	tmpX = *x * Cos(rotation) - *y * Sin(rotation);
	tmpY = *y * Cos(rotation) + *x * Sin(rotation);
	*x = tmpX;
	*y = tmpY;
}

void ShowStar(GLint n) {
	float x0, y0, x1, y1, width;
	GLint i;

	x0 = stars[n].x[0] * windW / stars[n].z[0];
	y0 = stars[n].y[0] * windH / stars[n].z[0];
	RotatePoint(&x0, &y0, stars[n].rotation);
	x0 += windW / 2.0;
	y0 += windH / 2.0;

	if (x0 >= 0.0 && x0 < windW && y0 >= 0.0 && y0 < windH) {
		if (stars[n].type == STREAK) {
			x1 = stars[n].x[1] * windW / stars[n].z[1];
			y1 = stars[n].y[1] * windH / stars[n].z[1];
			RotatePoint(&x1, &y1, stars[n].rotation);
			x1 += windW / 2.0;
			y1 += windH / 2.0;

			glLineWidth(MAXPOS / 100.0 / stars[n].z[0] + 1.0);
			glColor3f(1.0, (MAXWARP - speed) / MAXWARP,
					(MAXWARP - speed) / MAXWARP);
			if (fabs(x0 - x1) < 1.0 && fabs(y0 - y1) < 1.0) {
				glBegin(GL_POINTS);
				glVertex2f(x0, y0);
				glEnd();
			} else {
				glBegin(GL_LINES);
				glVertex2f(x0, y0);
				glVertex2f(x1, y1);
				glEnd();
			}
		} else {
			width = MAXPOS / 10.0 / stars[n].z[0] + 1.0;
			glColor3f(1.0, 0.0, 0.0);
			glBegin(GL_POLYGON);
			for (i = 0; i < 8; i++) {
				float x = x0 + width * Cos((float) i * MAXANGLES / 8.0);
				float y = y0 + width * Sin((float) i * MAXANGLES / 8.0);
				glVertex2f(x, y);
			};
			glEnd();
		}
	}
}

void NewStar(GLint n, GLint d) {
	if (rand() % 4 == 0) {
		stars[n].type = CIRCLE;
	} else {
		stars[n].type = STREAK;
	}
	stars[n].x[0] = (float) (rand() % MAXPOS - MAXPOS / 2);
	stars[n].y[0] = (float) (rand() % MAXPOS - MAXPOS / 2);
	stars[n].z[0] = (float) (rand() % MAXPOS + d);
	stars[n].x[1] = stars[n].x[0];
	stars[n].y[1] = stars[n].y[0];
	stars[n].z[1] = stars[n].z[0];
	if (rand() % 4 == 0 && flag == WEIRD) {
		stars[n].offsetX = (float) (rand() % 100 - 100 / 2);
		stars[n].offsetY = (float) (rand() % 100 - 100 / 2);
		stars[n].offsetR = (float) (rand() % 25 - 25 / 2);
	} else {
		stars[n].offsetX = 0.0;
		stars[n].offsetY = 0.0;
		stars[n].offsetR = 1.0;
	}
}

GLenum StarPoint(GLint n) {
	float x0, y0;

	x0 = stars[n].x[0] * windW / stars[n].z[0];
	y0 = stars[n].y[0] * windH / stars[n].z[0];
	RotatePoint(&x0, &y0, stars[n].rotation);
	x0 += windW / 2.0;
	y0 += windH / 2.0;

	if (x0 >= 0.0 && x0 < windW && y0 >= 0.0 && y0 < windH) {
		return GL_TRUE;
	} else {
		return GL_FALSE;
	}
}

void UpdateStars(void) {
	GLint n;

	glClear(GL_COLOR_BUFFER_BIT);

	for (n = 0; n < starCount; n++) {
		if (stars[n].z[0] > speed || (stars[n].z[0] > 0.0 && speed < MAXWARP)) {
			if (StarPoint(n) == GL_FALSE) {
				NewStar(n, MAXPOS);
			}
		} else {
			NewStar(n, MAXPOS);
		}
	}
}

void ShowStars(void) {
	GLint n;

	glClear(GL_COLOR_BUFFER_BIT);

	for (n = 0; n < starCount; n++) {
		if (stars[n].z[0] > speed || (stars[n].z[0] > 0.0 && speed < MAXWARP)) {
			ShowStar(n);
		}
	}
}

void draw() {

	// clear (has to be done at the beginning)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable( GL_DEPTH_TEST);
	glLoadIdentity();
	// draw rackets
	if (play == 1){
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable( GL_BLEND);
		glColor4f(.23, .78, .32, 0.1);
		ShowStars();


		glColor4f(.23, .78, .32, 0.41);
		drawRect(racket_left_x, racket_left_y, racket_width, racket_height);
		glColor4f(.23, .78, .32, .41);
		drawRect(racket_right_x, racket_right_y, racket_width, racket_height);
		glColor4f(.23, .78, .32, .41);
		drawRect(racket_top_x, racket_top_y, racket_widtht, racket_heightt);
		glColor4f(.23, .78, .32, .41);
		drawRect(racket_bot_x, racket_bot_y, racket_widtht, racket_heightt);
		glColor4f(.23, .78, .32, 0.41);
		// draw score
		drawRect(ball_pos_x - ball_size / 2, ball_pos_y - ball_size / 2, ball_size,
				ball_size);

	}
	glutSwapBuffers();
}
void vec2_norm(float& x, float &y) {
	// sets a vectors length to 1 (which means that x + y == 1)
	float length = sqrt((x * x) + (y * y));
	if (length != 0.0f) {
		length = 1.0f / length;
		x *= length;
		y *= length;
	}
}
void updateBall() {
	// fly a bit
	ball_pos_x += ball_dir_x * ball_speed;
	ball_pos_y += ball_dir_y * ball_speed;

	// hit by left racket?
	if (ball_pos_x < racket_left_x + racket_width && ball_pos_x > racket_left_x
			&& ball_pos_y < racket_left_y + racket_height
			&& ball_pos_y > racket_left_y) {
		// set fly direction depending on where it hit the racket
		// (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
		float t = ((ball_pos_y - racket_left_y) / racket_height) - 0.5;
		ball_dir_x = fabs(ball_dir_x); // force it to be positive
		ball_dir_y = t;
		ball_speed = ball_speed + 1.1;
	}

	// hit by right racket?
	if (ball_pos_x > racket_right_x
			&& ball_pos_x < racket_right_x + racket_width
			&& ball_pos_y < racket_right_y + racket_height
			&& ball_pos_y > racket_right_y) {
		// set fly direction depending on where it hit the racket
		// (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
		float t = ((ball_pos_y - racket_right_y) / racket_height) - 0.5f;
		ball_dir_x = -fabs(ball_dir_x); // force it to be negative
		ball_dir_y = t;
		ball_speed = ball_speed + 1.1;
	}
	// hit by top racket?

	if (ball_pos_y < racket_top_y && ball_pos_y > racket_top_y - racket_widtht

	) {
		printf("true");
		// set fly direction depending on where it hit the racket
		// (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
		float t = ((ball_pos_y - racket_right_y) / racket_height) - 0.5f;
		ball_dir_y = -fabs(ball_dir_y);
		ball_dir_x = 0;
		ball_speed = ball_speed + 1.1;
	}
	// hit left wall?
	if (ball_pos_x < 0) {
		ball_pos_x = width / 2;
		ball_pos_y = height / 2;
		ball_dir_x = fabs(ball_dir_x); // force it to be positive
		ball_dir_y = 0;
		ball_speed = 5.0;
	}

	// hit right wall?
	if (ball_pos_x > width) {
		ball_pos_x = width / 2;
		ball_pos_y = height / 2;
		ball_dir_x = -fabs(ball_dir_x); // force it to be negative
		ball_dir_y = 0;
		ball_speed = 5.0;
	}

	// hit top wall?
	if (ball_pos_y > height) {
		ball_pos_x = width / 2;
		ball_pos_y = height / 2;
		ball_dir_x = -fabs(ball_dir_x); // force it to be negative
		ball_dir_y = 0;
		ball_speed = 5.0;
	}
	// hit bottom wall?
	if (ball_pos_y < 0) {
		ball_pos_x = width / 2;
		ball_pos_y = height / 2;
		ball_dir_x = -fabs(ball_dir_y); // force it to be negative
		ball_dir_y = 0;
		ball_speed = 5.0;
	}


	// make sure that length of dir stays at 1
	vec2_norm(ball_dir_x, ball_dir_y);
}
void update(int value) {
	// input handling
	keyboard();
	// update ball
	updateBall();
	// Call update() again in 'interval' milliseconds
	glutTimerFunc(interval, update, 0);
	// Redisplay frame
	glutPostRedisplay();
}
// program entry point
void enable2D(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Reshape(int width, int height) {
	windW = width;
	windH = height;

	glViewport(0, 0, windW, windH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-0.5, windW + 0.5, -0.5, windH + 0.5);
	glMatrixMode(GL_MODELVIEW);
}

static void Init(void) {
	float angle;
	GLint n;

	srand((unsigned int) time(NULL));

	for (n = 0; n < MAXSTARS; n++) {
		NewStar(n, 100);
	}

	angle = 0.0;
	for (n = 0; n <= MAXANGLES; n++) {
		sinTable[n] = sin(angle);
		angle += PI / (MAXANGLES / 2.0);
	}
}

void Idle(void) {
	MoveStars();
	UpdateStars();
	if (nitro > 0) {
		speed = (float) (nitro / 10) + 1.0;
		if (speed > MAXWARP) {
			speed = MAXWARP;
		}
		if (++nitro > MAXWARP * 10) {
			nitro = -nitro;
		}
	} else if (nitro < 0) {
		nitro++;
		speed = (float) (-nitro / 10) + 1.0;
		if (speed > MAXWARP) {
			speed = MAXWARP;
		}
	}
	glutPostRedisplay();
}

void Visible(int state) {
	if (state == GLUT_VISIBLE) {
		glutIdleFunc(Idle);
	} else {
		glutIdleFunc(NULL);
	}
}

// program entry point
int main(int argc, char** argv) {
	// initialize opengl (via glut)
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow("Pong");

	Init();
	glutReshapeFunc(Reshape);
	glutVisibilityFunc(Visible);
	//glutDi//splayFunc(Display);

	//glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glutDisplayFunc(draw);
	glutTimerFunc(interval, update, 0);
	enable2D(width, height);
	//glColor3f(1.0f, 1.0f, 1.0f);
	glutMainLoop();
	return 0;
}
