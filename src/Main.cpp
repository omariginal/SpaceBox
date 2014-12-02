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
#include "Paddle.hpp"
#include "ball.hpp"

#define DEG2RAD (M_PI/180.0)
#define MAXSTARS 4000
#define MAXPOS 10000
#define MAXWARP 10
#define MAXANGLES 6000

int KEY_W = 0x57;
int KEY_A = 0x41;
int KEY_S = 0x53;
int KEY_D = 0x44;


//UPdate rate [fps]
int interval = 1000 / 60;

//Paddle stats
int paddleShort = 20;
int paddleLong = 180;


Paddle leftPaddle;
Paddle rightPaddle;
Paddle topPaddle;
Paddle botPaddle;
Ball ball;

GLint play = 1;

GLint STREAK = 0;
GLint CIRCLE = 1;

typedef struct _starRec {
	GLint type;
	float x[2], y[2], z[2];
	float offsetX, offsetY, offsetR, rotation;
} starRec;

GLenum doubleBuffer;
GLint windW = 1000, windH = 600;
GLint starCount = MAXSTARS / 2;
float speed = 0.2;
GLint nitro = 1;
starRec stars[MAXSTARS];
float sinTable[MAXANGLES];

float Sin(float angle) {
	return (sinTable[(GLint) angle]);
}

float Cos(float angle) {
	return (sinTable[((GLint) angle + (MAXANGLES / 4)) % MAXANGLES]);
}

std::string int2str(int x) {
	std::stringstream ss;
	ss << x;
	return ss.str();
}



void keyboard() {
	// left paddle
	if (GetAsyncKeyState(VK_UP) && (rightPaddle.posY < 500)) {
		rightPaddle.posY += rightPaddle.speed;
	}
	if (GetAsyncKeyState(VK_DOWN) && (rightPaddle.posY > 0)) {
		rightPaddle.posY -= rightPaddle.speed;
	}
	if (GetAsyncKeyState(VK_LEFT) && (botPaddle.posX > 0)) {
		botPaddle.posX -= botPaddle.speed;
	}
	if (GetAsyncKeyState(VK_RIGHT) && (botPaddle.posX < 1000)) {
		botPaddle.posX += botPaddle.speed;
	}
	if (GetAsyncKeyState(KEY_W) && (leftPaddle.posY < 500)) {
		leftPaddle.posY += leftPaddle.speed;
	}
	if (GetAsyncKeyState(KEY_S) && (leftPaddle.posY > 0)) {
		leftPaddle.posY -= leftPaddle.speed;
	}
	if (GetAsyncKeyState(KEY_A) && (topPaddle.posX > 0)) {
		topPaddle.posX -= topPaddle.speed;
	}
	if (GetAsyncKeyState(KEY_D) && (topPaddle.posX < 1000)) {
		topPaddle.posX += topPaddle.speed;
	}
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
			glColor3f(1.0, 1, (MAXWARP - speed) / MAXWARP);
			if (fabs(x0 - x1) < 1.0 && fabs(y0 - y1) < 1.0) {
				glBegin(GL_POINTS);
				glVertex2f(x0, y0);
				glEnd();
			} else {
				glBegin(GL_LINES);
				glColor4f(1.0, 1.0, 0.0,.70);
				glVertex2f(x0, y0);
				glVertex2f(x1, y1);
				glEnd();
			}
		} else {
			width = MAXPOS / 10.0 / stars[n].z[0] + 1.0;
			glColor4f(1.0, 1.0, 0.0,.70);
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

	stars[n].offsetX = ball.posX/100;
	stars[n].offsetY = ball.posY/100;
	stars[n].offsetR = 1.2;
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable( GL_DEPTH_TEST);
	glLoadIdentity();
	if (play == 1) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable( GL_BLEND);
		glColor4f(.23, .78, .32, 0.1);
		ShowStars();
		//draw paddles
		leftPaddle.draw(leftPaddle.posX, leftPaddle.posY, leftPaddle.width,
				leftPaddle.height);
		rightPaddle.draw(rightPaddle.posX, rightPaddle.posY, rightPaddle.width,
				rightPaddle.height);
		topPaddle.draw(topPaddle.posX, topPaddle.posY, topPaddle.width,
				topPaddle.height);
		botPaddle.draw(botPaddle.posX, botPaddle.posY, botPaddle.width,
				botPaddle.height);
		//draw ball
		ball.draw(ball.posX - ball.size / 2, ball.posY - ball.size / 2,
				ball.size);

	}

	glutSwapBuffers();
}

void vec2_norm(float& x, float &y) {
	float length = sqrt((x * x) + (y * y));
	if (length != 0.0f) {
		length = 1.0f / length;
		x *= length;
		y *= length;
	}
}

