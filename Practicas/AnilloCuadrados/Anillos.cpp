/* -------------------------------------------------------------------
 * Alfombra.h
 * versión 1.0
 * Copyright (C) 2016 
 * José Ricardo Rodríguez Abreu,
 * Facultad de Ciencias,
 * Universidad Nacional Autónoma de México, Mexico.
 *
 * Este programa es software libre; se puede redistribuir
 * y/o modificar en los términos establecidos por la
 * Licencia Pública General de GNU tal como fue publicada
 * por la Free Software Foundation en la versión 2 o
 * superior.
 *
 * Este programa es distribuido con la esperanza de que
 * resulte de utilidad, pero SIN GARANTÍA ALGUNA; de hecho
 * sin la garantía implícita de COMERCIALIZACIÓN o
 * ADECUACIÓN PARA PROPÓSITOS PARTICULARES. Véase la
 * Licencia Pública General de GNU para mayores detalles.
 *
 * Con este programa se debe haber recibido una copia de la
 * Licencia Pública General de GNU, de no ser así, visite el
 * siguiente URL:
 * http://www.gnu.org/licenses/gpl.html
 * o escriba a la Free Software Foundation Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * -------------------------------------------------------------------
 */
#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#ifdef __APPLE__
# define __gl_h_
# define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#endif
#include <OpenGL/gl3.h>
#define __gl_h_
#include <GL/freeglut.h>

/**
 * Metodo para actualizar el tamaño de la ventana.
 */
void reshape(int width, int height)
{
	glViewport(0, 0, 500, 500);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
}

/**
 * Muestra las figuras.
 */
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

/**
 * Inicializa el mundo.
 */
void init()
{
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_FLAT);
}

/**
 * Inicializa la ventana.
 */
void initWindow(){
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Hello OpenGL");
}

/**
 * Metodo principal del programa.
 */
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	initWindow();
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
} //Fin de Anillos.cpp
