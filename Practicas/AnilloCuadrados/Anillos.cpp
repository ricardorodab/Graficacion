
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

	//Color Rosado.
	glColor3f(1.0,0,1.0);

	//Mitad 1 del cuadrado grande.
	glBegin(GL_TRIANGLES);
	glVertex2f(1,1);      
	glVertex2f(-1,-1);
	glVertex2f(1,-1);

	//Mitad 2 del cuadrado grande.
	glBegin(GL_TRIANGLES);
	glVertex2f(1,1);      
	glVertex2f(-1,-1);
	glVertex2f(-1,1);

	//Color amarillo.
	glColor3f(1.0,1.1,0);	

	//Mitad 1 del cuadrado chico.
	glBegin(GL_TRIANGLES);
	glVertex2f(0.5,0.5);      
	glVertex2f(-0.5,-0.5);
	glVertex2f(0.5,-0.5);

	//Mitad 2 del cuadrado chico.
	glBegin(GL_TRIANGLES);
	glVertex2f(0.5,0.5);      
	glVertex2f(-0.5,-0.5);
	glVertex2f(-0.5,0.5);
	
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
