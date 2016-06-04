
#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
//#include "Dependencies\glew\glew.h"// cambienlo de acuerdo a como ustedes lo tengan
#include <GL/glew.h>
#ifdef __APPLE__
# define __gl_h_
# define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#endif
//#include "Dependencies\freeglut\freeglut.h"// cambienlo de acuerdo a como ustedes lo tengan
#include <OpenGL/gl3.h>
#define __gl_h_
#include <GL/freeglut.h>

void reshape(int width, int height)
{
	glViewport(0, 0, 500, 500);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);	

	
	glBegin(GL_TRIANGLES);
	glColor3f(0,1,0);
	glVertex2f(-0.2, 0.3);
	glColor3f(0,1.0,1.0);
	glVertex2f(0.2, 0.3);
	glColor3f(1.0,1.0,0);
	glVertex2f(0,0.9);

	glColor3f(0,1.0,0);
	
	glBegin(GL_TRIANGLES);
	glVertex2f(0.8,0.3);
	glVertex2f(-0.8,0.3);
	glVertex2f(0,-0.4);
	
	glColor3f(1.0,0,1.0);
	
	glBegin(GL_TRIANGLES);
	glVertex2f(0,-0.4);
	glVertex2f(-0.6,-0.9);
	glVertex2f(-0.3,-0.1);
	
	glBegin(GL_TRIANGLES);
	glVertex2f(0,-0.4);      
	glVertex2f(0.6,-0.9);
	glVertex2f(0.3,-0.1);
	glColor3f(1.0,0,1.0);
	glEnd();

	glFlush();
}

void init()
{
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_FLAT);
}

void initWindow(){
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Hello OpenGL");
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	initWindow();
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}