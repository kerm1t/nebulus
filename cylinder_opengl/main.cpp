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
dreidee::vec3f cols[10*33];

enum rotation {rot_vert, rot_horiz};
rotation rot = rot_horiz;

//int playerx = 10;
//int playery = 0;

#define CAM_ROT M_PI*2/33

/* Program initialization NOT OpenGL/GLUT dependent,
 as we haven't created a GLUT window yet */
void init(void)
{
  width  = 1280.0;                 /* initial window width and height, */
  height = 800.0;                  /* within which we draw. */
  
//  cam.eye = {20.0f,0.0f,0.0f};
  cam.t = 1; // 2do: 0 stellt nix dar
  
  // set fix colors per QUAD --> later use this to get values from a map
  int idx=0;
  for (int z = -5;z<=5;z++)
  {
    for (int i=0;i<33;i++)
    {
      cols[idx++] = {rand()%255/255.0f,rand()%255/255.0f,rand()%255/255.0f};
    }
  }
  for (int i=0;i<33;i++) cols[i] = {1.0f,0,0};
  for (int i=0;i<33;i++) cols[9*33+i] = {1.0f,0,0};
  return;
}

/* Callback functions for GLUT */

/* Draw the window - this is where all the GL actions are */
void display(void)
{
  /* clear the screen to white */
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // Bildpuffer und Z-Buffer zurücksetzen

  glLoadIdentity();
  gluPerspective(45, 1.33, 0.1, 100);

  if (rot==rot_horiz)
  {
    cam.eye.x = -20.0f*sin(cam.t*0.2f)+0.01f; // es darf nicht alles 0 werden, s.u. up-Vektor!!
    cam.eye.y = -20.0f*cos(cam.t*0.2f)+0.01f;
    cam.eye.z = 0.0f;
  }
  if (rot==rot_vert)
  {
    cam.eye.x = -20.0f*sin(cam.t*0.2f)+0.01f;
    cam.eye.y = 0.0f;
    cam.eye.z = -20.0f*cos(cam.t*0.2f)+0.01f;
  }
  std::cout << "----" << std::endl;
  std::cout << "t:" << cam.t << std::endl;
  std::cout << "x:" << cam.eye.x << std::endl;
  std::cout << "y:" << cam.eye.y << std::endl;
  std::cout << "z:" << cam.eye.z << std::endl;
  dreidee::vec3f center;
  center.x = 0.0f;
  center.y = 0.0f;
  center.z = 0.0f;
  dreidee::vec3f dir;
  dir.x = center.x-cam.eye.x;
  dir.y = center.y-cam.eye.y;
  dir.z = center.z-cam.eye.z;
  // https://www.onlinemathe.de/forum/Bestimmung-zueinander-orthogonaler-Vektoren-2
  dreidee::vec3f up;
  up.x = dir.z;
  up.y = 0.0f;
  up.z = abs(dir.x);
  gluLookAt(cam.eye.x, cam.eye.y, cam.eye.z, center.x, center.y, center.z, up.x, up.y, up.z);

  float fcenter = 0.0f;
  float fradius = 2.0f;
  int idx=0;
  for (int z=-5;z<5;z++)
  {
    for (int i=0;i<33;i++)
    {
      glColor3f(cols[idx].x, cols[idx].y, cols[idx++].z); // pre-calculated, could be replaces with level-map

      // optimieren!! nicht mehrfach berechnen --> Quadstrip --> Triangles
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
  width  = (GLdouble) w;
  height = (GLdouble) h;
  
  /* tell OpenGL to use the whole window for drawing */
  glViewport(0, 0, (GLsizei) width, (GLsizei) height);
  
  /* do an orthographic parallel projection with the coordinate
   system set to first quadrant, limited by screen/window size */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, 1.33, 0.1, 10.0);
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
      rot = rot_vert;
      cam.t = (cam.t+1) % 34;
      break;
    case GLUT_KEY_DOWN:
      rot = rot_vert;
      cam.t = (cam.t-1) % 34;
      break;
    case GLUT_KEY_LEFT:
      rot = rot_horiz;
      cam.t = (cam.t+1) % 34;
      break;
    case GLUT_KEY_RIGHT:
      rot = rot_horiz;
      cam.t = (cam.t-1) % 34;
      break;
  }

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
  glLineWidth(2.0);
  
  glEnable(GL_DEPTH_TEST);
  // Cull backfacing polygons
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  

  /* start the GLUT main loop */
  glutMainLoop();

  return 0;
}
