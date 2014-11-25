//---------------------------------------
// Program: pong.cpp
// Purpose: To play the class pong game.
// Author:  John Gauch
// Date:    Tue Feb 26 14:39:25 CST 2013
//          Tue Feb 26 16:27:10 CST 2013
//---------------------------------------
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
// Constants
const int WIN = 10;
//const int SIZE = 500;
const int HALF = 250;
const int RADIUS = 10;
const float SMALL = 0.01;

// Variables
float Speed;
float Px, Py;
float Vx, Vy;
float LeftPos, RightPos;
int LeftScore, RightScore;

//---------------------------------------
// Calculate random value between [low..high]
//---------------------------------------
float myrand(float low, float high)
{
   return low + (rand() * (high - low)) / RAND_MAX;
}

//---------------------------------------
// Reset ball at center
//---------------------------------------
void reset_ball()
{
   float theta = 0;
   if (LeftScore > RightScore)
      theta = myrand(3*M_PI/4, 5*M_PI/4);
   else
      theta = myrand(7*M_PI/4, 9*M_PI/4);
   Px = 0;
   Py = 0;
   Vx = cos(theta);
   Vy = sin(theta);
   Speed = 2 * SMALL;
}

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   // Initialize OpenGL
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-HALF, HALF, -HALF, HALF, -HALF, HALF);

   // Initialize pong board
   srand(clock());
   LeftPos = 0;
   RightPos = 0;
   LeftScore = 0;
   RightScore = 0;
   reset_ball();
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   // Check game over
   if ((LeftScore >= WIN) || (RightScore >= WIN)) return;

   // Clear screen
   glClear(GL_COLOR_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // Draw pong board
   glBegin(GL_POLYGON);
   glColor3f(0.1, 0.1, 0.8);
   glVertex2f(-HALF + RADIUS, -HALF + RADIUS);
   glVertex2f(-HALF + RADIUS,  HALF - RADIUS);
   glVertex2f( HALF - RADIUS,  HALF - RADIUS);
   glVertex2f( HALF - RADIUS, -HALF + RADIUS);
   glEnd();

   // Draw paddles
   glBegin(GL_POLYGON);
   glColor3f(0.8, 0.8, 0.8);
   glVertex2f(-HALF + 1*RADIUS, LeftPos - 10*RADIUS);
   glVertex2f(-HALF + 1*RADIUS, LeftPos + 10*RADIUS);
   glVertex2f(-HALF + 3*RADIUS, LeftPos + 10*RADIUS);
   glVertex2f(-HALF + 3*RADIUS, LeftPos - 10*RADIUS);
   glEnd();
   glBegin(GL_POLYGON);
   glColor3f(0.8, 0.8, 0.8);
   glVertex2f( HALF - 1*RADIUS, RightPos - 10*RADIUS);
   glVertex2f( HALF - 1*RADIUS, RightPos + 10*RADIUS);
   glVertex2f( HALF - 3*RADIUS, RightPos + 10*RADIUS);
   glVertex2f( HALF - 3*RADIUS, RightPos - 10*RADIUS);
   glEnd();

   // Draw ball
   glBegin(GL_POLYGON);
   glColor3f(0.8, 0.1, 0.1);
   glVertex2f(Px - RADIUS, Py - RADIUS);
   glVertex2f(Px - RADIUS, Py + RADIUS);
   glVertex2f(Px + RADIUS, Py + RADIUS);
   glVertex2f(Px + RADIUS, Py - RADIUS);
   glEnd();
   glFlush();
}

//---------------------------------------
// Idle callback for OpenGL
//---------------------------------------
void idle()
{
   // Check game over
   if ((LeftScore >= WIN) || (RightScore >= WIN)) return;

   // Move bouncing ball
   Px += Vx * Speed;
   Py += Vy * Speed;

   // Bounce off paddles
   if ((Px >  HALF - 4*RADIUS) && (abs(Py-RightPos) < 10*RADIUS))
      {Px =  HALF - 4*RADIUS; Vx *= -1;}
   if ((Px < -HALF + 4*RADIUS) && (abs(Py-LeftPos) < 10*RADIUS))
      {Px = -HALF + 4*RADIUS; Vx *= -1;}

   // Bounce off walls
   if (Py >  HALF - 2*RADIUS)
      {Py =  HALF - 2*RADIUS; Vy *= -1;}
   if (Py < -HALF + 2*RADIUS)
      {Py = -HALF + 2*RADIUS; Vy *= -1;}

   // Increment scores
   bool Reset = false;
   if (Px >  HALF)
      { LeftScore++; Reset = true; }
   if (Px < -HALF)
      { RightScore++; Reset = true; }

   // Reset ball
   if (Reset)
   {
      reset_ball();
      printf("SCORE -- Left %d  Right %d\n", LeftScore, RightScore);
      if (LeftScore >= WIN) printf("GAME OVER -- Left Wins\n");
      if (RightScore >= WIN) printf("GAME OVER -- Right Wins\n");
   }

   glutPostRedisplay();
}

//---------------------------------------
// Special callback for OpenGL
//---------------------------------------
void special(int key, int x, int y)
{
   printf("%d\n", key);
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   printf("%d\n", int(key));

   // Check game over
   if ((LeftScore >= WIN) || (RightScore >= WIN)) return;

   if ((key == 'q') && (LeftPos < HALF - 11*RADIUS))
      LeftPos += RADIUS*2;
   else if ((key == 'a') && (LeftPos > -HALF + 11*RADIUS))
      LeftPos -= RADIUS*2;
   else if ((key == 'p') && (RightPos < HALF - 11*RADIUS))
      RightPos += RADIUS*2;
   else if ((key == 'l') && (RightPos > -HALF + 11*RADIUS))
      RightPos -= RADIUS*2;
   else if ((key == '+') && (Speed < 1))
      Speed += SMALL;
   else if ((key == '-') && (Speed > 0))
      Speed -= SMALL;

   // Redraw objects
   glutPostRedisplay();
}

//---------------------------------------
// Print command menu
//---------------------------------------
void print_menu()
{
   printf("\nGame controls:\n");
   printf("  'q' = Left Paddle Up\n");
   printf("  'a' = Left Paddle Down\n");
   printf("  'p' = Right Paddle Up\n");
   printf("  'l' = Right Paddle Down\n");
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   // Create OpenGL window
   glutInit(&argc, argv);
   glutInitWindowSize(500, 600);
   glutInitWindowPosition(HALF, HALF);
   glutInitDisplayMode(GLUT_RGB);
   glutCreateWindow("Pong");
   init();
   print_menu();

   // Specify callback function
   glutDisplayFunc(display);
   glutIdleFunc(idle);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(special);
   glutMainLoop();
   return 0;
}
