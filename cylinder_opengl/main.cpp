//
//  main.cpp
//  cylinder_opengl
//
//  Created by Wolfgang Schulz on 22.12.17.
//  Copyright © 2017 EkwoTECH GmbH. All rights reserved.
//

#include <iostream>
#include <GLUT/glut.h>
#include <math.h>

#include "dreidee.hpp"
#include "cam.hpp"

GLdouble width, height;   /* window width and height */
int wd;                   /* GLUT window handle */

camera cam;

/* Program initialization NOT OpenGL/GLUT dependent,
 as we haven't created a GLUT window yet */
void init(void)
{
  width  = 1280.0;                 /* initial window width and height, */
  height = 800.0;                  /* within which we draw. */
  
  cam.eye = {0.0f,0.0f,-5.0f};
  cam.t = 0;
  
  return;
}

/* Callback functions for GLUT */

/* Draw the window - this is where all the GL actions are */
void display(void)
{
  /* clear the screen to white */
  glClear(GL_COLOR_BUFFER_BIT);

  glLoadIdentity();
  gluPerspective(45, 1.33, 0.1, 100);
//    glTranslatef(0,0,camz);
  glTranslatef(0,-20.0f*sin(cam.t*0.2f),-20.0f*cos(cam.t*0.2f));
/*
  // draw a cylinder
  glBegin(GL_LINE_STRIP);
  float fcenter = 0.0f;
  float fradius = 2.0f;
  for (int z = -5;z<=5;z++)
  {
    for (int i=0;i<34;i++)
    {
      float f = 0.2f*i;
      float p[3];
      p[0] = fcenter + fradius * cos(f);
      p[1] = fcenter + fradius * sin(f);
      p[2] = (float)z;
      glVertex3fv((GLfloat *) p);
    }
  }
  glEnd();
*/
  float fcenter = 0.0f;
  float fradius = 2.0f;
  for (int z = -5;z<=5;z++)
  {
    for (int i=0;i<33;i++)
    {
      // optimieren!! nicht mehrfach berechnen --> Quadstrip --> Triangles
      glColor3b(rand()%255, rand()%255, rand()%255);
      glBegin(GL_QUADS);
      float f = 0.2f*i;
      float p[3];
      p[0] = fcenter + fradius * cos(f);
      p[1] = fcenter + fradius * sin(f);
      p[2] = (float)z;
      glVertex3fv((GLfloat *) p);

      f = 0.2f*(i+1);
      p[0] = fcenter + fradius * cos(f);
      p[1] = fcenter + fradius * sin(f);
      p[2] = (float)z;
      glVertex3fv((GLfloat *) p);

      f = 0.2f*(i+1);
      p[0] = fcenter + fradius * cos(f);
      p[1] = fcenter + fradius * sin(f);
      p[2] = (float)z+1;
      glVertex3fv((GLfloat *) p);

      f = 0.2f*i;
      p[0] = fcenter + fradius * cos(f);
      p[1] = fcenter + fradius * sin(f);
      p[2] = (float)z+1;
      glVertex3fv((GLfloat *) p);

      glEnd();
    }
  }

  glFlush();
  return;
}

/* Called when window is resized,
 also when window is first created,
 before the first call to display(). */
void reshape(int w, int h)
{
  /* save new screen dimensions */
  width = (GLdouble) w;
  height = (GLdouble) h;
  
  /* tell OpenGL to use the whole window for drawing */
  glViewport(0, 0, (GLsizei) width, (GLsizei) height);
  
  /* do an orthographic parallel projection with the coordinate
   system set to first quadrant, limited by screen/window size */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
//    glOrtho(0.0, width, 0.0, height, -1.f, 1.f);
//    glFrustum(-1, 1.0, -1.0, 1.0, 1.5, 20.0);
//    glFrustum(-10.0f, 10.0f, -10.0f, 10.0f, 1.5f, 3.0f);
  gluPerspective(45, 1.33, 0.1, 100);
  return;
}

void kbd(unsigned char key, int x, int y)
{
  switch((char)key) {
    case 'q':
    case 27:    /* ESC */
      glutDestroyWindow(wd);
      exit(0);
    default:
      break;
  }
  
  return;
}

void SpecialInput(int key, int x, int y)
{
  switch(key)
  {
    case GLUT_KEY_UP:
      //do something here
      break;
    case GLUT_KEY_DOWN:
      //do something here
      break;
    case GLUT_KEY_LEFT:
      //do something here
      cam.t = (cam.t+1) % 34;
      cam.eye.z--;
      break;
    case GLUT_KEY_RIGHT:
      //do something here
      cam.t = (cam.t-1) % 34;
      cam.eye.z++;
      break;
  }
/*  dreidee::vec3 center;
  center.x = 0.0f;
  center.y = 0.0f;
  center.z = 0.0f;
  dreidee::vec3 dir;
  dir.x = center.x-cam.eye.x;
  dir.y = center.y-cam.eye.y;
  dir.z = center.z-cam.eye.z;
  dreidee::vec3 up;
  up.x = -dir.z;
  up.y = 0;
  up.z = dir.x;
  gluLookAt(cam.eye.x, cam.eye.y, cam.eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
*/
  glutPostRedisplay();
}

//    int main(int argc, const char * argv[]) {
int main(int argc, char * argv[]) {
  // insert code here...
  std::cout << "Hello, World!\n";

  /* perform initialization NOT OpenGL/GLUT dependent,
   as we haven't created a GLUT window yet */
  init();

  /* initialize GLUT, let it extract command-line
   GLUT options that you may provide
   - NOTE THE '&' BEFORE argc */
  glutInit(&argc, argv);
  
  /* specify the display to be single
   buffered and color as RGBA values */
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
  
  /* set the initial window size */
  glutInitWindowSize((int) width, (int) height);
  
  /* create the window and store the handle to it */
  wd = glutCreateWindow("OpenGL Cylinder" /* title */ );
  
  /* --- register callbacks with GLUT --- */
  
  /* register function to handle window resizes */
  glutReshapeFunc(reshape);
  
  /* register keyboard event processing function */
  glutKeyboardFunc(kbd);
  glutSpecialFunc(SpecialInput);
  
  /* register function that draws in the window */
  glutDisplayFunc(display);
  
  /* init GL */
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glColor3f(0.0, 0.0, 0.0);
  glLineWidth(3.0);
  
  /* start the GLUT main loop */
  glutMainLoop();

  return 0;
}