// Testing collisions
void updateBall() {
	ball.posX += ball.dirX * ball.speed;
	ball.posY += ball.dirY * ball.speed;
	// hit by left paddle
	if (ball.posX < leftPaddle.posX + paddleShort && ball.posX > leftPaddle.posX
			&& ball.posY < leftPaddle.posY + paddleLong
			&& ball.posY > leftPaddle.posY) {
		float t = ((ball.posY - leftPaddle.posY) / paddleLong) - 0.5;
		ball.dirX = fabs(ball.dirX); // force it to be positive
		ball.dirY = t;
		ball.speed = ball.speed + 1.1;
		PlaySound((LPCSTR) "src/Mario_Jump_Sound.wav", NULL,
				SND_FILENAME | SND_ASYNC);
	}
	// hit by right paddle
	if (ball.posX > rightPaddle.posX
			&& ball.posX < rightPaddle.posX + paddleShort
			&& ball.posY < rightPaddle.posY + paddleLong
			&& ball.posY > rightPaddle.posY) {
		float t = ((ball.posY - rightPaddle.posY) / paddleLong) - 0.5f;
		ball.dirX = -fabs(ball.dirX); // force it to be negative
		ball.dirY = t;
		ball.speed = ball.speed + 1.1;
		PlaySound((LPCSTR) "src/Mario_Jump_Sound.wav", NULL,
				SND_FILENAME | SND_ASYNC);
	}
	//hit by top paddle
	if (ball.posX > topPaddle.posX && ball.posX < topPaddle.posX + paddleLong
			&& ball.posY > topPaddle.posY) {
		float t = ((ball.posX - topPaddle.posX) / paddleLong) - 0.5f;
		ball.dirY = -fabs(ball.dirY); // force it to be negative
		ball.dirX = t;
		ball.speed = ball.speed + 1.1;
		PlaySound((LPCSTR) "src/Mario_Jump_Sound.wav", NULL,
				SND_FILENAME | SND_ASYNC);
	}
	//hit by bottom paddle
	if (ball.posX > botPaddle.posX && ball.posX < botPaddle.posX + paddleLong
			&& ball.posY < botPaddle.posY + 2 * paddleShort) {
		float t = ((ball.posX - botPaddle.posX) / paddleLong) - 0.5f;
		ball.dirY = fabs(ball.dirY); // force it to be positive
		ball.dirX = t;
		ball.speed = ball.speed + 1.1;
		PlaySound((LPCSTR) "src/Mario_Jump_Sound.wav", NULL,
				SND_FILENAME | SND_ASYNC);
	}
	// hit left wall
	if (ball.posX < 0) {
		ball.posX = windW / 2;
		ball.posY = windH / 2;
		ball.dirX = fabs(ball.dirX);
		ball.dirY = 0;
		ball.speed = 5.0;
		PlaySound((LPCSTR) "src/Punch_Sound_Effect.wav", NULL,
				SND_FILENAME | SND_ASYNC);
	}

	// hit right wall
	if (ball.posX > windW) {
		ball.posX = windW / 2;
		ball.posY = windH / 2;
		ball.dirX = -fabs(ball.dirX);
		ball.dirY = 0;
		ball.speed = 5.0;
		PlaySound((LPCSTR) "src/Punch_Sound_Effect.wav", NULL,
				SND_FILENAME | SND_ASYNC);
	}

	// hit top wall
	if (ball.posY > windH) {
		ball.posX = windW / 2;
		ball.posY = windH / 2;
		ball.dirX = -fabs(ball.dirX);
		ball.dirY = 0;
		ball.speed = 5.0;
		PlaySound((LPCSTR) "src/Punch_Sound_Effect.wav", NULL,
				SND_FILENAME | SND_ASYNC);
	}
	// hit bottom wall
	if (ball.posY < 0) {
		ball.posX = windW / 2;
		ball.posY = windH / 2;
		ball.dirX = 0;
		ball.dirY = fabs(ball.dirY);
		ball.speed = 5.0;
		PlaySound((LPCSTR) "src/Punch_Sound_Effect.wav", NULL,
				SND_FILENAME | SND_ASYNC);
	}
	vec2_norm(ball.dirX, ball.dirY);
}

void update(int value) {
	keyboard();
	updateBall();
	glutTimerFunc(interval, update, 0);
	glutPostRedisplay();
}

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
	glMatrixMode(GL_MODELVIEW);
}

static void Init(void) {
	float angle;
	GLint n;
	srand((unsigned int) time(NULL));
	for (n = 0; n < MAXSTARS; n++) {
		NewStar(n, 100);
	}
	angle = 23.0;
	for (n = 0; n <= MAXANGLES; n++) {
		sinTable[n] = sin(angle);
		angle += M_PI / (MAXANGLES / 2.0);
	}



	botPaddle.width = paddleLong;
	botPaddle.height = paddleShort;
	topPaddle.width = paddleLong;
	topPaddle.height = paddleShort;
	rightPaddle.width = paddleShort;
	rightPaddle.height = paddleLong;
	leftPaddle.width = paddleShort;
	leftPaddle.height = paddleLong;
	//Start positions of paddles.
	leftPaddle.posX = 10;
	leftPaddle.posY = (600-paddleShort)/2;
	rightPaddle.posX = windW - paddleShort - 10;
	rightPaddle.posY = (600-paddleShort)/2;
	botPaddle.posX = (1000-paddleLong)/2;
	botPaddle.posY = 10;

	topPaddle.posX = (1000-paddleLong)/2;
	topPaddle.posY = windH - paddleShort - 10;
}

void Idle(void) {
	MoveStars();
	UpdateStars();
	glutPostRedisplay();
}

void Visible(int state) {
	if (state == GLUT_VISIBLE) {
		glutIdleFunc(Idle);
	} else {
		glutIdleFunc(NULL);
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(windW, windH);
	glutCreateWindow("SpaceBox");
	Init();
	glutReshapeFunc(Reshape);
	glutVisibilityFunc(Visible);
	glutDisplayFunc(draw);
	glutTimerFunc(interval, update, 0);
	enable2D(windW, windH);
	mciSendString("open src/Z2Pa66C8Y1c.wav alias announce", 0, 0, 0);
	mciSendString("play announce", 0, 0, 0);
	glutMainLoop();
	return 0;
}

