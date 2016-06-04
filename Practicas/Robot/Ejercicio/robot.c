/* -------------------------------------------------------------------
 * robot.c
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
#include <stdio.h>
#include <GL/glew.h>
#ifdef __APPLE__
# define __gl_h_
# define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#endif
#include <OpenGL/gl3.h>
#define __gl_h_
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "parts.h"

/* Atributos que podemos modificar para mover a el objeto robot. */
static int cinturaInt = 0, codoInt = 0, radioInt = 0, largoInt = 0, sube = 1, largo = 1, dedosCerrados = 0;
/* Atributo para mover a el robot completo */
static double cinturaDouble = 0;
/* Los puntos donde se traslada la pelota. */
static double puntosPelota[3] = {0,0,0};
/* La matriz del viewPort */
static float mvm[16];

/* Función para las configuraciones iniciales. */
void myInit(char *progname)
{
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(500, 500); 
  glutInitWindowPosition(0, 0);
  glutCreateWindow(progname);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  
}

/* Codigo de para imprimir texto: https://www.opengl.org/discussion_boards/showthread.php/169189-Printing-Text */
void print(double x, double y,double z, char *string)
{
  //set the position of the text in the window using the x and y coordinates
  glRasterPos3f(x-.7,y+2.5,z);
  //get the length of the string to display
  int len = (int)strlen(string);
  //loop to display character by character
  for (int i = 0; i < len; i++)
    {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,string[i]);
    }
}


/* Función principal para mostrar cosas en pantalla. */
void myDisplay(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3ub(255, 255, 255); // change color to red
  print(0,0,0,"S/s = Girar todo el robot");
  print(0,-.2,0,"e = Inclinar el robot");
  print(0,-.4,0,"d = Bajar brazo");
  print(0,-.6,0,"C/c = Cerrar/Abrir mano");
  glPushMatrix();
  glRotated((double)0.0, cinturaInt, 0.0, 1.0);
  glTranslated(1.5, -1.0, 0.0);  
  base();
  cintura(cinturaInt);
  brazo(radioInt);
  codo();
  mano(largo, radioInt);
  nudillo(largo);
  dedos(mvm, dedosCerrados);
  if(dedosCerrados == 0)
    {
      glPopMatrix();
      glPushMatrix();
      glTranslated(1.5, -1.0, 0.0);
      puntosPelota[0] = puntosPelota[1] = puntosPelota[2] = 0;
    }
  else
    {
      puntosPelota[0] = -1.9;
      puntosPelota[1] = 0;
      puntosPelota[2] = 0;
    }
  pelota(puntosPelota);
  glPopMatrix();  
  glutSwapBuffers();
}

/* Función para poder volver a dibujar con algún cambio. */
void myReshape (int width, int height)
{
  glViewport(0, 0, width, height); 
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (double)width/(double)height, 0.1, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslated(-2.0, 0.0, -5.0); // move to enable viewing
}

/* Funciones del teclado. */
void myKeyboard (unsigned char key, int x, int y)
{
  switch (key) {
  case 's':
    cinturaInt = (cinturaInt + 5) % 360;
    cinturaDouble = (cinturaDouble + .1);
    glutPostRedisplay();
    break;
  case 'S':
    cinturaInt = (cinturaInt - 5) % 360;
    glutPostRedisplay();
    break;
  case 'e':
    if(radioInt > 20)
      {
	sube = 1;
      }
    else if(radioInt < -20)
      {
	sube = 0;
      }
    if(sube)
      {
	radioInt = radioInt - 5; 
      }
    else
      {
	radioInt = radioInt + 5;
      }
    glutPostRedisplay();
    break;
  case 'E':
    if(radioInt > 20)
      {
	sube = 1;
      }
    else if(radioInt < -20)
      {
	sube = 0;
      }
    if(sube)
      {
	radioInt = radioInt - 5; 
      }
    else
      {
	radioInt = radioInt + 5;
      }
    glutPostRedisplay();
    break;
  case 'd':
    if(largo)
      {
	largo = 0;
      }
    else
      {
	largo = 1;
      }
    glutPostRedisplay();
    break;
    break;
  case 'c':
  case 'C':
    dedosCerrados = dedosCerrados == 0? 1 : 0;
    glutPostRedisplay();
    break;
  case 27:
    exit(0);
    break;
  default:
    break;
  }
}

/* Función principal de la clase. */
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  myInit(argv[0]);
  glEnable(GL_DEPTH_TEST);
  glutKeyboardFunc(myKeyboard);
  glutReshapeFunc(myReshape);
  glutDisplayFunc(myDisplay); 
  glutMainLoop();
  return 0;
}
